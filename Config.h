#pragma once

#include <vector>
#include <string>

using VectorString = std::vector<std::string>;

// size in bytes of file chunks to be compared separately
const size_t HASHED_BLOCK_SIZE = 1024 * 1024;


enum class search_t
{
    Deep = 0,   // deep search
    Simple,     // simple search
};


enum class hash_t
{
    MD5 = 0,  // MD5 hash
    CRC32,    // CRC32 hash
};

class Config
{
public:
    bool Parse(int argc, char* argv[]);    
    VectorString GetDirSearch() const;    
    VectorString GetDirExclude() const;   
    search_t GetSearchType() const;    
    size_t GetMinFileSizeBt() const;   
    std::string GetFileMask() const;    
    size_t GetBlockSize() const; 
    hash_t GetHashType() const;
private:
    VectorString m_dir_search; //  Directories to search
    VectorString m_dir_exclude; // Directories to exclude from search
    search_t m_search_t = search_t::Deep; // Search type
    size_t m_min_file_sz_bt = 1; //  Minimum file size in bytes
    std::string m_file_mask; // File mask to filter files
    size_t m_block_sz = 0; // Block size for file reading
    hash_t m_hash_t = hash_t::MD5; // Hash type
};