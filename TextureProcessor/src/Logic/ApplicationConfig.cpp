#include <Logic/ApplicationConfig.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

ApplicationConfig::ApplicationConfig()
{
	fs::path cache{ filename.c_str() };
	std::fstream f;
	std::string s;

	if (!fs::exists(cache) || !fs::is_regular_file(cache))
	{
		SetProjFolder(Defaults::defprojdir);
		return;
	}

	f.open(cache, std::ios::in);
	std::getline(f, s);
	if (s.empty() || fs::exists(s) || !fs::is_directory(s))
	{
		SetProjFolder(Defaults::defprojdir);
		return;
	}
	SetProjFolder(s);
}

void ApplicationConfig::Write()
{
	fs::path cache{ filename.c_str() };
	std::fstream f;
	f.open(cache, std::ios::out);
	f << last_proj_folder << std::endl;
	f.close();
}

void ApplicationConfig::SetProjFolder(std::string_view f)
{
	last_proj_folder = f;
	fs::path p(f);
	if (!fs::exists(p) || !fs::is_directory(p))
		fs::create_directories(p);
}
