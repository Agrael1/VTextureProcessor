#pragma once
#include <Editor/Editor.h>
#include <UI/FlowScene.h>
#include <UI/FlowView.h>
#include <Windows/Tab.h>
#include <Windows/TableProp.h>
#include <QDockWidget>

#include <UI/DynamicNode.h>

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
		EditorTab(std::filesystem::path&& p, Properties& props)
			:Tab(std::move(p)), scene(props), tp(this), edited(Path())
		{
			Init(props);
			auto& x = edited.StyleName();
			tp.SetName(x.isEmpty()?"Node":x);
			scene.scene.addItem(&edited);
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

		void Request(UI::Request rq)override;
	private:
		void Init(Properties& props)noexcept;
		bool event(QEvent* e)override;

		void Compile();
	private:
		TableProperties tp;
		SceneDock scene;
		EditorDock edit;
		UI::DynamicNode edited;
	};
}
