#pragma once
#include <Source.h>
#include <Sink.h>
#include <vector>
#include <memory>

namespace ver
{
	class Node
	{
	public:
		Node(std::string_view name);
		explicit Node(std::string&& name);
		virtual ~Node() = default;
	public:
		std::string_view GetName() const noexcept;
		Source& GetSource(std::string_view registeredName);
		Sink& GetSink(std::string_view registeredName);
		Source& GetSource(size_t index);
		Sink& GetSink(size_t index);

		void RegisterSink(std::unique_ptr<Sink> in);
		void RegisterSource(std::unique_ptr<Source> in);

		void SetSinkLinkage(std::string_view registeredName, const std::string& target);
		//virtual void Update()noexcept = 0;
	private:
		std::vector<std::unique_ptr<Sink>> sinks;
		std::vector<std::unique_ptr<Source>> sources;
		std::string name;
	};

}
