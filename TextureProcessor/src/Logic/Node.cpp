/**
 * @file Node.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief Node model logic
 * Gathered from Veritas engine WinD3D project
 * https://github.com/Agrael1/VeritasD3D
 */


#include <Logic/Source.h>
#include <Logic/Sink.h>
#include <Logic/Node.h>
#include <Logic/ShaderProbe.h>
#include <utils/Exception.h>

using namespace ver;

ver::Node::~Node() {}

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

void Node::RegisterSink(std::unique_ptr<Sink> in)
{
	if (!ValidateSink(*in))
		throw RGC_EXCEPTION(std::format("Registered sink overlaps with existing: {}", in->GetRegisteredName()));
	sinks.push_back(std::move(in));
}
void Node::RegisterSource(std::unique_ptr<Source> in)
{
	if (!ValidateSource(*in))
		throw RGC_EXCEPTION(std::format("Registered source overlaps with existing: {}", in->GetName()));
	sources.push_back(std::move(in));
}

bool ver::Node::ValidateSink(Sink& in)
{
	for (auto& si : sinks)
		if (si->GetRegisteredName() == in.GetRegisteredName())
			return false;
	return true;
}
bool ver::Node::ValidateSource(Source& in)
{
	// check for overlap of output names
	for (auto& src : sources)
		if (src->GetName() == in.GetName())
			return false;
	return true;
}

void Node::SetSinkLinkage(std::string_view registeredName, std::string_view to_node, std::string_view source)
{
	GetSink(registeredName).SetTarget(to_node, source);
}
void ver::Node::SetSinkLinkage(size_t index, std::string_view to_node, std::string_view source)
{
	GetSink(index).SetTarget(to_node, source);
}

void ver::Node::Accept(ver::ShaderProbe& probe)
{
	for (auto& i : sinks)
	{
		if (!i)continue;
		probe.ReadNode(i->GetOutputNodeName().data());
	}
}
