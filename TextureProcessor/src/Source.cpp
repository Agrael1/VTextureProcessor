#include <Source.h>
#include <utils/Exception.h>

Source::Source(std::string_view name) :name(name) 
{
	if (name.empty())
	{
		throw RGC_EXCEPTION("Empty output name");
	}
	const bool nameCharsValid = std::all_of(name.begin(), name.end(), [](char c) {
		return std::isalnum(c) || c == '_';
		});
	if (!nameCharsValid || std::isdigit(name.front()))
	{
		throw RGC_EXCEPTION(fmt::sprintf("Invalid output name: %s", name));
	}
}

std::string_view Source::GetName() const noexcept
{
	return name;
}


