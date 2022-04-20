#pragma once
#include <Logic/Constants.h>
#include <utils/utils.h>

class ApplicationConfig
{
	struct Defaults
	{
		static constexpr const std::u16string_view defprojdir = u"projects";
	};
	static constexpr const auto filename = ver::config<u"app">();
public:
	ApplicationConfig();
	~ApplicationConfig() { Write(); };
public:
	void Write();
	void SetProjFolder(std::u16string f);
	std::u16string_view GetProjFolder()const noexcept {
		return last_proj_folder;
	}
	const QString GetQProjFolder()const noexcept {
		return ver::MakeConstString(last_proj_folder.c_str(), last_proj_folder.length());
	}
private:
	std::u16string last_proj_folder;
};