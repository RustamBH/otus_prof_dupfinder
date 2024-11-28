#pragma once

#include <iostream>
#include <cstdio>
#include <boost/filesystem.hpp>
#include <boost/integer.hpp>
#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>
#include "vectormap.h"

using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace boost::uuids::detail;


class FileClass
{
private:
	std::string name;
	FILE *pFile;
	long offset;
	bool reached_eof;

public:	
	FileClass(const std::string& filename) : name(filename), pFile(NULL), offset(0), reached_eof(false) {};
	std::string GetFileName() const;
	bool Open();
	bool Close();
	bool EoF() const;
	size_t Read(void *buf, size_t count);
};
