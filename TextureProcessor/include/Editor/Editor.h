#pragma once
#include <Editor/Highlight.h>
#include <QTextEdit>

class Editor : public QWidget
{
public:
	Editor();

private:
	QVBoxLayout vl;
	QTextEdit texter;
	Highlighter hl;
};
