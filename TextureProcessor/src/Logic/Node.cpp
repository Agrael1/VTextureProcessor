/**
 * @file Node.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief Node model logic
 * Gathered from Veritas engine WinD3D project
 * https://github.com/Agrael1/VeritasD3D
 */

#include <Logic/Node.h>
#include <utils/Exception.h>

using namespace ver;

std::string_view Node::GetName() const noexcept
{
	return name;
}

Source& Node::GetSource(std::string_view registeredName)
{
	for (auto& src : sources)
		if (src->GetName() == registeredName)
			return *src;
	throw RGC_EXCEPTION(std::format("Source named [{}] not found in node: {}", registeredName, GetName()));
}
Sink& Node::GetSink(std::string_view registeredName)
{
	for (auto& si : sinks)
		if (si->GetRegisteredName() == registeredName)
			return *si;
	throw RGC_EXCEPTION(std::format("Sink named [{}] not found in node: {}", registeredName, GetName()));
}
Source& Node::GetSource(size_t index)
{
	return *sources.at(index);
}
Sink& Node::GetSink(size_t index)
{
	return *sinks.at(index);
}

void Node::RegisterSink(std::unique_ptr<Sink>&& in)
{
	// check for overlap of input names
	for (auto& si : sinks)
		if (si->GetRegisteredName() == in->GetRegisteredName())
			throw RGC_EXCEPTION(std::format("Registered sink overlaps with existing: {}", in->GetRegisteredName()));

	sinks.push_back(std::move(in));
}
void Node::RegisterSource(std::unique_ptr<Source>&& in)
{
	// check for overlap of output names
	using namespace std::string_literals;
	for (auto& src : sources)
	{
		if (src->GetName() == in->GetName())
		{
			throw RGC_EXCEPTION(std::format("Registered source overlaps with existing: {}", in->GetName()));
		}
	}
	sources.push_back(std::move(in));
}

void Node::SetSinkLinkage(std::string_view registeredName, std::string_view to_node, std::string_view source)
{
	GetSink(registeredName).SetTarget(to_node, source);
}
void ver::Node::SetSinkLinkage(size_t index, std::string_view to_node, std::string_view source)
{
	GetSink(index).SetTarget(to_node, source);
}
