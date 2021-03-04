#pragma once
#include <Source.h>
#include <Sink.h>
#include <vector>

class Node
{
public:
	Node(std::string_view name);
	virtual ~Node() = default;
public:
	std::string_view GetName() const noexcept;
	Source& GetSource(std::string_view registeredName);
	Sink& GetSink(std::string_view registeredName);
	Source& GetSource(size_t index);
	Sink& GetSink(size_t index);
	virtual void Update()noexcept = 0;
private:
	std::vector<Sink> sinks;
	std::vector<Source> sources;
	std::string name;
};
