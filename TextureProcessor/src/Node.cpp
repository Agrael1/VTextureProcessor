#include <Node.h>
#include <utils/Exception.h>
#include <utils/Utils.h>

using namespace ver;

inline Node::Node(std::string_view name)
	:name(name)
{}
inline Node::Node(std::string&& name)
	: name(std::move(name))
{}

std::string_view Node::GetName() const noexcept
{
	return name;
}

Source& Node::GetSource(std::string_view registeredName)
{
	for (auto& src : sources)
		if (src->GetName() == registeredName)
			return *src;
	throw RGC_EXCEPTION(fmt::sprintf("Source named [%s] not found in node: %s", registeredName, GetName()));
}
Sink& Node::GetSink(std::string_view registeredName)
{
	for (auto& si : sinks)
		if (si->GetRegisteredName() == registeredName)
			return *si;
	throw RGC_EXCEPTION(fmt::sprintf("Sink named [%s] not found in node: %s", registeredName, GetName()));
}
Source& Node::GetSource(size_t index)
{
	return *sources.at(index);
}
Sink& Node::GetSink(size_t index)
{
	return *sinks.at(index);
}

void Node::RegisterSink(std::unique_ptr<Sink> in)
{
	// check for overlap of input names
	for (auto& si : sinks)
		if (si->GetRegisteredName() == in->GetRegisteredName())
			throw RGC_EXCEPTION(fmt::sprintf("Registered sink overlaps with existing: %s", in->GetRegisteredName()));

	sinks.push_back(std::move(in));
}
void Node::RegisterSource(std::unique_ptr<Source> in)
{
	// check for overlap of output names
	using namespace std::string_literals;
	for (auto& src : sources)
	{
		if (src->GetName() == in->GetName())
		{
			throw RGC_EXCEPTION(fmt::sprintf("Registered source overlaps with existing: %s", in->GetName()));
		}
	}
	sources.push_back(std::move(in));
}

void Node::SetSinkLinkage(std::string_view registeredName, const std::string& target)
{
	auto& sink = GetSink(registeredName);
	auto targetSplit = SplitString(target, ".");
	if (targetSplit.size() != 2u)
	{
		throw RGC_EXCEPTION("Input target has incorrect format");
	}
	sink.SetTarget(std::move(targetSplit[0]), std::move(targetSplit[1]));
}
