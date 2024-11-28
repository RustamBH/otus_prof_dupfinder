#include "FileClass.h"



std::string FileClass::GetFileName() const
{
	return name;
}

bool FileClass::Open()
{
	if (pFile)
		return true;
	if (!(pFile = fopen(name.c_str(), "r")))
		return false;
	fseek(pFile, offset, SEEK_SET);
	return true;
}

bool FileClass::Close()
{
	if (!pFile)
		return true;
	int success = !fclose(pFile);
	if (success)
		pFile = NULL;		
	return success;
}

bool FileClass::EoF() const
{
	return reached_eof;
}

size_t FileClass::Read(void *buf, size_t count)
{
	count = fread(buf, 1, count, pFile);
	offset += count;
	reached_eof = feof(pFile);
	return count;
}

