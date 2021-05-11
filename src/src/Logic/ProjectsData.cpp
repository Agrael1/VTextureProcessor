#include <Logic/ProjectsData.h>
#include <fstream>

namespace fs = std::filesystem;

/**
 * @brief Persistent data writer
*/
ProjectsData::~ProjectsData()
{
	std::ofstream f{ "cache.vtxc" };
	for (auto& x : GetRecent())
		f << x.string() << std::endl;
}

/**
 * @brief Reads cache from file and creates it if not present
*/
void ProjectsData::MakeCache()
{
	fs::path cache{ "cache.vtxc" };
	std::fstream f;
	std::string s;

	if (!fs::exists(cache) || !fs::is_regular_file(cache))
	{
		f.open(cache, std::ios::out);
		f.close();
	}

	f.open(cache, std::ios::in);
	for (; nreal < 20 && !f.eof(); nreal++) {
		std::getline(f, s);
		if (s.empty()) return;
		projects[nreal] = s;
	}
}

/**
 * @brief Appends new path to the queue
 * @param projName 
*/
void ProjectsData::AppendCache(const std::filesystem::path& projName)
{
	Trim();
	projects[0] = fs::absolute(projName);
	if (nreal < 20)nreal++;
}

/**
 * @brief Erases given project path and shifts 
 * @param projName path to remove
*/
void ProjectsData::erase(const std::filesystem::path& projName)
{
	if (auto it = std::find(projects.rbegin(), projects.rend(), projName); it != projects.rend())
	{
		for (; it != projects.rbegin(); it--)
			*(it) = std::move(*(it - 1));
		nreal--;
	}
}

/**
 * @brief Pushes recent projects down the queue
*/
void ProjectsData::Trim()noexcept
{
	for (int i = std::min(int(nreal), 19); i > 0; i--)
		projects[i] = std::move(projects[i - 1]);
}