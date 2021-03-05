#pragma once
#include <memory>
#include <string>

//class Node;

class Source
{
public:
	std::string_view GetName()const noexcept;
	virtual ~Source() = default;
protected:
	Source(std::string_view name);
private:
	std::string name;
	//std::vector<Node*> connections; //tmp update
};