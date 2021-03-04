#include <Source.h>

Source::Source(std::string_view name) :name(name) {}

std::string_view Source::GetName() const noexcept
{
	return name;
}


