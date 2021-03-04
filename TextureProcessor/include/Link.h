#pragma once
#include <Node.h>

class Link 
{
public:
	Link(Node& from, Node& to){}//TODO: link eval
public:
	constexpr operator bool()noexcept
	{
		return link.second != nullptr;
	}
private:
	std::pair<Node*, Node*> link;
};