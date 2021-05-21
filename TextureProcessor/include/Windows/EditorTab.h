#pragma once
#include <Editor/Editor.h>
#include <UI/IEditable.h>

class EditorTab : public IEditable
{
public:
	EditorTab() {};
public:
	QWidget* Widget() noexcept override
	{
		return &edit;
	}
	void Save() override {};
	void SaveAs() override {};
	void Clear() override {};
	void Load() override {};
private:
	Editor edit;
};