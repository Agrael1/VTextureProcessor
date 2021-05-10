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
	bool InCache(std::filesystem::path projName) {
		namespace fs = std::filesystem;

		for (int i = 0; i < 20; i++) {
			if (fs::absolute(projName) == fs::absolute(projects[i])) {
				return true;
			}
		}
		return false;
	}
	void AppendCache(std::filesystem::path projName) {
		namespace fs = std::filesystem;

		if (nreal < 20) {
			projects[nreal++] = fs::absolute(projName);
		}

		fs::path cache{ "cache.vtxc" };
		std::fstream f;
		f.open(cache, std::ios::out);
		for (int i = 0; i < nreal; i++) {
			f << fs::absolute(projects[i]).string().c_str() << std::endl;
		}
		f.close();
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
			if (s.empty()) return;
			projects[nreal] = s;
		}
	}
private:
	std::array<std::filesystem::path, 20> projects;
	size_t nreal = 0;
};
