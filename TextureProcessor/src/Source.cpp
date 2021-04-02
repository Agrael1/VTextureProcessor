#include <Source.h>
#include <utils/Exception.h>

Source::Source(std::string_view name, Type ty) :name(name), type(ty)
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
		throw RGC_EXCEPTION(fmt::sprintf("Invalid source name: %s", name));
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
std::shared_ptr<QOpenGLTexture> Source::YieldTexture()
{
	throw RGC_EXCEPTION("Source cannot be accessed as texture");
}

