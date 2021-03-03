#pragma once
#include "Source.h"
#include "Sink.h"
#include <vector>

class Node
{
public:
	Node(std::string_view name);
private:
	std::vector<Sink> sinks;
	std::vector<Source> sources;
	std::string name;
};
