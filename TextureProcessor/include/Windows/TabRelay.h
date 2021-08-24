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
		TabRelay(QWidget* parent, SceneTab*&);
	public:
		template<class T, class... Args> requires std::derived_from<T, Tab>
		T& LoadTab(const std::filesystem::path& path, std::string_view name, Args&&... args)
			{
				auto pstr = path.string();
				if (!tabs.contains(pstr))
				{
					auto x = tabs.emplace(pstr, std::make_unique<T>(std::forward<Args>(args)...));
					auto* w = x.first->second->Widget();
					addTab(w, name.data());
					setCurrentWidget(w);
					setTabToolTip(currentIndex(), pstr.c_str());
					x.first->second->Load();
					return (T&)*x.first->second;
				}
				auto& x = tabs.at(pstr);
				setCurrentWidget(x->Widget());
				return (T&)*x;
			}
		template<class T, class... Args> requires std::derived_from<T, Tab>
		T& TempTab(std::string_view name, Args&&... args)
		{
			auto xname = UKeyTemp(name);
			auto x = temp_tabs.emplace(xname, std::make_unique<T>(std::forward<Args>(args)...));
			auto* w = x.first->second->Widget();
			addTab(w, xname.data());
			setCurrentWidget(w);
			setTabToolTip(currentIndex(), xname.c_str());
			return (T&)*x.first->second;
		}
		void OnCurrentChanged(int index);
		void OnSave()
		{
			if (auto x = GetCurrent(); x)x->Save();
		}
		void OnSaveAs()
		{
			if (auto x = GetCurrent(); x)x->SaveAs();
		}
		void OnTabClosed(int prev_i)
		{
			auto key = tabToolTip(prev_i).toStdString();
			if (auto it = temp_tabs.find(key); it != temp_tabs.end())
			{
				temp_tabs.erase(it);
				return;
			}
			if (auto it = tabs.find(key); it != tabs.end())
			{
				tabs.erase(it);
				return;
			}
		}
		void Clear()noexcept
		{
			clear();
			tabs.clear();
			temp_tabs.clear();
		}
	private:
		Tab* GetCurrent();
		std::string UKeyTemp(std::string_view pattern);
	private:
		std::unordered_map<std::string, std::unique_ptr<Tab>> temp_tabs;
		std::unordered_map<std::string, std::unique_ptr<Tab>> tabs;
		SceneTab*& cur_scene;
	};
}

