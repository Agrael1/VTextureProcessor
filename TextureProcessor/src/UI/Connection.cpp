/**
 * @file Connection.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Class for managing connections between Nodes
 */
#include <UI/Connection.h>
#include <UI/FlowScene.h>
#include <QPainter>
#include <QJsonArray>
#include <QGraphicsSceneMouseEvent>
#include <Logic/Source.h>
#include <Logic/Sink.h>
#include <Logic/Node.h>
#include <UI/NodeStyle.h>
#include <ranges>

using namespace UI;
#pragma region delet
///**
// * @brief Construct a new Connection:: Connection object
// *
// * @param node Target node object
// * @param ty Type of the target port (Sink or Source)
// * @param portidx Index of the target port
// */
//Connection::Connection(Node& node, Port ty, uint8_t portidx)
//{
//	// Renders the connection between nodes
//	node.scene()->addItem(this);
//	// Gets coordinates of the target port
//	sink = source = node.GetPortPos(ty, portidx);
//	switch (ty)
//	{
//	case Port::Sink:
//		// If this node is the connection Sink
//		connector.second = &node;
//		sinkN = portidx;
//		break;
//	case Port::Source:
//		// If this node is the connection Source
//		connector.first = &node;
//		sourceN = portidx;
//		break;
//	default:
//		break;
//	}
//	Init();
//}
//
//Connection::~Connection()
//{
//	// Only unmaps if the nodes were correctly connected
//	if (bFinished)
//		ConnMapper::Unmap(connector.first, this);
//}
//
//void Connection::Init()
//{
//	// Set Qt properties of the connection
//	setFlag(QGraphicsItem::ItemIsMovable, false);
//	setFlag(QGraphicsItem::ItemIsFocusable, true);
//	setFlag(QGraphicsItem::ItemIsSelectable, true);
//
//	setAcceptHoverEvents(true);
//	setZValue(-1.0);
//}
//
///**
// * @brief Calculates bounding rectangle for spline selection
// *
// * @return QRectF calculated bounding rectangle
// */
//QRectF UI::Connection::boundingRect() const
//{
//	auto points = PointsC1C2();
//	QRectF c1c2Rect = QRectF(points.first, points.second).normalized();
//	QRectF commonRect = QRectF(source, sink).normalized().united(c1c2Rect);
//
//	constexpr QPointF cornerOffset(PortStyle::diameter, PortStyle::diameter);
//
//	commonRect.setTopLeft(commonRect.topLeft() - cornerOffset);
//	commonRect.setBottomRight(commonRect.bottomRight() + 2 * cornerOffset);
//	return commonRect;
//}
//
///**
// * @brief Draws spline using an appropriate style and calculated pivot points
// *
// * @param painter used for drawing on the canvas
// * @param option not used (only for interface compatibility)
// * @param widget not used (only for interface compatibility)
// */
//void Connection::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
//{
//	painter->setBrush(Qt::NoBrush);
//	// Selects style based on connection status (connected, selected or currently being dragged)
//	if (isSelected())
//		painter->setPen(ConnectionStyle::Grayscale.selected);
//	else if (bFinished)
//		painter->setPen(ConnectionStyle::Grayscale.connected);
//	else
//		painter->setPen(ConnectionStyle::Grayscale.sketch);
//
//	auto c1c2 = PointsC1C2();
//	// Draws a cubic spline using the calculated pivot points
//	QPainterPath cubic(source);
//	cubic.cubicTo(c1c2.first, c1c2.second, sink);
//	painter->drawPath(cubic);
//}
//
///**
// * @brief Callback for mouse move event
// *
// * @param event Captured event
// */
//void UI::Connection::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
//{
//	prepareGeometryChange();
//
//	// Endpoint only moved if the position has changed
//	if (auto x = Requires(); any(x))
//		Move(event->pos() - event->lastPos(), x);
//
//	update();
//	event->accept();
//}
//
///**
// * @brief Handles mouse release event (performs connection if mouse released on target)
// *
// * @param event Captured mouse release event
// */
//void UI::Connection::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
//{
//	ungrabMouse();
//	event->accept();
//
//	auto& xscene = static_cast<UI::FlowScene&>(*scene());
//	auto* node = xscene.LocateNode(event->scenePos());
//
//	// Finishes connection if mouse was released on a target
//	if (node && node != StartNode())
//		if (auto p = node->PortHitScene(event->scenePos()); p)
//			return PlaceConnection(p, node);
//
//	if (any(Requires()))ConnMapper::ClearTemporary();
//}
//
///**
// * @brief Finishes a connection between two nodes
// *
// * @param port Target port
// * @param node Target Node
// */
//void UI::Connection::PlaceConnection(std::optional<std::pair<Port, uint8_t>> port, Node* node)
//{
//	auto re = Requires();
//	// If connecting Sink to Sink (or Source to Source)
//	if (port->first != re) {
//		ConnMapper::ClearTemporary();
//		return;
//	};
//	prepareGeometryChange();
//
//	// Extrapolates missing Sink or Source
//	switch (re)
//	{
//	case Port::Sink:
//	{
//		sinkN = port->second;
//		connector.second = node;
//		node->SetConnection(ConnMapper::DetachTemporary(), sinkN);
//		sink = node->GetPortPos(Port::Sink, sinkN);
//		break;
//	}
//	case Port::Source:
//		connector.first = node;
//		sourceN = port->second;
//		connector.second->SetConnection(ConnMapper::DetachTemporary(), sinkN);
//		source = node->GetPortPos(Port::Source, sourceN);
//		break;
//	}
//	ConnMapper::Map(connector.first, this);
//	connector.second->OnConnect(sinkN, *connector.first, sourceN);
//
//	// Set connection as properly terminated (for cleanup)
//	bFinished = true;
//}
//
//
///**
//* @brief Propagates changes to nodes following current Node
//*
//*/
//
//void UI::Connection::Update()
//{
//	connector.second->OnConnect(sinkN, *connector.first, sourceN);
//}
//
///**
// * @brief Propagates information about disconnection
// *
// */
//void UI::Connection::UpdateDisconnect()
//{
//	connector.second->OnDisconnect(sinkN);
//}
//
///**
// * @brief Calculates spline pivot points based on relative Node position
// *
// * @return std::pair<QPointF, QPointF> spline pivot points
// */
//std::pair<QPointF, QPointF> Connection::PointsC1C2() const
//{
//	constexpr qreal defaultOffset = 200;
//	qreal xDistance = sink.x() - source.x();
//	// Maximum offset is 200
//	qreal horizontalOffset = std::min(defaultOffset, std::abs(xDistance));
//	qreal verticalOffset = 0;
//	qreal ratioX = 0.5;
//
//	// If the Sink is left of Source
//	if (xDistance <= 0)
//	{
//		verticalOffset = qMin(defaultOffset, 20.0);
//		ratioX = 1.0;
//	}
//	horizontalOffset *= ratioX;
//
//	// Returns spline pivot points
//	return {
//		{source.x() + horizontalOffset, source.y() + verticalOffset},
//		{sink.x() - horizontalOffset, sink.y() - verticalOffset }
//	};
//}
//
///**
// * @brief Generates a JSON representation of the connection
// *
// * @return QJsonObject
// */
//QJsonObject Connection::Serialize()
//{
//	/*
//	The returned JSON has the following structure:
//
//	{
//		"Source": ["Node_name", source_index],
//		"Sink": ["Node_name", sink_index]
//	}
//	*/
//	QJsonObject top;
//
//	QJsonArray source;
//	source.append(connector.first->GetName().data());
//	source.append(sourceN);
//	top.insert("Source", source);
//
//	QJsonArray sink;
//
//	sink.append(connector.second->GetName().data());
//	sink.append(sinkN);
//	top.insert("Sink", sink);
//
//	return top;
//}
//
///**
// * @brief Checks what Port type is requred to properly finish the connection
// *
// * @return Port
// */
//Port Connection::Requires()const
//{
//	if (!connector.first) return Port::Source;
//	if (!connector.second) return Port::Sink;
//	return Port::None;
//}
//
///**
// * @brief Returns Source and Sink type (based on which end of the connection is currently connected)
// *
// * @return PortType
// */
//PortType Connection::GetType() const noexcept
//{
//	if (connector.first)
//		return connector.first->GetSourceType(sourceN);
//
//	if (connector.second)
//		return connector.first->GetSinkType(sinkN);
//
//	return PortType::None;
//}
//
///**
// * @brief Updates coordinates of a spline point
// *
// * @param deltapos Target offset
// * @param ty Port to change position of
// */
//void Connection::Move(QPointF deltapos, Port ty)
//{
//	prepareGeometryChange();
//	switch (ty)
//	{
//	case Port::Sink:
//		sink += deltapos;
//		break;
//	case Port::Source:
//		source += deltapos;
//		break;
//	default:
//		break;
//	}
//}
//
///**
// * @brief Destroys the connection
// *
// */
//void Connection::RemoveForce() noexcept
//{
//	if (bFinished)
//	{
//		UpdateDisconnect();
//		connector.second->SetConnection(nullptr, sinkN);
//	}
//}
//
///////////////////////////////////////////////////////////////////////////
//
///**
// * @brief Singleton for connection mapper
// *
// * @return ConnMapper&
// */
//ConnMapper& UI::ConnMapper::Instance()
//{
//	static ConnMapper mapper;
//	return mapper;
//}
//
///**
// * @brief Creates a new Node mapping in the connection mapper
// *
// * @param n Node that is being connected
// * @param c New connection of the Node
// */
//void ConnMapper::Map(Node* n, Connection* c)
//{
//	auto& i = Instance();
//	i.map[n].push_back(c);
//}
//
///**
// * @brief Returns all connections associated with the Node
// *
// * @param n Target Node
// * @return std::span<Connection*> all asociated connection
// */
//std::span<Connection*> UI::ConnMapper::Get(Node* n)
//{
//	return Instance().map[n];
//}
//
///**
// * @brief Creates a new temporary connection (the one being currently dragged)
// *
// * @param node Source node
// * @param port Source port
// * @param portidx Source port index
// */
//void UI::ConnMapper::MakeTemporary(Node& node, Port port, uint8_t portidx)
//{
//	Instance().tmp.reset(new Connection{ node, port, portidx });
//	Instance().tmp->grabMouse();
//}
//
///**
// * @brief Moves a connection to the temporary connection (when it is being reattached to a different node)
// *
// * @param in Target connection
// */
//void UI::ConnMapper::AttachTemporary(std::unique_ptr<Connection>&& in)
//{
//	auto& x = Instance().tmp;
//	x = std::move(in);
//	Unmap(x->connector.first, x.get());
//	x->UpdateDisconnect();
//	x->ResetSink();
//	x->grabMouse();
//}
//
///**
// * @brief Destroyes the temporary connection
// *
// */
//void UI::ConnMapper::ClearTemporary()
//{
//	Instance().tmp->ungrabMouse();
//	Instance().tmp.reset();
//}
#pragma endregion


