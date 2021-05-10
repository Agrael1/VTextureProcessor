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
		return std::find(projects.rbegin(), projects.rend(), std::filesystem::absolute(projName))!= projects.rend();
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
	void MakeCache();
	void Trim()
	{
		if (nreal != 20)return;
		for (size_t i = 1; auto& x : projects)
			if (i > 0)x = std::move(projects[i++]);
	}
private:
	std::array<std::filesystem::path, 20> projects;
	size_t nreal = 0;
};
