#pragma once
#include <Editor/Editor.h>
#include <UI/FlowScene.h>
#include <UI/FlowView.h>
#include <Windows/Tab.h>
#include <Windows/TableProp.h>
#include <QDockWidget>
#include <QMainWindow>

#include <UI/UINode.h>
#include <Logic/DynamicNode.h>

#include <DockManager.h>


namespace UI::Windows
{
	class EditorTab : public Tab, public QMainWindow
	{
		struct SceneDock : public ads::CDockWidget
		{
			using base_class = ads::CDockWidget;
			SceneDock(Properties& props);
			~SceneDock();
			FlowScene scene;
			FlowView view;
		};
		struct EditorDock : public ads::CDockWidget
		{
			using base_class = ads::CDockWidget;
			EditorDock();
			Editor edit;
		};
		struct ConsoleDock : public ads::CDockWidget
		{
			using base_class = ads::CDockWidget;
			ConsoleDock();
			QPlainTextEdit console;
		};
	public:
		EditorTab(std::filesystem::path&& p, Properties& props);
		~EditorTab()
		{
			docker.removeDockWidget(&scene);
			docker.removeDockWidget(&edit);
			docker.removeDockWidget(&con);
		}
		QJsonObject Parse(const std::filesystem::path& p);
	public:
		QWidget* Widget() noexcept override
		{
			return this;
		}
		void Save();
		void SaveAs();
		bool Load();
		void OnEnter()noexcept override;
		void OnLeave()noexcept override;

		void Request(UI::Request rq)override;
	private:
		void Init(Properties& props)noexcept;
		bool event(QEvent* e)override;
		void SetCBufInfo();
		void Compile();
	private:
		ads::CDockManager docker;
		TableProperties tp;
		SceneDock scene;
		EditorDock edit;
		ConsoleDock con;
		std::optional<ver::DynamicDescriptor> tdesc;
		std::optional<UI::NodeUI> node;
	};
}
