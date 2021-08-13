/**
 * @file Node.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Node model logic
 * Gathered from Veritas engine WinD3D project
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once
#include <Logic/Source.h>
#include <Logic/Sink.h>
#include <Logic/PropertyView.h>
#include <vector>
#include <span>



namespace ver
{
	class Node
	{
	public:
		Node() = default;
		virtual ~Node() = default;
	public:
		std::span<const std::unique_ptr<Source>> GetSources()const noexcept	{return sources;}
		std::span<const std::unique_ptr<Sink>> GetSinks()const noexcept	{return sinks;}

		void SetUniqueName(std::string_view xname) { name = xname; }
		void SetUniqueName(std::string&& xname) { name = std::move(xname); }
		std::string_view GetName() const noexcept;

		Source& GetSource(std::string_view registeredName);
		Sink& GetSink(std::string_view registeredName);

		Source& GetSource(size_t index);
		Sink& GetSink(size_t index);

		size_t SourcesCount()const noexcept { return sources.size(); }
		size_t SinksCount()const noexcept { return sinks.size(); }

		void RegisterSink(std::unique_ptr<Sink>&& in);
		void RegisterSource(std::unique_ptr<Source>&& in);

		void SetSinkLinkage(std::string_view registeredName, std::string_view to_node, std::string_view source);
		void SetSinkLinkage(size_t index, std::string_view to_node, std::string_view source);
		virtual PropertyView GetProperties() = 0;
	protected:
		std::vector<std::unique_ptr<Sink>> sinks;
		std::vector<std::unique_ptr<Source>> sources;
		std::string name;
	};

}