static UI::IXNode* LocateNode(QGraphicsScene& scene, QPointF pos)noexcept
{
	auto xitems = scene.items(pos);
	auto node = [](QGraphicsItem* item)
	{
		return (dynamic_cast<UI::IXNode*>(item) != nullptr);
	};

	for (auto* i : xitems | std::views::filter(node))
		return static_cast<UI::IXNode*>(i);
	return nullptr;
}

UI::XConnection::XConnection(IXNode& node, Port ty, uint8_t portidx)
{
	// Renders the connection between nodes
	node.scene()->addItem(this);
	// Gets coordinates of the target port
	sink = source = node.GetPortPos(ty, portidx);
	switch (ty)
	{
	case Port::Sink:
		// If this node is the connection Sink
		connector.second = &node;
		sinkN = portidx;
		break;
	case Port::Source:
		// If this node is the connection Source
		connector.first = &node;
		sourceN = portidx;
		break;
	default:
		break;
	}
	Init();
}

UI::XConnection::~XConnection()
{
	// Only unmaps if the nodes were correctly connected
	if (bFinished)
		XConnMapper::Unmap(connector.first, this);
}

void UI::XConnection::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setBrush(Qt::NoBrush);
	// Selects style based on connection status (connected, selected or currently being dragged)
	if (isSelected())
		painter->setPen(ConnectionStyle::Grayscale.selected);
	else if (bFinished)
		painter->setPen(ConnectionStyle::Grayscale.connected);
	else
		painter->setPen(ConnectionStyle::Grayscale.sketch);

	auto c1c2 = PointsC1C2();
	// Draws a cubic spline using the calculated pivot points
	QPainterPath cubic(source);
	cubic.cubicTo(c1c2.first, c1c2.second, sink);
	painter->drawPath(cubic);
}

