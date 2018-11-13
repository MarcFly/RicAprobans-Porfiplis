#include "FileHelpers.h"
#include "Globals.h"
#include "MD5.h"
#include <string.h>

std::string GetDir(const char* full_path)
{
	if (full_path != nullptr)
	{
		const char* test = strrchr(full_path, '\\/');
		const char* test1 = strchr(full_path, '\\/');
		std::string aux = full_path;
		if (test == NULL)
		{
			if (test1 != NULL)
				aux = test1;
			else
			{
				int last = aux.find_last_of('\\');
				aux.erase(last + 1);
				return aux;
			}
		}
		else
			aux = test;

		std::string path = full_path;
		path.erase(path.length() - aux.length() + 1);
		return path;
	}
	return std::string("");
}

void CreateHiddenDir(const char* dir)
{
	CreateDirectory(dir, NULL);
	SetFileAttributes(dir, FILE_ATTRIBUTE_HIDDEN);
}

uint GetExtSize(const char* file)
{
	std::string ret = strrchr(file, '.');

	return ret.length();
}

std::string GetFileName(const char* file)
{
	std::string ret = file;

	ret = ret.substr(ret.find_last_of("\\/") + 1);

	const char* test = strrchr(ret.c_str(), '.');
	if (test != NULL && test != nullptr)
	{
		std::string aux = test;
		ret.erase(ret.length() - aux.length()).c_str();
	}
	return ret;
}

Uint32 GetUUID()
{
	pcg32_random_t rng;
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	QueryPerformanceCounter(&li);
	pcg32_srandom_r(&rng, li.QuadPart, (intptr_t)&rng);

	return pcg32_random_r(&rng);
}

std::string GetMD5ID(const char* file)
{
	std::string send(file);
	return GetMD5ID(send);
}

std::string GetMD5ID(std::string file)
{
	MD5 md5;
	std::string str(file);
	char* dest = new char[str.length() + 1];
	std::copy(str.begin(), str.end(), dest);
	char* cursor = dest + str.length();
	*cursor = '\0';
	const char* ret = md5.digestFile(dest);
	std::string retstr(ret);
	return retstr;
}