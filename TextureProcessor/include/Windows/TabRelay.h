#pragma once
#include <QTabWidget>
#include <Windows/Tab.h>
#include <unordered_map>

namespace UI::Windows
{
	class SceneTab;
	class TabRelay : public QTabWidget
	{
	public:
		TabRelay(QWidget* parent);
		~TabRelay();
	public:
		template<class T, class... Args> requires std::derived_from<T, Tab>
		void LoadTab(std::filesystem::path path, QString name, Args&&... args)
		{
			namespace fs = std::filesystem;
			if (auto it = tabs.find(path.wstring()); it!=tabs.end())
				return setCurrentWidget(it->second);

			auto* xtab = new T(std::move(path), std::forward<Args>(args)...);
			xtab->SetName(name);
			xtab->Load();
			auto pname = xtab->Path().wstring();

			// Temporary
			if (pname.empty())
			{
				auto xname = UKeyTemp(name);
				addTab(xtab, xname);
			}
			else
			{
				tabs.emplace(pname, xtab);
				addTab(xtab, name);
			}
			connect(xtab, &Tab::NameChanged, [this, xtab](const QString& nam) {setTabText(indexOf(xtab), nam); });
			connect(xtab, &Tab::PathChanged, [this, xtab](const fs::path& one, const fs::path& other)
				{
					tabs.erase(one.wstring());
					tabs.emplace(other.wstring(), xtab);
					setTabToolTip(currentIndex(), QString::fromStdWString(other.wstring()));
				});
			setCurrentWidget(xtab);

			setTabToolTip(currentIndex(), QString::fromStdWString(pname));
		}
		void OnCurrentChanged(int index = 0);
		void OnTabCreated();
		void RequestActive(Request rq)
		{
			auto* tab = GetCurrent();
			if (tab)tab->Request(rq);
		}
	private:
		Tab* GetCurrent();
		QString UKeyTemp(QStringView pattern);
	private:
		std::unordered_map<std::filesystem::path::string_type, Tab*> tabs;
		size_t temp_index = 0u;
		Tab* prev_tab = nullptr;
	};
}

