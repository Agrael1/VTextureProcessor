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
	void SetCBufInfo(std::unordered_set<std::wstring> xcbuf);
	void SetInfo(
		std::unordered_set<std::wstring> types, 
		std::unordered_set<std::wstring> macros,  
		std::unordered_map<std::wstring, size_t> xfuncs);
private:
	void Parse(std::wstring_view part, size_t offset = 0);
private:
	std::array<QTextCharFormat, ver::detail::Format::size> formats;

	std::unordered_set<std::wstring> types;
	std::unordered_set<std::wstring> macros;
	std::unordered_set<std::wstring> cbuf;
	std::unordered_map<std::wstring, size_t> funcs;
};