/**
 * @file Connection.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Class for managing connections between Nodes
 */
#include <UI/Connection.h>
#include <UI/Port.h>
#include <UI/Interfaces/INode.h>

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
	class XConnection : public QGraphicsItem, public IXConnection
	{
		friend class XConnMapper;
	private:
		XConnection(XPort& port);
	public:
		~XConnection();
	public:
		void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;
		QRectF boundingRect()const override;
	public:
		void Move(QPointF deltapos, Port ty)override;
		void ResetSink()
		{
			connector.second = nullptr;
			bFinished = false;
		}
		void Update();

		virtual QJsonObject Serialize()override;
		virtual void Deserialize(QJsonObject)override {}
		void RemoveForce()noexcept;

	private:
		void Init();
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
		void PlaceConnection(XPort& port);
		bool HasCycle(IXNode* with)const noexcept;

		Port Requires()const;

		auto& rpSink() { return connector.second; }
		auto& rpSource() { return connector.first; }
		IXNode& SinkNode();
		IXNode& SourceNode();
		const IXNode& SinkNode()const;
		const IXNode& SourceNode()const;
		void Connect();
		void Disconnect();

		std::pair<QPointF, QPointF> PointsC1C2()const;
	private:
		std::pair<XPort*/*source*/, XPort*/*sink*/> connector{};
		bool bFinished = false;
		QPointF source;
		QPointF sink;
	};

	IXConnection& Query(XConnection* in)
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

static UI::XPort* LocatePort(QGraphicsScene& scene, QPointF pos)noexcept
{
	auto xitems = scene.items(pos);
	auto node = [](QGraphicsItem* item)
	{
		return (dynamic_cast<UI::XPort*>(item) != nullptr);
	};

	for (auto* i : xitems | std::views::filter(node))
		return static_cast<UI::XPort*>(i);
	return nullptr;
}

UI::XConnection::XConnection(XPort& port)
{
	// Renders the connection between nodes
	port.scene()->addItem(this);
	// Gets coordinates of the target port
	sink = source = port.CenterScene();
	switch (port.GetType())
	{
	case Port::Sink:
		// If this node is the connection Sink
		rpSink() = &port;
		break;
	case Port::Source:
		// If this node is the connection Source
		rpSource() = &port;
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
		XConnMapper::Unmap(SourceNode(), *this);
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
	auto& node = SinkNode();
	node.Update();
	XConnMapper::UpdateGraph(node);
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
	source.append(SourceNode().Name().data());
	source.append(rpSource()->Index());
	top.insert("Source", source);

	QJsonArray sink;

	sink.append(SinkNode().Name().data());
	sink.append(rpSink()->Index());
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

	auto* port = LocatePort(*scene(), event->scenePos());

	// Finishes connection if mouse was released on a target
	if (port && !HasCycle(&port->Node()) && port->GetType() == Requires())
		return PlaceConnection(*port);

	if (any(Requires()))XConnMapper::ClearTemporary();
}

void UI::XConnection::PlaceConnection(XPort& port)
{
	prepareGeometryChange();

	// Extrapolates missing Sink or Source
	switch (Requires())
	{
	case Port::Sink:
	{
		rpSink() = &port;
		static_cast<Sink&>(port).connection = std::move(XConnMapper::DetachTemporary());
		sink = port.CenterScene();
		break;
	}
	case Port::Source:
		rpSource() = &port;
		static_cast<Sink&>(*rpSink()).connection = std::move(XConnMapper::DetachTemporary());
		source = port.CenterScene();
		break;
	}
	XConnMapper::Map(SourceNode(), *this);
	Connect();

	// Set connection as properly terminated (for cleanup)
	bFinished = true;
}
bool UI::XConnection::HasCycle(IXNode* with) const noexcept
{
	std::array<IXNode*, 128> place;
	std::array<std::byte, 1024> place2;
	std::pmr::monotonic_buffer_resource rsrc{ place.data(), place.size()*sizeof(IXNode*) };
	std::pmr::monotonic_buffer_resource rsrc2{ place2.data(), place2.size() };
	std::pmr::vector<const IXNode*> nodes{&rsrc};
	std::pmr::unordered_set<const IXNode*> visited(&rsrc2);

	auto trans = [this](XConnection* in)->const IXNode& {return in->SinkNode(); };

	const IXNode* root = nullptr;
	const IXNode* searched = nullptr;
	if (Requires() == Port::Sink){
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
		for (auto& x : XConnMapper::Get(*node) | std::views::transform(trans))
			if (!visited.contains(&x))
				nodes.push_back(&x);
	}
	return false;
}
void UI::XConnection::RemoveForce() noexcept
{
	if (bFinished)
	{
		Disconnect();
		static_cast<Sink&>(*rpSink()).connection = nullptr;
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
const IXNode& UI::XConnection::SinkNode()const
{
	return const_cast<XConnection*>(this)->SinkNode();
}
const IXNode& UI::XConnection::SourceNode()const 
{
	return const_cast<XConnection*>(this)->SourceNode();
}
IXNode& UI::XConnection::SinkNode()
{
	return rpSink()->Node();
}
IXNode& UI::XConnection::SourceNode()
{
	return rpSource()->Node();
}
void UI::XConnection::Connect()
{
	auto& xsink = static_cast<Sink&>(*rpSink());
	xsink.model.Link(static_cast<Source&>(*rpSource()).model);
	Update();
}
void UI::XConnection::Disconnect()
{
	auto& xsink = static_cast<Sink&>(*rpSink());
	xsink.model.Unlink();
	Update();
}



XConnMapper& UI::XConnMapper::Instance()
{
	static XConnMapper mapper;
	return mapper;
}

void UI::XConnMapper::MakeTemporary(XPort& port)
{
	Instance().tmp.reset(new XConnection{ port });
	Instance().tmp->grabMouse();
}
void UI::XConnMapper::ConnectTemporary(XPort& port)
{
	Instance().tmp->ungrabMouse();
	Instance().tmp->PlaceConnection(port);
}
void UI::XConnMapper::AttachTemporary(std::unique_ptr<IXConnection>&& in)
{
	auto& x = Instance().tmp;
	x = static_unique_pointer_cast<XConnection>(std::move(in));
	Unmap(x->SourceNode(), *x);
	x->Disconnect();
	x->ResetSink();
	x->grabMouse();
}
void UI::XConnMapper::ClearTemporary()
{
	Instance().tmp.reset();
}
std::unique_ptr<XConnection> UI::XConnMapper::DetachTemporary()
{
	return std::move(Instance().tmp);
}
void UI::XConnMapper::UpdateGraph(IXNode& from)
{
	for (auto* c : Get(from))
		c->Update();
}


void UI::XConnMapper::Map(IXNode& n, XConnection& c)
{
	auto& i = Instance();
	i.map[&n].push_back(&c);
}
void UI::XConnMapper::Unmap(IXNode& n, XConnection& c)
{
	auto& x = Instance().map.at(&n);
	auto it = std::find(x.begin(), x.end(), &c);
	if (it != x.end())x.erase(it);
}
void UI::XConnMapper::Trim(IXNode& n)
{
	auto& x = Instance().map[&n];
	for (auto* i : x)
		i->RemoveForce();
	Instance().map.erase(&n);
}
std::span<XConnection*> UI::XConnMapper::Get(const IXNode& n)
{
	return Instance().map[&n];
}

UI::XConnMapper::~XConnMapper()
{
}
