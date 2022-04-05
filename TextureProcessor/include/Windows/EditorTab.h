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
		EditorTab(std::filesystem::path&& p, Properties& props);
		std::pair<QJsonObject, std::string> Parse(const std::filesystem::path& p);
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
		void SetCBufInfo();
		void Compile();
	private:
		TableProperties tp;
		SceneDock scene;
		EditorDock edit;
		std::optional<ver::DynamicDescriptor> tdesc;
		std::optional<UI::NodeUI> node;
	};
}