QRectF UI::XConnection::boundingRect() const
{
	auto points = PointsC1C2();
	QRectF c1c2Rect = QRectF(points.first, points.second).normalized();
	QRectF commonRect = QRectF(source, sink).normalized().united(c1c2Rect);

	constexpr QPointF cornerOffset(PortStyle::diameter, PortStyle::diameter);

	commonRect.setTopLeft(commonRect.topLeft() - cornerOffset);
	commonRect.setBottomRight(commonRect.bottomRight() + 2 * cornerOffset);
	return commonRect;
}

void UI::XConnection::Move(QPointF deltapos, Port ty)
{
	prepareGeometryChange();
	switch (ty)
	{
	case Port::Sink:
		sink += deltapos;
		break;
	case Port::Source:
		source += deltapos;
		break;
	default:
		break;
	}
}

void UI::XConnection::Update()
{
	connector.second->OnConnect(sinkN, *connector.first, sourceN);
}

void UI::XConnection::UpdateDisconnect()
{
	connector.second->OnDisconnect(sinkN);
}

QJsonObject UI::XConnection::Serialize()
{
	/*
	The returned JSON has the following structure:

	{
		"Source": ["Node_name", source_index],
		"Sink": ["Node_name", sink_index]
	}
	*/
	QJsonObject top;

	QJsonArray source;
	source.append(connector.first->Name().data());
	source.append(sourceN);
	top.insert("Source", source);

	QJsonArray sink;

	sink.append(connector.second->Name().data());
	sink.append(sinkN);
	top.insert("Sink", sink);

	return top;
}

