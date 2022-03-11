#pragma once
#include <Interfaces/IEditable.h>
#include <UI/Requests.h>
#include <filesystem>

namespace UI::Windows
{
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
		virtual void OnEnter()noexcept
		{

		}
		virtual void OnLeave()noexcept
		{

		}
		virtual void Request(Request rq)
		{

		}
	private:
		std::filesystem::path save_path;
	};
}
