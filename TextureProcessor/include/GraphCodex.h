#pragma once
#include <unordered_map>
#include <Link.h>


class GraphCodex
{
public:
	static GraphCodex& get()
	{
		static GraphCodex lazy;
		return lazy;
	}
	Link& MakeLink(Source& from, Sink& to)
	{
		
	}
private:
	std::unordered_map<std::string_view, std::vector<Link>> codex;
};