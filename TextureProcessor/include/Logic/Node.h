/**
 * @file Node.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Node model logic
 * Gathered from Veritas engine WinD3D project
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once
#include <Logic/PropertyView.h>
#include <span>
#include <Interfaces/ISerialize.h>

namespace ver
{
	class Sink;
	class Source;

	class Node : public ISerialize
	{
	public:
		Node() = default;
		virtual ~Node();
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
		virtual std::string EPort() = 0;
		virtual void EPortSilent(std::string_view name) = 0;
		virtual QJsonObject Serialize()override;
		virtual void Deserialize(QJsonObject in)override {};
	protected:
		std::vector<std::unique_ptr<Sink>> sinks;
		std::vector<std::unique_ptr<Source>> sources;
		std::string name;
	};

}
