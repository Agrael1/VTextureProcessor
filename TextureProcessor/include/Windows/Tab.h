#pragma once
#include <UI/Requests.h>
#include <filesystem>

namespace UI::Windows
{
	class Tab : public QWidget
	{
		Q_OBJECT
	public:
		Tab(std::filesystem::path p):save_path(std::move(p)){}
	public:
		void SetPath(const std::filesystem::path& p)
		{
			emit PathChanged(save_path, p);
			save_path = p;
		}
		void SetName(QString xname)
		{
			name = std::move(xname);
			emit NameChanged(name);
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
	signals:
		void NameChanged(const QString& nam);
		void PathChanged(const std::filesystem::path& prev, const std::filesystem::path& new_path);
	private:
		std::filesystem::path save_path;
		QString name;
	};
}
