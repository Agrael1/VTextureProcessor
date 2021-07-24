#pragma once
#include <Editor/Editor.h>
#include <Windows/Tab.h>

class EditorTab : public Tab
{
public:
	EditorTab() {}
	using Tab::Tab;
public:
	QWidget* Widget() noexcept override
	{
		return &edit;
	}
	void Save() override {};
	void SaveAs() override {};
	void Clear() override {};
	void Load() override;
private:
	Editor edit;
};