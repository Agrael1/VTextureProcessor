#include <Node.h>
#include <utils/Exception.h>

Node::Node(std::string_view name)
	:name(name)
{}

std::string_view Node::GetName() const noexcept
{
	return name;
}

Source& Node::GetSource(std::string_view registeredName)
{
	for (auto& src : sources)
		if (src.GetName() == registeredName)
			return src;
	throw RGC_EXCEPTION(fmt::sprintf("Source named [%s] not found in node: %s", registeredName, GetName()));
}
Sink& Node::GetSink(std::string_view registeredName)
{
	for (auto& si : sinks)
		if (si.GetRegisteredName() == registeredName)
			return si;
	throw RGC_EXCEPTION(fmt::sprintf("Sink named [%s] not found in node: %s", registeredName, GetName()));
}
Source& Node::GetSource(size_t index)
{
	return sources.at(index);
}
Sink& Node::GetSink(size_t index)
{
	return sinks.at(index);
}
