#pragma once
#include <Editor/Editor.h>
#include <UI/FlowScene.h>
#include <UI/FlowView.h>
#include <UI/Node.h>
#include <Logic/ShaderNode.h>
#include <QDockWidget>
#include <Windows/Tab.h>
#include <Windows/TableProp.h>

namespace UI::Windows
{
	class EditorTab : public Tab, public QMainWindow
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
		EditorTab(QWidget* tab_relay, Properties& props, std::filesystem::path&& p)
			: Tab(std::move(p)), scene(props), tp(tab_relay)
		{
			Init(props);
		}
		EditorTab(QWidget* tab_relay, Properties& props) : scene(props), tp(tab_relay)
		{
			Init(props);
		}
	public:
		QWidget* Widget() noexcept override
		{
			return this;
		}
		void Save() override {};
		void SaveAs() override {};
		void Clear() override {};
		void Load() override;
		void OnEnter()noexcept override;
		void OnLeave()noexcept override;
	private:
		void Init(Properties& props)noexcept;
		bool event(QEvent* e)override;
	private:
		TableProperties tp;
		SceneDock scene;
		EditorDock edit;
		//UI::XNode<ver::ShaderNode> edited;
	};
}
