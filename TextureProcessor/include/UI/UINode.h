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
		void MakeHeader();
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
		QGraphicsLinearLayout l_left;
		QGraphicsLinearLayout l_central;
		QGraphicsLinearLayout l_right;

		std::vector<Module> modules;
		std::unique_ptr<QGraphicsProxyWidget> proxy;
		std::vector<Source> sources;
		std::vector<Sink> sinks;
		bool b_destroyed = false;
		Type type = Type::Other;
	};


	//template<class XModel> requires std::derived_from<XModel, ver::Node>
	//class XNode : public NodeUI
	//{
	//public:
	//	template<class ...Args>
	//	XNode(QJsonObject document, std::string_view name, Args&&... e)
	//		:NodeUI(document, name), model(document, std::forward<Args>(e)...) {}
	//	explicit XNode(std::pair<QJsonObject, std::string> pair)
	//		: NodeUI(pair.first, pair.second), model(pair.first) {}
	//	XNode(const XNode& in)
	//		:NodeUI(in), model(in.model)
	//	{
	//		Init();
	//		UpdateLayouts();
	//	}
	//public:
	//	virtual std::unique_ptr<INode> Clone(std::string&& name)const override
	//	{
	//		printf("copy called\n");
	//		auto x = std::make_unique<XNode<XModel>>(*this);
	//		x->model.SetUniqueName(name);
	//		return x;
	//	}
	//	virtual ver::Node& GetModel() noexcept override
	//	{
	//		return model;
	//	}
	//	virtual void UpdateProperties(Windows::PropertyElement& properties) override
	//	{
	//		if constexpr (has_property_handler<XModel>)
	//			return XModel::property_handler::ConstructProperties(properties, model);
	//		PlaceProperties(properties, model);
	//	}
	//protected:
	//	XModel model;
	//};
}

