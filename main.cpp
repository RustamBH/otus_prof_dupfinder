#include "DupFinder.h"
#include <stdexcept>


int main(int argc, char* argv[])
{
	try
	{
		if (argc < 2)
		{
			std::cout << "Too small command-line arguments" << endl;
			return -1;
		}

		Config cfg;
		cfg.Parse(argc, argv);

		std::cout << "Duplicate file finder\n";
		std::cout << "deep search    : " << (cfg.GetSearchType() == search_t::Deep ? "all dir" : "one dir") << std::endl;
		std::cout << "min file size: " << cfg.GetMinFileSizeBt() << std::endl;
		std::cout << "file mask    : " << cfg.GetFileMask() << std::endl;
		std::cout << "block size   : " << cfg.GetBlockSize() << std::endl;
		std::cout << "hash type : " << (cfg.GetHashType() == hash_t::MD5 ? "md5" : "crc32") << std::endl << std::endl;

		DupFinder df;
		df.FindDuplicates(cfg, cout, HASHED_BLOCK_SIZE);
	}
	catch (const std::invalid_argument& ex)
	{
		std::cerr << ex.what() << std::endl;
	}

	return 0;
}
