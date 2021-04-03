#include <Sink.h>
#include <utils/Exception.h>

std::string_view Sink::GetRegisteredName() const noexcept
{
	return registeredName;
}
std::string_view Sink::GetOutputNodeName() const noexcept
{
	return nodeName;
}
std::string_view Sink::GetSourceName() const noexcept
{
	return sourceName;
}

void Sink::SetTarget(std::string_view nodeName, std::string_view sourceName)
{
	{
		if (nodeName.empty())
		{
			throw RGC_EXCEPTION("Empty output name");
		}
		const bool nameCharsValid = std::all_of(nodeName.begin(), nodeName.end(), [](char c) {
			return std::isalnum(c) || c == '_';
			});
		if (nodeName != "$" && (!nameCharsValid || std::isdigit(nodeName.front())))
		{
			throw RGC_EXCEPTION("Invalid pass name: " + registeredName);
		}
		this->nodeName = nodeName;
	}
	{
		if (sourceName.empty())
		{
			throw RGC_EXCEPTION("Empty output name");
		}
		const bool nameCharsValid = std::all_of(sourceName.begin(), sourceName.end(), [](char c) {
			return std::isalnum(c) || c == '_';
			});
		if (!nameCharsValid || std::isdigit(sourceName.front()))
		{
			throw RGC_EXCEPTION("Invalid output name: " + registeredName);
		}
		this->sourceName = sourceName;
	}
}

Sink::Sink(std::string_view registeredName, PortType type):registeredName(registeredName), type(type)
{
	if (registeredName.empty())
	{
		throw RGC_EXCEPTION("Empty output name");
	}
	const bool nameCharsValid = std::all_of(registeredName.begin(), registeredName.end(), [](char c) {
		return std::isalnum(c) || c == '_';
		});
	if (!nameCharsValid || std::isdigit(registeredName.front()))
	{
		throw RGC_EXCEPTION(fmt::sprintf("Invalid output name: %s", registeredName));
	}
}
