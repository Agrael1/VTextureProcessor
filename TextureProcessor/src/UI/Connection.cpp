/**
 * @file Connection.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Class for managing connections between Nodes
 */
#include <UI/Connection.h>
#include <UI/Port.h>
#include <Interfaces/INode.h>

#include <Logic/Sink.h>

#include <QPainter>
#include <QJsonArray>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include <ranges>
#include <memory_resource>
#include <unordered_set>

using namespace UI;

namespace UI
{
	class Connection : public QGraphicsItem, public IConnection
	{
		friend class ConnectionMap;
	private:
		Connection(Port& port);
	public:
		~Connection();
	public:
		void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;
		QRectF boundingRect()const override;
	public:
		void Move(QPointF deltapos, ver::PortSide ty)override;
		void ResetSink()
		{
			connector.second = nullptr;
			bFinished = false;
		}
		void Update();

		virtual void UpdatePosition()override
		{
			prepareGeometryChange();
			sink = rpSink()->CenterScene();
			source = rpSource()->CenterScene();
		}
		virtual void Serialize(QJsonObject& doc)override;
		virtual bool Deserialize(QJsonObject)override { return true; }
		void RemoveForce()noexcept;

	private:
		void Init();
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
		void PlaceConnection(Port& port);
		bool HasCycle(INode* with)const noexcept;

		ver::PortSide Requires()const;

		Port*& rpSink() { return connector.second; }
		Port*& rpSource() { return connector.first; }
		INode& SinkNode();
		INode& SourceNode();
		const INode& SinkNode()const;
		const INode& SourceNode()const;
		void Connect();
		void Disconnect();

		std::pair<QPointF, QPointF> PointsC1C2()const;
	private:
		std::pair<Port*/*source*/, Port*/*sink*/> connector{};
		bool bFinished = false;
		QPointF source;
		QPointF sink;
	};

	IConnection& Query(Connection* in)
	{
		return *in;
	}
}


/**
 * @brief Statically casts a unique pointer
 *
 * @tparam TO Type of the returned pointer
 * @tparam FROM Type of the input pointer
 * @param old Input pointer
 * @return std::unique_ptr<TO> Returned pointer
 */
template<typename TO, typename FROM>
std::unique_ptr<TO> static_unique_pointer_cast(std::unique_ptr<FROM>&& old) {
	return std::unique_ptr<TO>{static_cast<TO*>(old.release())};
}

static UI::Port* LocatePort(QGraphicsScene& scene, QPointF pos)noexcept
{
	auto xitems = scene.items(pos);
	auto node = [](QGraphicsItem* item)
	{
		return (dynamic_cast<UI::Port*>(item) != nullptr);
	};

	for (auto* i : xitems | std::views::filter(node))
		return static_cast<UI::Port*>(i);
	return nullptr;
}

UI::Connection::Connection(Port& port)
{
	// Renders the connection between nodes
	port.scene()->addItem(this);
	// Gets coordinates of the target port
	sink = source = port.CenterScene();
	switch (port.GetType())
	{
	case ver::PortSide::Sink:
		// If this node is the connection Sink
		rpSink() = &port;
		break;
	case ver::PortSide::Source:
		// If this node is the connection Source
		rpSource() = &port;
		break;
	default:
		break;
	}
	Init();
}

UI::Connection::~Connection()
{
	// Only unmaps if the nodes were correctly connected
	if (bFinished)
		ConnectionMap::Unmap(SourceNode(), *this);
}

void UI::Connection::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
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
QRectF UI::Connection::boundingRect() const
{
	auto points = PointsC1C2();
	QRectF c1c2Rect = QRectF(points.first, points.second).normalized();
	QRectF commonRect = QRectF(source, sink).normalized().united(c1c2Rect);

	constexpr QPointF cornerOffset(PortStyle::diameter, PortStyle::diameter);

	commonRect.setTopLeft(commonRect.topLeft() - cornerOffset);
	commonRect.setBottomRight(commonRect.bottomRight() + 2 * cornerOffset);
	return commonRect;
}
void UI::Connection::Move(QPointF deltapos, ver::PortSide ty)
{
	prepareGeometryChange();
	switch (ty)
	{
	case ver::PortSide::Sink:
		sink += deltapos;
		break;
	case ver::PortSide::Source:
		source += deltapos;
		break;
	default:
		break;
	}
}

void UI::Connection::Update()
{
	auto& node = SinkNode();
	node.Update();
}

void UI::Connection::Serialize(QJsonObject& doc)
{
	/*
	The returned JSON has the following structure:

	{
		"Source": ["Node_name", source_index],
		"Sink": ["Node_name", sink_index]
	}
	*/

	QJsonArray source;
	source.append(SourceNode().Name().data());
	source.append(rpSource()->Index());
	doc.insert("Source", source);

	QJsonArray sink;
	sink.append(SinkNode().Name().data());
	sink.append(rpSink()->Index());
	doc.insert("Sink", sink);
}

void UI::Connection::Init()
{
	// Set Qt properties of the connection
	setFlag(QGraphicsItem::ItemIsMovable, false);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);

	setAcceptHoverEvents(true);
	setZValue(-1.0);
}

void UI::Connection::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	prepareGeometryChange();

	// Endpoint only moved if the position has changed
	if (auto x = Requires(); any(x))
		Move(event->pos() - event->lastPos(), x);

	update();
	event->accept();
}
void UI::Connection::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	ungrabMouse();
	event->accept();

	auto* port = LocatePort(*scene(), event->scenePos());

	// Finishes connection if mouse was released on a target
	if (port && !HasCycle(&port->Node()) && port->GetType() == Requires())
		return PlaceConnection(*port);

	if (any(Requires()))ConnectionMap::ClearTemporary();
}

