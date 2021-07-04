#pragma once
#include <UI/IEditable.h>
#include <filesystem>

class Tab : public IEditable
{
public:
	Tab() = default;
	Tab(std::filesystem::path&& p):save_path(std::move(p)){}
public:
	void SetPath(const std::filesystem::path& p)
	{
		save_path = p;
	}
	bool IsTemporary()const noexcept
	{
		return save_path.empty();
	}
	auto& Path()const noexcept
	{
		return save_path;
	}
private:
	std::filesystem::path save_path;
};