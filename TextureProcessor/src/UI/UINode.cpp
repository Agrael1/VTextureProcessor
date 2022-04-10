#include <UI/UINode.h>
#include <UI/Connection.h>
#include <UI/NodeStyle.h>
#include <QPainter>
#include <QLabel>
#include <QJsonArray>
#include <QGraphicsProxyWidget>
#include <Logic/Node.h>


using namespace UI;

UI::NodeUI::NodeUI(std::unique_ptr<ver::Node> xmodel)
	:model(std::move(xmodel)),
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
	setLayout(l_main);

	auto stn = model->GetStyle().StyleName();
	if (stn == u"Output") {
		type = Type::Output;
	}
	else if (stn == u"Input") {
		type = Type::Input;
	}

	l_main->setContentsMargins(0.0, 0.0, 0.0, 0.0);
	l_main->setSpacing(0.0);

	l_central.setSpacing(NodeStyle::item_padding);
	l_central.setContentsMargins(0.0, 0.0, 0.0, NodeStyle::item_padding);

	Init();
}
UI::NodeUI::~NodeUI()
{
	b_destroyed = true;
	Disconnect();
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
	auto& style = model->GetStyle();

	auto xwidth = geometry().width() - PortStyle::port_bbox;
	auto xheight = Header().height();

	// path for the caption of this node
	QPainterPath path_title;
	path_title.setFillRule(Qt::WindingFill);
	path_title.addRoundedRect(QRectF(offset, 0, xwidth, xheight), edge_size, edge_size);
	path_title.addRect(offset, xheight - edge_size, xwidth, edge_size);
	painter->setPen(Qt::NoPen);
	painter->setBrush(style.brTitle);
	painter->drawPath(path_title.simplified());


	// path for the content of this node
	QPainterPath path_content;
	path_content.setFillRule(Qt::WindingFill);
	path_content.addRoundedRect(QRectF(offset, xheight, xwidth, geometry().height() - xheight), edge_size, edge_size);
	path_content.addRect(offset, xheight, xwidth, edge_size);
	painter->setBrush(style.brBackground);
	painter->drawPath(path_content.simplified());

	painter->setPen(style.Boundary(isSelected()));
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
	if (proxy)return UpdateHeader();
	auto& style = GetModel().GetStyle();
	proxy = std::make_unique<QGraphicsProxyWidget>();
	auto xlab = new QLabel(style.StyleName());
	QFont a{}; a.setBold(true);
	QFontMetrics m{ a };
	auto sz = m.height() >> 1;
	QPalette p;
	p.setColor(xlab->foregroundRole(), style.font_color);
	xlab->setFont(a);
	xlab->setAutoFillBackground(true);
	xlab->setPalette(p);
	xlab->setAttribute(Qt::WA_TranslucentBackground);
	xlab->setAlignment(Qt::AlignCenter);
	xlab->setWordWrap(true);
	xlab->setContentsMargins(0, sz, 0, sz);
	proxy->setWidget(xlab);
	l_central.addItem(proxy.get());
}
void UI::NodeUI::UpdateHeader()
{
	auto& h = Header();
	h.setText(model->GetStyle().StyleName());
	h.adjustSize();
	h.setMinimumSize(h.size());
}
void UI::NodeUI::MakeSinks()
{
	size_t sk = model->SinksCount();
	sinks.reserve(sk);
	for (uint8_t i = 0; i < sk; i++)
		l_left.addItem(&sinks.emplace_back(*this, i, model->GetSink(i)));
}
void UI::NodeUI::MakeSources()
{
	size_t sk = model->SourcesCount();
	sources.reserve(sk);
	for (uint8_t i = 0; i < sk; i++)
		l_right.addItem(&sources.emplace_back(*this, i, model->GetSource(i)));
}
void UI::NodeUI::ConstructModules()
{
	//make probe
	modules.clear();
	auto r = model->GetLayout();
	modules.reserve(r.size());
	for (const auto& x : r)
		l_central.addItem(&modules.emplace_back(x));
	l_central.activate();
}

void UI::NodeUI::Init()
{
	MakeHeader();
	ConstructModules();

	MakeSinks();
	MakeSources();

	UpdateLayouts();
	Update();
}

void UI::NodeUI::UpdateLayouts()
{
	auto h = l_central.contentsRect().height();
	auto sink_s = sinks.size();
	auto source_s = sources.size();

	auto dsi = (h - sink_s * PortStyle::port_bbox);
	auto dso = (h - source_s * PortStyle::port_bbox);
	auto sink_delta = dsi < 0 ? 0 : dsi / (sink_s + 1);
	auto source_delta = dso < 0 ? 0 : dso / (source_s + 1);

	l_left.setContentsMargins(sink_s ? 0.0 : PortStyle::port_bbox, sink_delta + PortStyle::port_bbox / 2, 0.0f, 0.0f);
	l_left.setSpacing(sink_delta);

	l_right.setContentsMargins(0.0f, source_delta + PortStyle::port_bbox / 2, source_s ? 0.0 : PortStyle::port_bbox, 0.0f);
	l_right.setSpacing(source_delta);
	l_main->activate();
}
QLabel& UI::NodeUI::Header()
{
	return *static_cast<QLabel*>(proxy->widget());
}

void UI::NodeUI::Serialize(QJsonObject& doc)
{
	QJsonArray xpos;
	xpos.append(scenePos().x());
	xpos.append(scenePos().y());

	doc.insert(u"Type", model->GetStyle().StyleName());
	model->Serialize(doc);
	doc.insert(u"Position", xpos);
}
bool UI::NodeUI::Deserialize(QJsonObject in)
{
	if (in.contains(u"Position"))
	{
		auto vx = in[u"Position"];
		if (!vx.isArray()) return false;
		auto v = vx.toArray();
		if (v.size() < 2) return false;
		setPos(QPointF{ v[0].toDouble(), v[1].toDouble() });
	}
	if (!GetModel().Deserialize(in))return false;
	Update();
	return true;
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
	if(scene())
		QueryConnectionMap(scene()).UpdateGraph(*this);
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
	QueryConnectionMap(scene()).MakeTemporary(sources[index]);
}

void UI::NodeUI::FinishConnection(uint8_t index)
{
	QueryConnectionMap(scene()).ConnectTemporary(sinks[index]);
}

void UI::NodeUI::UpdateProperties(Windows::PropertyElement& properties)
{
	model->GetProperties(properties);
}

void UI::NodeUI::Disconnect()
{
	QueryConnectionMap(scene()).Trim(*this);
}


