#pragma once
#include <QSyntaxHighlighter>
#include <array>
#include <unordered_map>


class Highlighter : public QSyntaxHighlighter
{
public:
	enum Format
	{
		statements,
		kwords,
		comment,
		user_type,
		size
	};
public:
	Highlighter(QTextDocument* parent);
public:
	void highlightBlock(const QString& text)override;
	void SetTypeInfo(std::unordered_map<std::wstring, size_t> types);
private:
	void Parse(std::wstring_view part);
private:
	std::array<QTextCharFormat, size> formats;
	std::unordered_map<std::wstring, size_t> types;
};