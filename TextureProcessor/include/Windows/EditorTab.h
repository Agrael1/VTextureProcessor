#pragma once
#include <Editor/Editor.h>
#include <UI/FlowScene.h>
#include <UI/FlowView.h>
#include <UI/Node.h>
#include <Logic/ShaderNode.h>
#include <QDockWidget>
#include <Windows/Tab.h>
#include <Windows/TableProp.h>
#include <Windows/Properties.h>

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
			Init(props);
		}
		EditorTab(Properties& props): scene(props)
		{
			Init(props);
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
		void OnEnter()noexcept override;
		void OnLeave()noexcept override;
	private:
		void Init(Properties& props)noexcept
		{
			cw.addDockWidget(Qt::LeftDockWidgetArea, &edit);
			cw.addDockWidget(Qt::RightDockWidgetArea, &scene);
			((QMainWindow*)props.parentWidget())->addDockWidget(Qt::RightDockWidgetArea, &tp, Qt::Vertical);
			tp.hide();
		}
	private:
		TableProperties tp;
		QMainWindow cw;
		SceneDock scene;
		EditorDock edit;
		//UI::XNode<ver::ShaderNode> edited;
	};
}
