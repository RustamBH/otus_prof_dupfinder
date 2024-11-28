#pragma once

#include "FileClass.h"
#include "Config.h"
#include <ranges>
#include <boost/regex.hpp>

using VectorMap = vectormap<uintmax_t, FileClass>;

class DupFinder
{
public:	
	void FindDuplicates(const Config& cfg, ostream& of, size_t block_size);
	inline size_t GetCRC32(void const* buf, size_t bytes);
	inline size_t GetMD5(void const* buf, size_t bytes);
};