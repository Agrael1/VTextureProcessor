#pragma once
#include <Editor/Highlight.h>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QScrollBar>


class CodeEditor : public QPlainTextEdit
{
	class LineNumberArea : public QWidget
	{
	public:
		LineNumberArea(CodeEditor* editor) : QWidget(editor), code_editor(editor)
		{}
		QSize sizeHint() const override;
	protected:
		void paintEvent(QPaintEvent* event) override;
	private:
		CodeEditor* code_editor;
	};
public:
	CodeEditor(QWidget* parent = nullptr);
public:
	void lineNumberAreaPaintEvent(QPaintEvent* event);
	int lineNumberAreaWidth();
protected:
	void resizeEvent(QResizeEvent* event) override;
private:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightLine();
	void updateLineNumberArea(const QRect& rect, int dy);

private:
	LineNumberArea line_number_area;
};



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
