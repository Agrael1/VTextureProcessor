#include <UI/Node.h>
#include <UI/Connection.h>
#include <UI/NodeStyle.h>
#include <QPainter>
#include <QGraphicsProxyWidget>


using namespace UI;


UI::NodeUI::NodeUI(QJsonObject document, std::string_view name)
	:style(std::make_shared<NodeStyle>(document, name)) {}

UI::NodeUI::NodeUI(const NodeUI& in)
	:style(in.style)
	, l_main(new GraphicsLinearLayout(Qt::Orientation::Horizontal))
{
	sinks.reserve(in.GetModel().SinksCount());
	sources.reserve(in.GetModel().SourcesCount());
}

UI::NodeUI::~NodeUI()
{
	b_destroyed = true;
	ConnectionMap::Trim(*this);
}

void UI::NodeUI::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	DrawBackground(painter);
	QGraphicsWidget::paint(painter, option, widget);
}

void UI::NodeUI::DrawBackground(QPainter* painter)
{
	constexpr qreal edge_size = 10.0;
	constexpr qreal offset = PortStyle::port_bbox / 2;

	auto xwidth = geometry().width() - PortStyle::port_bbox;
	// path for the caption of this node
	QPainterPath path_title;
	path_title.setFillRule(Qt::WindingFill);
	path_title.addRoundedRect(QRectF(offset, 0, xwidth, NodeStyle::title_height), edge_size, edge_size);
	path_title.addRect(offset, NodeStyle::title_height - edge_size, xwidth, edge_size);
	painter->setPen(Qt::NoPen);
	painter->setBrush(style->Title());
	painter->drawPath(path_title.simplified());


	// path for the content of this node
	QPainterPath path_content;
	path_content.setFillRule(Qt::WindingFill);
	path_content.addRoundedRect(QRectF(offset, NodeStyle::title_height, xwidth, geometry().height() - NodeStyle::title_height), edge_size, edge_size);
	path_content.addRect(offset, NodeStyle::title_height, xwidth, edge_size);
	painter->setPen(Qt::NoPen);
	painter->setBrush(style->Background());
	painter->drawPath(path_content.simplified());

	if (isSelected()) {
		painter->setPen({ { "#FFFFFFFF" },2.0 });
	}
	else {
		painter->setPen(Qt::NoPen);
	}
	painter->setBrush(Qt::BrushStyle::NoBrush);
	painter->drawPath((path_title + path_content).simplified());
}

QVariant UI::NodeUI::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemPositionChange)
	{
		auto delta = value.toPointF() - pos();
		for (auto& x : sinks)x.MoveConnections(delta);
		for (auto& x : sources)x.MoveConnections(delta);
	}
	return INode::itemChange(change, value);
}

void UI::NodeUI::Init()
{
	proxy = std::make_unique<QGraphicsProxyWidget>();
	auto xlab = new QLabel(style->StyleName());
	QFont a{}; a.setBold(true);
	QPalette p;
	p.setColor(xlab->foregroundRole(), style->FontColor());
	xlab->setFont(a);
	xlab->setAutoFillBackground(true);
	xlab->setPalette(p);
	xlab->setStyleSheet("QLabel {background: transparent; }");
	xlab->setAlignment(Qt::AlignCenter);
	proxy->setWidget(xlab);

	l_left.emplace(Qt::Orientation::Vertical);
	l_central.emplace(Qt::Orientation::Vertical);
	l_right.emplace(Qt::Orientation::Vertical);

	setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	setAcceptHoverEvents(true);
	setZValue(0.0f);

	const auto off_l = sinks.empty() ? PortStyle::port_bbox : 0.0f;
	const auto off_r = sources.empty() ? PortStyle::port_bbox : 0.0f;
	for (auto& x : sinks)l_left->addItem(&x);
	for (auto& x : sources)l_right->addItem(&x);

	l_main->setContentsMargins(off_l, 3.0f, off_r, PortStyle::port_bbox);
	l_central->setSpacing(NodeStyle::item_padding);
	l_central->addItem(proxy.get());
	l_central->setItemSpacing(0, NodeStyle::title_height);
	ConstructModules();

	l_main->addItem(std::addressof(*l_left));
	l_main->addItem(std::addressof(*l_central));
	l_main->addItem(std::addressof(*l_right));
	setLayout(l_main);
	Update();
	adjustSize();
}

void UI::NodeUI::ConstructModules()
{
	auto r = GetModel().GetLayout();
	modules.reserve(r.size());
	for (const auto& x : r)
		l_central->addItem(&modules.emplace_back(x));
}

void UI::NodeUI::UpdateLayouts()
{
	auto h = l_main->contentsRect().height();
	auto sink_delta = (h - sinks.size() * PortStyle::port_bbox) / (sinks.size() + 1);
	auto source_delta = (h - sources.size() * PortStyle::port_bbox) / (sources.size() + 1);

	l_left->setContentsMargins(0.0f, sink_delta + NodeStyle::title_height * (sinks.size() > 0), 0.0f, 0.0f);
	l_left->setSpacing(sink_delta);

	l_right->setContentsMargins(0.0f, source_delta + NodeStyle::title_height * (sources.size() > 0), 0.0f, 0.0f);
	l_right->setSpacing(source_delta);
	l_main->activate();
}

QLabel& UI::NodeUI::Header()
{
	return *static_cast<QLabel*>(proxy->widget());
}

QJsonObject UI::NodeUI::Serialize()
{
	QJsonObject j;
	QJsonArray xpos;
	QJsonObject node = GetModel().Serialize();

	xpos.append(scenePos().x());
	xpos.append(scenePos().y());
	node.insert("Position", xpos);

	j.insert(style->StyleName(), node);
	return j;
}

void UI::NodeUI::Deserialize(QJsonObject in)
{
	if (in.contains("Position"))
	{
		auto v = in["Position"].toArray();
		setPos(QPointF{ v[0].toDouble(), v[1].toDouble() });
	}
	GetModel().Deserialize(in);
	Update();
}

std::string_view UI::NodeUI::Name() const
{
	return GetModel().GetName();
}

void UI::NodeUI::Update()
{
	if (b_destroyed)return;
	GetModel().Update();
	for (auto& x : modules)
		x.Update();
	ConnectionMap::UpdateGraph(*this);
	update();
}

std::string UI::NodeUI::Export()
{
	return GetModel().Export();
}

void UI::NodeUI::ExportSilent(std::string_view in)
{
	return GetModel().ExportSilent(in);
}

void UI::NodeUI::StartConnection(uint8_t index)
{
	ConnectionMap::MakeTemporary(sources[index]);
}

void UI::NodeUI::FinishConnection(uint8_t index)
{
	ConnectionMap::ConnectTemporary(sinks[index]);
}
