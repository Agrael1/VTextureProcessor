/**
 * @file Source.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief Source connector logic
 * Gathered from Veritas engine WinD3D project
 * https://github.com/Agrael1/VeritasD3D
 */
#include <Logic/Source.h>
#include <utils/Exception.h>
#include <algorithm>

Source::Source(std::string_view name, PortType ty) :name(name), type(ty)
{
	if (name.empty())
	{
		throw RGC_EXCEPTION("Empty source name");
	}
	const bool nameCharsValid = std::all_of(name.begin(), name.end(), [](char c) {
		return std::isalnum(c) || c == '_';
		});
	if (!nameCharsValid || std::isdigit(name.front()))
	{
		throw RGC_EXCEPTION(std::format("Invalid source name: {}", name));
	}
	if (!any(ty))
	{
		throw RGC_EXCEPTION(std::format("Invalid type: {}", uint8_t(ty)));
	}
}

std::string_view Source::GetName() const noexcept
{
	return name;
}
std::string_view Source::YieldShader()
{
	throw RGC_EXCEPTION("Source cannot be accessed as shader");
}
std::shared_ptr<QImage> Source::YieldTexture()
{
	throw RGC_EXCEPTION("Source cannot be accessed as texture");
}

