#pragma once
#include <QPlainTextEdit>


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
	void wheelEvent(QWheelEvent* event)override;
private:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightLine();
	void updateLineNumberArea(const QRect& rect, int dy);

private:
	LineNumberArea line_number_area;
};