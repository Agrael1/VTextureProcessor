#include <Sink.h>

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
}

Sink::Sink(std::string_view registeredName):registeredName(registeredName)
{

}
