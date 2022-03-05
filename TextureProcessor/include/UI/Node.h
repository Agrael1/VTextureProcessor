#pragma once
#include <UI/GraphicsLayout.h>
#include <UI/NodeModules.h>
#include <UI/Port.h>
#include <Interfaces/INode.h>
#include <Logic/Node.h>
#include <UI/PropertyHandler.h>
#include <UI/PropertyGenerator.h>


namespace UI
{
	class Sink;
	class Source;

	class NodeUI : public INode
	{
	public:
		NodeUI(QJsonObject document, std::string_view name);
		NodeUI(const NodeUI& in);
		~NodeUI();
	public:
		virtual void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;
		void DrawBackground(QPainter* painter);
		virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value)override;
	protected:
		void Init();
		void ConstructModules();
		void UpdateLayouts();
		QLabel& Header();
	protected:
		const ver::Node& GetModel()const noexcept
		{
			return const_cast<ver::Node&>(const_cast<NodeUI*>(this)->GetModel());
		}
		virtual ver::Node& GetModel() noexcept = 0;
	public:
		virtual QJsonObject Serialize()override;
		virtual void Deserialize(QJsonObject in)override;

		virtual std::string_view Name()const override;
		virtual void Update()override;
		virtual std::string Export()override;
		virtual void ExportSilent(std::string_view in)override;
		virtual void StartConnection(uint8_t index)override;
		virtual void FinishConnection(uint8_t index)override;
	protected:
		std::shared_ptr<NodeStyle> style;
		GraphicsLinearLayout* l_main = nullptr;

		std::vector<Source> sources;
		std::vector<Sink> sinks;

		std::optional<QGraphicsLinearLayout> l_left;
		std::optional<QGraphicsLinearLayout> l_central;
		std::optional<QGraphicsLinearLayout> l_right;

		std::vector<Module> modules;
		std::unique_ptr<QGraphicsProxyWidget> proxy;
		bool b_destroyed = false;
	};


	template<class XModel> requires std::derived_from<XModel, ver::Node>
	class XNode : public NodeUI
	{
	public:
		template<class ...Args>
		XNode(QJsonObject document, std::string_view name, Args&&... e)
			:NodeUI(document, name), model(document, std::forward<Args>(e)...) {}
		XNode(const XNode& in)
			:NodeUI(in), model(in.model)
		{
			for (uint8_t i = 0; i < in.model.SinksCount(); i++)
				sinks.emplace_back(*this, i, model.GetSink(i));
			for (uint8_t i = 0; i < in.model.SourcesCount(); i++)
				sources.emplace_back(*this, i, model.GetSource(i));

			Init();
			UpdateLayouts();
		}
	public:
		virtual std::unique_ptr<INode> Clone(std::string&& name)const override
		{
			printf("copy called\n");
			auto x = std::make_unique<XNode<XModel>>(*this);
			x->model.SetUniqueName(name);
			return x;
		}
		virtual ver::Node& GetModel() noexcept override
		{
			return model;
		}
		virtual void UpdateProperties(Windows::PropertyElement& properties) override
		{
			if constexpr (has_property_handler<XModel>)
				return XModel::property_handler::ConstructProperties(properties, model);
			PlaceProperties(properties, model);
		}
	protected:
		XModel model;
	};
}

