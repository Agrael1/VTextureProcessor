/**
 * @file Sink.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Sink connector logic
 * Modified from Veritas engine WinD3D project (file Sink.*)
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once
#include <Logic/PortType.h>
#include <string>

class Source;

class Sink
{
public:
	auto GetType()const noexcept { return type; }
	std::string_view GetRegisteredName()const noexcept;
	std::string_view GetOutputNodeName()const noexcept;
	std::string_view GetSourceName()const noexcept;
	void SetTarget(std::string_view nodeName, std::string_view sourceName);
	virtual void Bind(Source& source) = 0;
	virtual void PostLinkValidate()const = 0;
	virtual void Unlink() = 0;
	virtual ~Sink() = default;
protected:
	Sink(std::string_view registeredName, PortType type);
private:
	std::string registeredName;	//sink name
	std::string nodeName;		//from which node input comes
	std::string sourceName;		//which source it is
	PortType type;
};