#pragma once
#include <array>
#include <filesystem>
#include <fstream>
#include <span>

class ProjectsData
{
public:
	ProjectsData()
	{
		MakeCache();
	}
public:
	std::span<const std::filesystem::path>
		GetRecent()const noexcept
	{
		return { projects.data(), nreal };
	}
private:
	void MakeCache()
	{
		namespace fs = std::filesystem;
		fs::path cache{ "cache.vtxc" };
		std::fstream f;
		std::string s;

		if (!fs::exists(cache) || !fs::is_regular_file(cache))
		{
			f.open(cache, std::ios::out);
			f.close();
		}

		f.open(cache, std::ios::in);
		for (; nreal < 20 && !f.eof(); nreal++){
			std::getline(f, s);
			projects[nreal] = s;
		}
	}
private:
	std::array<std::filesystem::path, 20> projects;
	size_t nreal = 0;
};
