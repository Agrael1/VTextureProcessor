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
	void Parse(std::wstring_view part);
private:
	std::array<QTextCharFormat, 3> formats;
};