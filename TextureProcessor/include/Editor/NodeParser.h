#pragma once
#include <Editor/Lexer.h>
#include <string>
#include <unordered_set>


class NodeParser
{
public:
	bool Parse(std::wstring_view code);
	auto GetTypesInfo() { return std::move(types); }
	auto GetFuncsInfo() { return std::move(funcs); }
private:
	std::optional<token> GetTokenInternal(ver::generator<token>& gen);
	void TryParseFunction(ver::generator<token>& gen);
private:
	std::unordered_set<std::wstring> types;
	std::unordered_map<std::wstring, size_t> funcs;
	std::vector<token> tokens;
};