void UI::Connection::PlaceConnection(Port& port)
{
	prepareGeometryChange();

	// Extrapolates missing Sink or Source
	switch (Requires())
	{
	case ver::PortSide::Sink:
	{
		rpSink() = &port;
		static_cast<Sink&>(port).connection = std::move(ConnectionMap::DetachTemporary());
		sink = port.CenterScene();
		break;
	}
	case ver::PortSide::Source:
		rpSource() = &port;
		static_cast<Sink&>(*rpSink()).connection = std::move(ConnectionMap::DetachTemporary());
		source = port.CenterScene();
		break;
	}
	ConnectionMap::Map(SourceNode(), *this);
	Connect();

	// Set connection as properly terminated (for cleanup)
	bFinished = true;
}
bool UI::Connection::HasCycle(INode* with) const noexcept
{
	std::array<INode*, 128> place;
	std::array<std::byte, 1024> place2;
	std::pmr::monotonic_buffer_resource rsrc{ place.data(), place.size()*sizeof(INode*) };
	std::pmr::monotonic_buffer_resource rsrc2{ place2.data(), place2.size() };
	std::pmr::vector<const INode*> nodes{&rsrc};
	std::pmr::unordered_set<const INode*> visited(&rsrc2);

	auto trans = [this](Connection* in)->const INode& {return in->SinkNode(); };

	const INode* root = nullptr;
	const INode* searched = nullptr;
	if (Requires() == ver::PortSide::Sink){
		root = with;
		searched = &SourceNode();
	}
	else {
		root = &SinkNode();
		searched = with;
	}
	if (root == searched)return true;

	nodes.push_back(root);

	while (!nodes.empty())
	{
		auto* node = nodes.back();
		nodes.pop_back();

		if (!visited.contains(node)){
			if (node == searched)return true;
			visited.emplace(node);
		}
		for (auto& x : ConnectionMap::Get(*node) | std::views::transform(trans))
			if (!visited.contains(&x))
				nodes.push_back(&x);
	}
	return false;
}
void UI::Connection::RemoveForce() noexcept
{
	if (bFinished)
	{
		Disconnect();
		static_cast<Sink&>(*rpSink()).connection = nullptr;
	}
}

std::pair<QPointF, QPointF> UI::Connection::PointsC1C2() const
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

ver::PortSide UI::Connection::Requires() const
{
	if (!connector.first) return ver::PortSide::Source;
	if (!connector.second) return ver::PortSide::Sink;
	return ver::PortSide::None;
}
const INode& UI::Connection::SinkNode()const
{
	return const_cast<Connection*>(this)->SinkNode();
}
const INode& UI::Connection::SourceNode()const 
{
	return const_cast<Connection*>(this)->SourceNode();
}
INode& UI::Connection::SinkNode()
{
	return rpSink()->Node();
}
INode& UI::Connection::SourceNode()
{
	return rpSource()->Node();
}
void UI::Connection::Connect()
{
	auto& xsink = static_cast<Sink&>(*rpSink());
	xsink.model.Link(static_cast<Source&>(*rpSource()).model);
	Update();
}
void UI::Connection::Disconnect()
{
	auto& xsink = static_cast<Sink&>(*rpSink());
	xsink.model.Unlink();
	Update();
}



ConnectionMap& UI::ConnectionMap::Instance()
{
	static ConnectionMap mapper;
	return mapper;
}

void UI::ConnectionMap::MakeTemporary(Port& port)
{
	Instance().tmp.reset(new Connection{ port });
	Instance().tmp->grabMouse();
}
void UI::ConnectionMap::ConnectTemporary(Port& port)
{
	auto* tmp = Instance().tmp.get();
	if (!tmp)return;
	Instance().tmp->ungrabMouse();
	Instance().tmp->PlaceConnection(port);
}
void UI::ConnectionMap::AttachTemporary(std::unique_ptr<IConnection>&& in)
{
	auto& x = Instance().tmp;
	x = static_unique_pointer_cast<Connection>(std::move(in));
	Unmap(x->SourceNode(), *x);
	x->Disconnect();
	x->ResetSink();
	x->grabMouse();
}
void UI::ConnectionMap::ClearTemporary()
{
	Instance().tmp.reset();
}
std::unique_ptr<Connection> UI::ConnectionMap::DetachTemporary()
{
	return std::move(Instance().tmp);
}
void UI::ConnectionMap::UpdateGraph(INode& from)
{
	for (auto* c : Get(from))
		c->Update();
}


void UI::ConnectionMap::Map(INode& n, Connection& c)
{
	auto& i = Instance();
	i.map[&n].push_back(&c);
}
void UI::ConnectionMap::Unmap(INode& n, Connection& c)
{
	auto& x = Instance().map.at(&n);
	auto it = std::find(x.begin(), x.end(), &c);
	if (it != x.end())x.erase(it);
}
void UI::ConnectionMap::Trim(INode& n)
{
	auto x = std::move(Instance().map[&n]);
	for (auto* i : x)
		i->RemoveForce();
	Instance().map.erase(&n);
}
std::span<Connection*> UI::ConnectionMap::Get(const INode& n)
{
	return Instance().map[&n];
}

UI::ConnectionMap::~ConnectionMap()
{
}
