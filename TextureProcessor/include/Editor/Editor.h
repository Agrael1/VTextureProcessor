#pragma once
#include <Editor/Highlight.h>
#include <Editor/CodeEditor.h>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QBoxLayout>
#include <QScrollBar>



struct FontSizeController : public QComboBox
{
	uint16_t font_percent = 100;
	QIntValidator value_range;
};

class Editor : public QWidget
{
	static constexpr size_t font_defsz = 12ull;
	static constexpr size_t font_defc = 146;
public:
	Editor();
public:
	void LoadText(const QString& in);
protected:
	void wheelEvent(QWheelEvent* event);
private:
	void ParseFontSize();
	void SelectFontSize(const QString&);
private:
	QVBoxLayout vl;
	QTextEdit texter;
	CodeEditor code;
	Highlighter hl;

	QHBoxLayout sbar;
	uint16_t font_percent = 100;
	QComboBox font_szbox;
	QRegularExpressionValidator value_range;
	QScrollBar scroll;
};
