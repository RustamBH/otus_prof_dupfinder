#include "DupFinder.h"


inline size_t DupFinder::GetCRC32(void const *buf, size_t bytes)
{
	crc_32_type result{};
   	result.process_bytes(buf, bytes);
	return result.checksum();
}


inline size_t DupFinder::GetMD5(void const* buf, size_t bytes)
{
	md5 hash{};
	md5::digest_type digest{};

	hash.process_bytes(buf, bytes);
	hash.get_digest(digest);

	const auto size_t_digest{ reinterpret_cast<const size_t*>(&digest) };
	return *size_t_digest;
}


bool is_exclude_dir(const directory_entry& entry, const VectorString& exclude_dirs)
{
	auto res = [&entry](const std::string& exclude_dir) { return entry.path().string().find(exclude_dir) != std::string::npos; };	
	return std::ranges::any_of(exclude_dirs, res);
}


bool is_file_match_by_mask(const Config& cfg, const directory_entry& entry)
{
	const boost::regex file_mask(cfg.GetFileMask());	
	if (file_mask.size() == 0)	
		return true;

	return boost::regex_match(entry.path().filename().string(), file_mask);	
}


void DupFinder::FindDuplicates(const Config& cfg, ostream& of, size_t block_size)
{
	vector<vector<FileClass> > groups;	
	std::unique_ptr<char[]> pbuf{std::make_unique<char[]>(HASHED_BLOCK_SIZE)};
	//std::unique_ptr<char> pbuf{ new char[HASHED_BLOCK_SIZE] };
	char* buf = pbuf.get();

	VectorMap files_by_size;

	for (const auto& srch_dir : cfg.GetDirSearch())
	{
		if (cfg.GetSearchType() == search_t::Deep) {
			// Perform initial grouping of files by their size	
			for (recursive_directory_iterator iter(srch_dir), end; iter != end; ++iter)
			{
				const auto entry = *iter;				
				if (is_exclude_dir(entry, cfg.GetDirExclude()))
				{						
					continue; // skip excluded dir	
				}			
				
				string name = iter->path().string();
				if (!is_directory(name))
				{
					const auto file_size_bt{ file_size(entry) };
					if (file_size_bt < cfg.GetMinFileSizeBt())
					{						
						continue; // skip files which size is less then minimum size
					}

					if (!is_file_match_by_mask(cfg, entry))
					{						
						continue; // skip files that do not match the mask
					}

					uintmax_t size = file_size(name);
					VectorMap::iterator p = files_by_size.find(size);
					files_by_size.insert(size, FileClass(name));
				}
			}
		}
		else {
			// Perform initial grouping of files by their size	
			for (directory_iterator iter(srch_dir), end; iter != end; ++iter)
			{
				const auto entry = *iter;
				if (is_exclude_dir(entry, cfg.GetDirExclude()))
				{
					continue;
				}

				string name = iter->path().string();
				if (!is_directory(name))
				{
					const auto file_size_bt{ file_size(entry) };
					if (file_size_bt < cfg.GetMinFileSizeBt())
					{
						continue;
					}

					if (!is_file_match_by_mask(cfg, entry))
					{
						continue;
					}
					uintmax_t size = file_size(name);
					VectorMap::iterator p = files_by_size.find(size);
					files_by_size.insert(size, FileClass(name));
				}
			}	
		}

		for (VectorMap::iterator it = files_by_size.begin(); it != files_by_size.end(); ++it)
			if (it->second.size() > 1) // ignore one-FileClass groups	
				groups.push_back(it->second);

		files_by_size.clear(); // no longer needed

		// Analyze formed groups as a stack, by splitting them further when hashes of files chunks don't match
		while (!groups.empty())
		{
			vector<FileClass> group = groups.back();
			groups.pop_back();

			if (!group.begin()->EoF())
			{
				// Read a block of data from every FileClass in the group, and compare their CRC32 hashes
				VectorMap files_by_hash; 
				for (vector<FileClass>::iterator FileClass = group.begin(); FileClass != group.end(); ++FileClass)
				{
					if (FileClass->Open())
					{
						int bytes_read = FileClass->Read(buf, block_size);
						if (cfg.GetHashType() == hash_t::CRC32) {					
							files_by_hash.insert(GetCRC32(buf, bytes_read), *FileClass);
						}
						else if (cfg.GetHashType() == hash_t::MD5) {
							files_by_hash.insert(GetMD5(buf, bytes_read), *FileClass);
						}					
					}			
				}

				// Split group into smaller ones, according to the hashes of the last read blocks 
				for (VectorMap::iterator it = files_by_hash.begin(); it != files_by_hash.end(); ++it)
					if (it->second.size() > 1) // ignore one-FileClass groups
						groups.push_back(it->second);
					else
						it->second.begin()->Close();
			}
			else // EOF was reached and all hashes matched, print out FileClass names
			{
				for (vector<FileClass>::iterator FileClass = group.begin(); FileClass != group.end(); ++FileClass)
					of << FileClass->GetFileName() << endl;
				of << endl;
			}
		}
	}
}