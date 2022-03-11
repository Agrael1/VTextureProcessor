#pragma once
#include <QSyntaxHighlighter>
#include <array>
#include <unordered_set>
#include <Editor/TypeInfo.h>


class Highlighter : public QSyntaxHighlighter
{
public:
	Highlighter(QTextDocument* parent);
public:
	void highlightBlock(const QString& text)override;
	void SetTypeInfo(std::unordered_set<std::wstring> types);
	void SetFuncInfo(std::unordered_map<std::wstring, size_t> xfuncs);
private:
	void Parse(std::wstring_view part);
private:
	std::array<QTextCharFormat, ver::detail::Format::size> formats;

	std::unordered_set<std::wstring> types;
	std::unordered_map<std::wstring, size_t> funcs;
};