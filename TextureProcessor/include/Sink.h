#pragma once
#include <string>

class Source;

class Sink
{
public:
	std::string_view GetRegisteredName()const noexcept;
	std::string_view GetOutputNodeName()const noexcept;
	std::string_view GetSourceName()const noexcept;
	void SetTarget(std::string_view nodeName, std::string_view sourceName);
	virtual void Bind(Source& source) = 0;
	virtual void PostLinkValidate()const = 0;
	virtual void Unlink() = 0;
	virtual ~Sink() = default;
protected:
	Sink(std::string_view registeredName);
private:
	std::string registeredName;	//sink name
	std::string nodeName;		//from which node input comes
	std::string sourceName;		//which source it is
};