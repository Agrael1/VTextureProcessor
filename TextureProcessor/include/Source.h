#pragma once
#include <memory>
#include <string>

class Source
{
public:
	std::string_view GetName()const noexcept;
	virtual ~Source() = default;
protected:
	Source(std::string_view name);
private:
	std::string name;
};