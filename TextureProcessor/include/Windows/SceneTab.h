#pragma once
#include <UI/IEditable.h>
#include <UI/FlowView.h>


class SceneTab : public IEditable
{
public:
	SceneTab(UI::Properties& props, std::filesystem::path&& xproj_path);
public:
	QWidget* Widget()noexcept override
	{
		return &view;
	}
	void Save()override;
	void SaveAs()override;
	void Clear()override;
	void Load()override;

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
	std::filesystem::path proj_path;
	UI::FlowScene scene;
	FlowView view;
};