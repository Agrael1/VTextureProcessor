#pragma once
#include <string>
#include <Logic/Constants.h>

class ApplicationConfig
{
	struct Defaults
	{
		static constexpr const auto defprojdir = "projects";
	};
	static constexpr const auto filename = ver::config<"app">();
public:
	ApplicationConfig();
	~ApplicationConfig() { Write(); };
public:
	void Write();
	void SetProjFolder(std::string_view f);
	std::string_view GetProjFolder()const noexcept {
		return last_proj_folder;
	}
private:
	std::string last_proj_folder;
};