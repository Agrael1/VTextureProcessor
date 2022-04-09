#pragma once
#include <Windows/Tab.h>
#include <UI/FlowView.h>
#include <UI/FlowScene.h>

namespace UI::Windows
{
	class SceneTab : public Tab
	{
	public:
		SceneTab(std::filesystem::path&& p, Properties& props, QSize resolution);
		~SceneTab();
	public:
		QWidget* Widget()noexcept override
		{
			return &view;
		}
		void Save();
		void SaveAs();
		void Clear();
		bool Load();
		void OnEnter()noexcept override;

		void Request(UI::Request rq) override;
	private:
		FlowScene scene;
		FlowView view;
	};
}
