#include <UI/UINode.h>
#include <UI/Connection.h>
#include <UI/NodeStyle.h>
#include <QPainter>
#include <QGraphicsProxyWidget>


using namespace UI;

UI::NodeUI::NodeUI(std::unique_ptr<ver::Node> model)
	:model(std::move(model)),
	l_main(new GraphicsLinearLayout(Qt::Orientation::Horizontal)),
	l_left(Qt::Orientation::Vertical),
	l_central(Qt::Orientation::Vertical),
	l_right(Qt::Orientation::Vertical)
{
	setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	setAcceptHoverEvents(true);
	setZValue(0.0f);

	l_main->addItem(&l_left);
	l_main->addItem(&l_central);
	l_main->addItem(&l_right);

	l_central.setSpacing(NodeStyle::item_padding);
	setLayout(l_main);

	auto& stn = model->GetStyle().StyleName();
	if (stn == "Output"){
		type = Type::Output;
	}
	else if(stn == "Input"){
		type = Type::Input;
	}


	Init();
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
	auto xheight = Header().height() + Header().fontMetrics().height() / 2;

	// path for the caption of this node
	QPainterPath path_title;
	path_title.setFillRule(Qt::WindingFill);
	path_title.addRoundedRect(QRectF(offset, 0, xwidth, xheight), edge_size, edge_size);
	path_title.addRect(offset, xheight - edge_size, xwidth, edge_size);
	painter->setPen(Qt::NoPen);
	painter->setBrush(GetModel().GetStyle().Title());
	painter->drawPath(path_title.simplified());


	// path for the content of this node
	QPainterPath path_content;
	path_content.setFillRule(Qt::WindingFill);
	path_content.addRoundedRect(QRectF(offset, xheight, xwidth, geometry().height() - xheight), edge_size, edge_size);
	path_content.addRect(offset, xheight, xwidth, edge_size);
	painter->setPen(Qt::NoPen);
	painter->setBrush(GetModel().GetStyle().Background());
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

void UI::NodeUI::MakeHeader()
{
	auto& style = GetModel().GetStyle();
	if (!proxy)
	{
		proxy = std::make_unique<QGraphicsProxyWidget>();
		l_central.addItem(proxy.get());
		l_central.setItemSpacing(0, 2 * NodeStyle::item_padding);
	}
	auto xlab = new QLabel(style.StyleName());
	QFont a{}; a.setBold(true);
	QPalette p;
	p.setColor(xlab->foregroundRole(), style.FontColor());
	xlab->setFont(a);
	xlab->setAutoFillBackground(true);
	xlab->setPalette(p);
	xlab->setAttribute(Qt::WA_TranslucentBackground);
	xlab->setAlignment(Qt::AlignCenter);
	xlab->setWordWrap(true);
	proxy->setWidget(xlab);
}
void UI::NodeUI::MakeSinks()
{
	size_t sk = model->SinksCount();
	sinks.clear();
	sinks.reserve(sk);
	for (uint8_t i = 0; i < sk; i++)
		l_left.addItem(&sinks.emplace_back(*this, i, model->GetSink(i)));
}
void UI::NodeUI::MakeSources()
{
	size_t sk = model->SinksCount();
	sources.clear();
	sources.reserve(sk);

	for (uint8_t i = 0; i < sk; i++)
		l_right.addItem(&sources.emplace_back(*this, i, model->GetSource(i)));
}

void UI::NodeUI::Init()
{
	MakeHeader();
	MakeSinks();
	MakeSources();
	ConstructModules();
	UpdateLayouts();

	Update();
	adjustSize();
}

void UI::NodeUI::ConstructModules()
{
	auto r = GetModel().GetLayout();
	modules.reserve(r.size());
	for (const auto& x : r)
		l_central.addItem(&modules.emplace_back(x));
}

void UI::NodeUI::UpdateLayouts()
{
	const auto off_l = sinks.empty() ? PortStyle::port_bbox : 0.0f;
	const auto off_r = sources.empty() ? PortStyle::port_bbox : 0.0f;
	l_main->setContentsMargins(off_l, 3.0f, off_r, PortStyle::port_bbox);

	auto h = l_main->contentsRect().height();
	auto sink_delta = (h - sinks.size() * PortStyle::port_bbox) / (sinks.size() + 1);
	auto source_delta = (h - sources.size() * PortStyle::port_bbox) / (sources.size() + 1);

	l_left.setContentsMargins(0.0f, sink_delta + 2 * NodeStyle::item_padding * (sinks.size() > 0), 0.0f, 0.0f);
	l_left.setSpacing(sink_delta);

	l_right.setContentsMargins(0.0f, source_delta + 2 * NodeStyle::item_padding * (sources.size() > 0), 0.0f, 0.0f);
	l_right.setSpacing(source_delta);
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

	j.insert(GetModel().GetStyle().StyleName(), node);
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

void UI::NodeUI::UpdateProperties(Windows::PropertyElement& properties)
{
	PlaceProperties(properties, *model);
}

void UI::NodeUI::Disconnect()
{
	ConnectionMap::Trim(*this);
}
