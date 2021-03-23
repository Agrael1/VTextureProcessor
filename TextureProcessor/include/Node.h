#pragma once
#include <Source.h>
#include <Sink.h>
#include <vector>
#include <memory>
#include <utils/polymorphic_value.h>

namespace pv = isocpp_p0201;

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
		void SetUniqueName(std::string_view xname) { name = xname; }
		std::string_view GetName() const noexcept;
		Source& GetSource(std::string_view registeredName);
		Sink& GetSink(std::string_view registeredName);
		Source& GetSource(size_t index);
		Sink& GetSink(size_t index);

		void RegisterSink(pv::polymorphic_value<Sink> in);
		void RegisterSource(pv::polymorphic_value<Source> in);

		void SetSinkLinkage(std::string_view registeredName, const std::string& target);
		//virtual void Update()noexcept = 0;
	private:
		std::vector<pv::polymorphic_value<Sink>> sinks;
		std::vector<pv::polymorphic_value<Source>> sources;
		std::string_view name;
	};

}
