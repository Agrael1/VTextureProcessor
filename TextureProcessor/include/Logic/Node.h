/**
 * @file Node.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Node model logic
 * Gathered from Veritas engine WinD3D project
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once
#include <Interfaces/ISerialize.h>
#include <Logic/PropertyView.h>
#include <Logic/Source.h>
#include <Logic/Sink.h>
#include <span>

namespace UI {
	class NodeStyle;
	namespace Windows {
		class PropertyElement;
	}
}
class QImage;
namespace ver
{
	class ShaderProbe;
	class Node : public ISerialize
	{
	public:
		Node() = default;
		Node(std::string name):name(std::move(name)){};
		virtual ~Node();
	public:
		std::span<const std::unique_ptr<Source>> GetSources()const noexcept	{return sources;}
		std::span<const std::unique_ptr<Sink>> GetSinks()const noexcept	{return sinks;}
		std::string_view GetName() const noexcept;

		Source& GetSource(std::string_view registeredName);
		Sink& GetSink(std::string_view registeredName);

		Source& GetSource(size_t index);
		Sink& GetSink(size_t index);

		size_t SourcesCount()const noexcept { return sources.size(); }
		size_t SinksCount()const noexcept { return sinks.size(); }

		void RegisterSink(std::unique_ptr<Sink> in);
		void RegisterSource(std::unique_ptr<Source> in);

		bool ValidateSink(Sink& in);
		bool ValidateSource(Source& in);

		void SetSinkLinkage(std::string_view registeredName, std::string_view to_node, std::string_view source);
		void SetSinkLinkage(size_t index, std::string_view to_node, std::string_view source);

		virtual UI::NodeStyle& GetStyle() = 0;
		virtual void GetProperties(UI::Windows::PropertyElement& properties) {}
		virtual std::string Export() = 0;
		virtual void Update() = 0;
		virtual void ExportSilent(std::string_view name) = 0;
		virtual std::span<std::shared_ptr<QImage>> GetLayout() = 0;
		virtual void Accept(ver::ShaderProbe& probe);


		virtual bool Deserialize(QJsonObject in)override { return true; };
	protected:
		std::vector<std::unique_ptr<Sink>> sinks;
		std::vector<std::unique_ptr<Source>> sources;
		std::string name;
	};

}
