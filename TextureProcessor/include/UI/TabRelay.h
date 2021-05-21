#pragma once
#include <QTabWidget>
#include <Windows/SceneTab.h>


class TabRelay : public QTabWidget
{
public:
	TabRelay(QWidget* parent, SceneTab*&);
public:
	template<class T, class... Args> requires std::derived_from<T, IEditable>
	T& AddTab(std::string_view name, Args&&... args)
	{
		auto& x = tabs.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		addTab(x->Widget(), name.data());
		setCurrentIndex(tabs.size() - 1);
		return (T&)*x;
	}
	void OnCurrentChanged(int index)
	{
		if (index == -1)return;
		cur_scene = dynamic_cast<SceneTab*>(tabs[index].get());
	}
	void OnSave()
	{
		auto x = currentIndex();
		if (x == -1)return;
		tabs[x]->Save();
	}
	void OnSaveAs()
	{
		auto x = currentIndex();
		if (x == -1)return;
		tabs[x]->SaveAs();
	}
	void OnTabClosed(int prev_i)
	{
		tabs.erase(tabs.begin() + prev_i);
	}
private:
	std::vector<std::unique_ptr<IEditable>> tabs;
	SceneTab*& cur_scene;
};