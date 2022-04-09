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
		void LoadTab(std::filesystem::path&& path, std::string_view name, Args&&... args)
		{
			auto pstr = path.string();
			if (tabs.contains(pstr))
				return setCurrentWidget(tabs.at(pstr)->Widget());

			auto rtab = std::make_unique<T>(std::move(path), std::forward<Args>(args)...);
			if (!rtab->Load())return;
			auto* w = rtab->Widget();
			addTab(w, name.data());
			setCurrentWidget(w);
			setTabToolTip(currentIndex(), pstr.c_str());
			tabs.emplace(pstr, std::move(rtab));
		}
		template<class T, class... Args> requires std::derived_from<T, Tab>
		void TempTab(std::string_view name, Args&&... args)
		{
			auto xname = UKeyTemp(name);
			auto x = temp_tabs.emplace(xname, std::make_unique<T>("", std::forward<Args>(args)...));
			auto* w = x.first->second->Widget();
			addTab(w, xname.data());
			setCurrentWidget(w);
			setTabToolTip(currentIndex(), xname.c_str());
		}
		void OnCurrentChanged(int index = 0);
		void OnTabClosed(int prev_i);
		void RequestActive(Request rq)
		{
			auto* tab = GetCurrent();
			if (tab)tab->Request(rq);
		}
	private:
		Tab* GetCurrent();
		std::string UKeyTemp(std::string_view pattern);
	private:
		std::unordered_map<std::string, std::unique_ptr<Tab>> temp_tabs;
		std::unordered_map<std::string, std::unique_ptr<Tab>> tabs;
		Tab* prev_tab = nullptr;
	};
}

