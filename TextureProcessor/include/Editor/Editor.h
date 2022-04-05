#pragma once
#include <Editor/Highlight.h>
#include <Editor/CodeEditor.h>
#include <QComboBox>
#include <QBoxLayout>
#include <QScrollBar>
#include <QToolBar>


class Editor : public QWidget
{
	static constexpr size_t font_defsz = 12ull;
	static constexpr size_t font_defc = 146;
public:
	Editor();
public:
	void LoadText(const QString& in);
	void SetConstants(std::vector<QString> c);
	QString GetText()const noexcept
	{
		return code.toPlainText();
	}
	void SetCBufInfo(std::unordered_set<std::wstring> cbuf)
	{
		hl.SetCBufInfo(std::move(cbuf));
	}
	void SetInfo(
		std::unordered_set<std::wstring> types, 
		std::unordered_set<std::wstring> macros, 
		std::unordered_map<std::wstring, size_t> funcs)
	{
		hl.SetInfo(std::move(types), std::move(macros), std::move(funcs));
	}
protected:
	void wheelEvent(QWheelEvent* event);
private:
	void ParseFontSize();
	void SelectFontSize(const QString&);
private:
	QVBoxLayout vl;
	CodeEditor code;
	Highlighter hl;

	QHBoxLayout sbar;
	uint16_t font_percent = 100;
	QComboBox font_szbox;
	QRegularExpressionValidator value_range;
	QScrollBar scroll;
};
