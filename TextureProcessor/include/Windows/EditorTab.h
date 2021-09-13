#pragma once
#include <Editor/Editor.h>
#include <UI/FlowScene.h>
#include <UI/FlowView.h>
#include <QDockWidget>
#include <Windows/Tab.h>

namespace UI::Windows
{
	class EditorTab : public Tab
	{
		struct SceneDock : public QDockWidget
		{
			SceneDock(Properties& props);
			~SceneDock();
			FlowScene scene;
			FlowView view;
		};
		struct EditorDock : public QDockWidget
		{
			EditorDock() { setWidget(&edit); }
			Editor edit;
		};

	public:
		EditorTab(Properties& props, std::filesystem::path&& p): Tab(std::move(p)), scene(props)
		{
			cw.addDockWidget(Qt::LeftDockWidgetArea, &edit);
			cw.addDockWidget(Qt::RightDockWidgetArea, &scene);
		}
		EditorTab(Properties& props): scene(props)
		{
			cw.addDockWidget(Qt::LeftDockWidgetArea, &edit);
			cw.addDockWidget(Qt::RightDockWidgetArea, &scene);
		}
	public:
		QWidget* Widget() noexcept override
		{
			return &cw;
		}
		void Save() override {};
		void SaveAs() override {};
		void Clear() override {};
		void Load() override;
		void OnChange()noexcept override;
	private:
		QMainWindow cw;
		SceneDock scene;
		EditorDock edit;
	};
}
