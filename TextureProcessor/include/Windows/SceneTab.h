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
		void Save();
		void SaveAs();
		void Clear();
		bool Load();
		void OnEnter()noexcept override;

		void Request(UI::Request rq) override;
	private:
		QVBoxLayout vl;
		FlowScene scene;
		FlowView view;
	};
}
