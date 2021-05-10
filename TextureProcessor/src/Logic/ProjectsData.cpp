#include <Logic/ProjectsData.h>

void ProjectsData::MakeCache()
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
	for (; nreal < 20 && !f.eof(); nreal++) {
		std::getline(f, s);
		if (s.empty()) return;
		projects[nreal] = s;
	}
}