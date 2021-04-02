#pragma once
#include <memory>
#include <string>

//class Node;

class Source
{
public:
	enum class Type
	{
		Grayscale
	};
public:
	std::string_view GetName()const noexcept;
	virtual ~Source() = default;
	virtual std::string_view YieldShader();
protected:
	Source(std::string_view name, Type ty);
private:
	std::string name;
	Type type;
	//std::vector<Node*> connections; //tmp update
};