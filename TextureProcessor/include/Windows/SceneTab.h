#pragma once
#include <Windows/Tab.h>
#include <UI/FlowView.h>
#include <UI/FlowScene.h>

namespace UI::Windows
{
	class SceneTab : public Tab
	{
	public:
		SceneTab(Properties& props, std::filesystem::path&& xproj_path, QSize resolution);
		~SceneTab();
	public:
		QWidget* Widget()noexcept override
		{
			return &view;
		}
		void Save()override;
		void SaveAs()override;
		void Clear()override;
		void Load()override;
		void OnEnter()noexcept override;

		void DeleteSelected()
		{
			scene.DeleteSelected();
		}
		void ClearSelection()
		{
			scene.clearSelection();
		}
		void Export()
		{
			scene.ExportAll();
		}
	private:
		FlowScene scene;
		FlowView view;
	};
}
