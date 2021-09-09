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
			SceneDock(XProperties& props) :scene(nullptr, props, { 128,128 }), view(&scene) { setWidget(&view); }
			FlowScene scene;
			FlowView view;
		};
		struct EditorDock : public QDockWidget
		{
			EditorDock() { setWidget(&edit); }
			Editor edit;
		};

	public:
		EditorTab(XProperties& props, std::filesystem::path&& p): Tab(std::move(p)), scene(props)
		{
			cw.addDockWidget(Qt::TopDockWidgetArea, &edit);
			cw.addDockWidget(Qt::BottomDockWidgetArea, &scene);
		}
		EditorTab(XProperties& props): scene(props)
		{
			cw.addDockWidget(Qt::TopDockWidgetArea, &edit);
			cw.addDockWidget(Qt::BottomDockWidgetArea, &scene);
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
	private:
		QMainWindow cw;
		SceneDock scene;
		EditorDock edit;
	};
}
