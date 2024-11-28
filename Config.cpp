#include "Config.h"
#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;


bool Config::Parse(int argc, char* argv[])
{
    try
    {
        po::options_description desc("Arguments");
        desc.add_options()
            ("help,H", "produce help message")
            ("directories,D", po::value<VectorString>()->multitoken(), "directories to search")
            ("exclude,E", po::value<VectorString>()->multitoken(), "directories to exclude")
            ("deep-search,P", po::value<size_t>()->default_value(1), "1 - include subdirectories, 0 - only directory")
            ("min-file-size,F", po::value<size_t>()->default_value(1), "minimum file size in bytes")
            ("file-mask,M", po::value<std::string>()->default_value(""), "mask names of files allowed for comparison")
            ("block-size,S", po::value<size_t>()->default_value(4096), "block size for reading files")
            ("hash-type,T", po::value<size_t>()->default_value(1), "hash type 0 - crc32, 1 - md5");

        po::variables_map vm{};
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            desc.print(std::cout);
            return false;
        }

        if (vm.count("directories") == 0)
        {
            throw std::invalid_argument("No directories specified");
        }
        m_dir_search = vm["directories"].as<VectorString>();

        if (vm.count("exclude"))
        {
            m_dir_exclude = vm["exclude"].as<VectorString>();
        }

        if (const auto deep_search{ vm["deep-search"].as<size_t>() }; deep_search == 1)
        {
            m_search_t = search_t::Deep;
        }
        else if (deep_search == 0)
        {
            m_search_t = search_t::Simple;
        }
        else
        {
            throw std::invalid_argument("Invalid deep search");
        }

        m_min_file_sz_bt = vm["min-file-size"].as<size_t>();
        m_file_mask = vm["file-mask"].as<std::string>();
        m_block_sz = vm["block-size"].as<size_t>();

        if (const auto hash_type{ vm["hash-type"].as<size_t>() }; hash_type == 1)
        {
            m_hash_t = hash_t::MD5;
        }
        else if (hash_type == 0)
        {
            m_hash_t = hash_t::CRC32;
        }
        else
        {
            throw std::invalid_argument("Invalid hash type");
        }

        return true;
    }
    catch (const po::unknown_option& ex)
    {     
        throw std::invalid_argument(ex.what());
    }
    catch (const po::invalid_command_line_syntax& ex)
    {        
        throw std::invalid_argument(ex.what());
    }
}

VectorString Config::GetDirSearch() const
{
    return m_dir_search;
}

VectorString Config::GetDirExclude() const
{
    return m_dir_exclude;
}

search_t Config::GetSearchType() const
{
    return m_search_t;
}

size_t Config::GetMinFileSizeBt() const
{    
    return m_min_file_sz_bt;
}

std::string Config::GetFileMask() const
{
    return m_file_mask;
}

size_t Config::GetBlockSize() const
{
    return m_block_sz;
}

hash_t Config::GetHashType() const
{
    return m_hash_t;
}
