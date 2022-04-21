/**
 * @file ProjectsData.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Logic for projects window and persistant data writer for cache.vtxc
 */
#pragma once
#include <span>
#include <array>
#include <filesystem>

class ProjectsData
{
public:
	ProjectsData(){
		MakeCache();
	}
	~ProjectsData();
public:
	std::span<const std::filesystem::path>
		GetRecent()const noexcept
	{
		return { projects.data(), nreal };
	}
	bool contains(const std::filesystem::path& projName) {
		return std::find(projects.rbegin(), projects.rend(), std::filesystem::absolute(projName))!= projects.rend();
	}
	void AppendCache(const std::filesystem::path& projName);
	void erase(const std::filesystem::path& projName);
private:
	void MakeCache();
	void Trim()noexcept;
private:
	std::array<std::filesystem::path, 20> projects;
	size_t nreal = 0;
};
