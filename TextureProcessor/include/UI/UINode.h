#pragma once
#include <UI/GraphicsLayout.h>
#include <UI/NodeModules.h>
#include <UI/Port.h>
#include <Interfaces/INode.h>
#include <Logic/Node.h>
#include <UI/PropertyHandler.h>

class QGraphicsProxyWidget;
class QLabel;

namespace UI
{
	class Sink;
	class Source;

	class NodeUI : public INode
	{
	public:
		enum class Type :uint8_t
		{
			Input,
			Output,
			Other
		};
	public:
		NodeUI(std::unique_ptr<ver::Node> model);
		~NodeUI();
	public:
		virtual void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;
		void DrawBackground(QPainter* painter);
		virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value)override;
	public:
		void ReplaceModel(std::unique_ptr<ver::Node> model);
		void MakeHeader();
		void UpdateHeader();
		void MakeSinks();
		void MakeSources();
		void Init();
		void ConstructModules();
		void UpdateLayouts();
		QLabel& Header();
	public:
		const ver::Node& GetModel()const noexcept
		{
			return const_cast<ver::Node&>(const_cast<NodeUI*>(this)->GetModel());
		}
		ver::Node& GetModel() noexcept { return *model; };
		Type GetType()const noexcept { return type; }
	public:
		virtual QJsonObject Serialize()override;
		virtual void Deserialize(QJsonObject in)override;

		virtual std::string_view Name()const override;
		virtual void Update()override;
		virtual std::string Export()override;
		virtual void ExportSilent(std::string_view in)override;
		virtual void StartConnection(uint8_t index)override;
		virtual void FinishConnection(uint8_t index)override;
		virtual void UpdateProperties(Windows::PropertyElement& properties) override;
		virtual void Disconnect()override;
	protected:
		std::unique_ptr<ver::Node> model;
		GraphicsLinearLayout* l_main = nullptr;
		GraphicsLinearLayout l_left;
		GraphicsLinearLayout l_central;
		GraphicsLinearLayout l_right;

		std::vector<Module> modules;
		std::unique_ptr<QGraphicsProxyWidget> proxy;
		std::vector<Source> sources;
		std::vector<Sink> sinks;
		bool b_destroyed = false;
		Type type = Type::Other;
	};
}

