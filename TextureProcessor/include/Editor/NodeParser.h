#pragma once
#include <Editor/Lexer.h>
#include <string>
#include <unordered_set>
#include <Logic/DynamicConstant.h>


class NodeParser
{	
public:
	NodeParser(std::wstring_view code);
public:
	bool Parse();
	auto GetTypesInfo() { return std::move(types); }
	auto GetFuncsInfo() { return std::move(funcs); }
	auto GetMacrosInfo() { return std::move(macros); }
private:
	std::optional<token> GetTokenInternal();
	void TryParseFunction();
private:
	ver::generator<token> gen;
	std::unordered_set<std::wstring> types;
	std::unordered_set<std::wstring> macros;
	std::unordered_map<std::wstring, size_t> funcs;
	std::vector<token> tokens;
};
