#pragma once
#include <UI/NodeStyle.h>
#include <UI/Connection.h>
#include <UI/NodeModules.h>
#include <UI/PropertyGenerator.h>
#include <Logic/Node.h>

#include <QPainter>

namespace UI
{
	template<class XModel> requires std::derived_from<XModel, ver::Node>
		class XNode : public IXNode//, public ISerialize
		{
		public:
			template<class ...Args>
			XNode(QJsonObject document, std::string_view name, Args&&... e)
				:gll(nullptr), style(document, name), model(document, std::forward<Args>(e)...)
			{
			}
			XNode(const XNode& in)
				:style(in.style)
				, model(in.model)
				, gll(new QGraphicsLinearLayout)
			{
				connections.resize(in.model.SinksCount());
				Init();
			}
		public:
			QRectF boundingRect() const override
			{
				// Compensate for pen width
				return QRectF(
					-NodeStyle::pen_width / 2.0 - PortStyle::port_bbox,
					-NodeStyle::pen_width / 2.0,
					geometry().width() + NodeStyle::pen_width / 2.0 + 2.0 * PortStyle::port_bbox,
					geometry().height() + NodeStyle::pen_width / 2.0
				).normalized();
			}
			void paint(QPainter* painter,
				const QStyleOptionGraphicsItem* option,
				QWidget* widget = nullptr) override
			{
				DrawBackground(painter);
				DrawCaptionName(painter);
				DrawConnectionPoints(painter);
				QGraphicsWidget::paint(painter, option, widget);
			}
			virtual std::unique_ptr<IXNode> Clone(std::string&& name)const override
			{
				printf("copy called\n");
				auto x = std::make_unique<XNode>(*this);
				x->Init();
				x->model.SetUniqueName(name);
				return x;
			}
			virtual std::string_view Name()const override
			{
				return model.GetName();
			}
			virtual void UpdateProperties(Windows::XPropertyElement& properties) override
			{
				PlaceProperties(properties, model, *this);
			}
			virtual void Update()override
			{
				model.Update();
				for (auto& x : modules)
					x.Update();
				update();
			}
		private:
			void DrawBackground(QPainter* painter)
			{
				constexpr qreal edge_size = 10.0;

				auto xwidth = geometry().width();
				// path for the caption of this node
				QPainterPath path_title;
				path_title.setFillRule(Qt::WindingFill);
				path_title.addRoundedRect(QRectF(0, 0, xwidth, NodeStyle::title_height), edge_size, edge_size);
				path_title.addRect(0, NodeStyle::title_height - edge_size, xwidth, edge_size);
				painter->setPen(Qt::NoPen);
				painter->setBrush(style.Title());
				painter->drawPath(path_title.simplified());


				// path for the content of this node
				QPainterPath path_content;
				path_content.setFillRule(Qt::WindingFill);
				path_content.addRoundedRect(QRectF(0, NodeStyle::title_height, xwidth, geometry().height() - NodeStyle::title_height), edge_size, edge_size);
				path_content.addRect(0, NodeStyle::title_height, xwidth, edge_size);
				painter->setPen(Qt::NoPen);
				painter->setBrush(style.Background());
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
			void DrawCaptionName(QPainter* painter)
			{
				QString name{ style.StyleName().data() };
				QFont f = painter->font();
				f.setBold(true);

				QFontMetrics metrics(f);
				auto rect = metrics.boundingRect(name);

				QPointF position((geometry().width() - rect.width()) / 2.0,
					(NodeStyle::title_height - rect.height() / 2.0));

				painter->setFont(f);
				painter->setPen(style.FontColor());
				painter->drawText(position, name);

				f.setBold(false);
				painter->setFont(f);
			}
			void DrawConnectionPoints(QPainter* painter)
			{
				auto h = geometry().height();
				auto pdelta_sink = h / (model.SinksCount() + 1);
				auto pdelta_source = h / (model.SourcesCount() + 1);

				auto& style = PortStyle::Grayscale;

				painter->setPen(style.port);
				auto ypos = pdelta_sink;
				// Draw each Sink connection point
				for (auto si = 0; si < model.SinksCount(); si++)
				{
					painter->setBrush(connections[si] ? style.brSinkUsed : style.brSink);
					painter->drawEllipse(-PortStyle::diameter / 2, ypos, PortStyle::diameter, PortStyle::diameter);
					ypos += pdelta_sink;
				}

				painter->setBrush(style.brSource);
				ypos = pdelta_source;
				// Draw each Source connection point
				for (const auto& so : model.GetSources())
				{
					painter->drawEllipse(geometry().width() - PortStyle::diameter / 2, ypos, PortStyle::diameter, PortStyle::diameter);
					ypos += pdelta_source;
				}
			}
			void Init()
			{
				setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
				setFlag(QGraphicsItem::ItemIsMovable, true);
				setFlag(QGraphicsItem::ItemIsFocusable, true);
				setFlag(QGraphicsItem::ItemIsSelectable, true);
				setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

				setCacheMode(QGraphicsItem::DeviceCoordinateCache);

				setAcceptHoverEvents(true);

				setZValue(0.0f);
				gll->setOrientation(Qt::Vertical);
				gll->setContentsMargins(NodeStyle::h_offset, NodeStyle::title_height + NodeStyle::item_padding, NodeStyle::h_offset, NodeStyle::item_padding);
				gll->setSpacing(NodeStyle::item_padding);
				ConstructModules();
				setLayout(gll);
				Update();
			}
			void ConstructModules()
			{
				auto r = model.GetLayout();
				modules.reserve(r.size());
				for (const auto& x : r)
					gll->addItem(&modules.emplace_back(x));
			}
		private:
			NodeStyle style;
			XModel model;

			QGraphicsLinearLayout* gll;
			std::vector<Module> modules;
			std::vector<std::unique_ptr<Connection>> connections; //sink holds connection
		};
}

