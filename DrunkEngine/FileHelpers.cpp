#include "FileHelpers.h"
#include "Globals.h"
#include "Libs/picosha2.h"

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


std::string GetHexID(const char* data) 
{
	std::vector<unsigned char> hash(picosha2::k_digest_size);
	std::string str = data;
	picosha2::hash256(str.begin(), str.end(), hash.begin(), hash.end());

	return picosha2::bytes_to_hex_string(hash.begin(), hash.end()).c_str();
}

std::string GetHexID(char* data, int size)
{
	std::vector<unsigned char> hash(picosha2::k_digest_size);
	std::string str;
	str.copy(data, size);
	picosha2::hash256(str.begin(), str.end(), hash.begin(), hash.end());

	return picosha2::bytes_to_hex_string(hash.begin(), hash.end()).c_str();
}