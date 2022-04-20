#include <Logic/ApplicationConfig.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

ApplicationConfig::ApplicationConfig()
{
	fs::path cache{ filename.c_str() };
	std::basic_fstream<char16_t> f;
	std::u16string s;

	if (!fs::exists(cache) || !fs::is_regular_file(cache))
	{
		SetProjFolder(Defaults::defprojdir.data());
		return;
	}

	f.open(cache, std::ios::in);
	std::getline(f, s);
	if (s.empty() || fs::exists(s) && !fs::is_directory(s))
	{
		SetProjFolder(Defaults::defprojdir.data());
		return;
	}
	SetProjFolder(std::move(s));
}

void ApplicationConfig::Write()
{
	fs::path cache{ filename.c_str() };
	std::basic_fstream<char16_t> f;
	f.open(cache, std::ios::out);
	f << last_proj_folder << '\n';
	f.close();
}

void ApplicationConfig::SetProjFolder(std::u16string f)
{
	last_proj_folder = std::move(f);
	fs::path p(last_proj_folder);
	if (!fs::exists(p) || !fs::is_directory(p))
		fs::create_directories(p);
}
