#pragma once
#include <string>
#include <unordered_set>


class NodeParser
{
public:
	bool Parse(std::wstring_view code);
	auto GetTypesInfo(){return std::move(types);}
private:

private:
	std::unordered_map<std::wstring, size_t> types;
};