void UI::XConnection::Init()
{
	// Set Qt properties of the connection
	setFlag(QGraphicsItem::ItemIsMovable, false);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);

	setAcceptHoverEvents(true);
	setZValue(-1.0);
}

void UI::XConnection::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	prepareGeometryChange();

	// Endpoint only moved if the position has changed
	if (auto x = Requires(); any(x))
		Move(event->pos() - event->lastPos(), x);

	update();
	event->accept();
}

void UI::XConnection::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	ungrabMouse();
	event->accept();

	auto* node = LocateNode(*scene(), event->scenePos());

	// Finishes connection if mouse was released on a target
	if (node && node != StartNode())
		if (auto p = node->PortHitScene(event->scenePos()); any(p.first))
			return PlaceConnection(p, node);

	if (any(Requires()))XConnMapper::ClearTemporary();
}

void UI::XConnection::PlaceConnection(std::optional<std::pair<Port, uint8_t>> port, IXNode* node)
{
	auto re = Requires();
	// If connecting Sink to Sink (or Source to Source)
	if (port->first != re) {
		XConnMapper::ClearTemporary();
		return;
	};
	prepareGeometryChange();

	// Extrapolates missing Sink or Source
	switch (re)
	{
	case Port::Sink:
	{
		sinkN = port->second;
		connector.second = node;
		node->SetConnection(XConnMapper::DetachTemporary(), sinkN);
		sink = node->GetPortPos(Port::Sink, sinkN);
		break;
	}
	case Port::Source:
		connector.first = node;
		sourceN = port->second;
		connector.second->SetConnection(XConnMapper::DetachTemporary(), sinkN);
		source = node->GetPortPos(Port::Source, sourceN);
		break;
	}
	XConnMapper::Map(connector.first, this);
	connector.second->OnConnect(sinkN, *connector.first, sourceN);

	// Set connection as properly terminated (for cleanup)
	bFinished = true;
}

void UI::XConnection::RemoveForce() noexcept
{
	if (bFinished)
	{
		UpdateDisconnect();
		connector.second->SetConnection(nullptr, sinkN);
	}
}

std::pair<QPointF, QPointF> UI::XConnection::PointsC1C2() const
{
	constexpr qreal defaultOffset = 200;
	qreal xDistance = sink.x() - source.x();
	// Maximum offset is 200
	qreal horizontalOffset = std::min(defaultOffset, std::abs(xDistance));
	qreal verticalOffset = 0;
	qreal ratioX = 0.5;

	// If the Sink is left of Source
	if (xDistance <= 0)
	{
		verticalOffset = qMin(defaultOffset, 20.0);
		ratioX = 1.0;
	}
	horizontalOffset *= ratioX;

	// Returns spline pivot points
	return {
		{source.x() + horizontalOffset, source.y() + verticalOffset},
		{sink.x() - horizontalOffset, sink.y() - verticalOffset }
	};
}

Port UI::XConnection::Requires() const
{
	if (!connector.first) return Port::Source;
	if (!connector.second) return Port::Sink;
	return Port::None;
}

PortType UI::XConnection::GetType() const noexcept
{
	if (connector.first)
		return connector.first->Model().GetSource(sourceN).GetType();

	if (connector.second)
		return connector.first->Model().GetSink(sinkN).GetType();

	return PortType::None;
}


void UI::XConnMapper::MakeTemporary(IXNode& node, Port port, uint8_t portidx)
{
	Instance().tmp.reset(new XConnection{ node, port, portidx });
	Instance().tmp->grabMouse();
}

void UI::XConnMapper::AttachTemporary(std::unique_ptr<XConnection>&& in)
{
	auto& x = Instance().tmp;
	x = std::move(in);
	Unmap(x->connector.first, x.get());
	x->UpdateDisconnect();
	x->ResetSink();
	x->grabMouse();
}

void UI::XConnMapper::ClearTemporary()
{
	//Instance().tmp->ungrabMouse();
	Instance().tmp.reset();
}

XConnMapper& UI::XConnMapper::Instance()
{
	static XConnMapper mapper;
	return mapper;
}

void UI::XConnMapper::Map(IXNode* n, XConnection* c)
{
	auto& i = Instance();
	i.map[n].push_back(c);
}

std::span<XConnection*> UI::XConnMapper::Get(IXNode* n)
{
	return Instance().map[n];
}
