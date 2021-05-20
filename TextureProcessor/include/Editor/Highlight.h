#pragma once
#include <QSyntaxHighlighter>


class Highlighter : public QSyntaxHighlighter
{
public:
	Highlighter(QTextDocument* parent);
public:
	void highlightBlock(const QString& text)override;
private:
	QTextCharFormat stmt;
	QTextCharFormat kwd;
private:
	static const std::array<QRegularExpression, 16> statements;
	static const std::array<QRegularExpression, 92> kwords;
};