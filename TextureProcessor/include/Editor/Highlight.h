#pragma once
#include <QSyntaxHighlighter>
#include <array>


class Highlighter : public QSyntaxHighlighter
{
public:
	Highlighter(QTextDocument* parent);
public:
	void highlightBlock(const QString& text)override;
private:
	std::array<QTextCharFormat, 2> formats;
};