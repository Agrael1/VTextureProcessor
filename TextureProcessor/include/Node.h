#pragma once
#include <Source.h>
#include <Sink.h>
#include <vector>
#include <memory>
#include <span>



namespace ver
{
	class Node
	{
	public:
		Node() = default;
		Node(std::string_view name);
		Node(std::string&& name);
		virtual ~Node() = default;
	public:
		std::span<const std::unique_ptr<Source>> GetSources()const noexcept
		{
			return sources;
		}
		std::span<const std::unique_ptr<Sink>> GetSinks()const noexcept
		{
			return sinks;
		}
		void SetUniqueName(std::string_view xname) { name = xname; }
		std::string_view GetName() const noexcept;
		Source& GetSource(std::string_view registeredName);
		Sink& GetSink(std::string_view registeredName);
		Source& GetSource(size_t index);
		Sink& GetSink(size_t index);
		size_t SourcesCount()const noexcept { return sources.size(); }
		size_t SinksCount()const noexcept { return sinks.size(); }

		void RegisterSink(std::unique_ptr<Sink> in);
		void RegisterSource(std::unique_ptr<Source> in);

		void SetSinkLinkage(std::string_view registeredName, const std::string& target);
		//virtual void Update()noexcept = 0;
	protected:
		std::vector<std::unique_ptr<Sink>> sinks;
		std::vector<std::unique_ptr<Source>> sources;
		std::string_view name;
	};

}
