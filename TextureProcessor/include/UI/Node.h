#pragma once
#include <UI/NodeStyle.h>
#include <UI/NodeModules.h>
#include <UI/PropertyGenerator.h>
#include <UI/GraphicsLayout.h>
#include <UI/Port.h>
#include <Interfaces/INode.h>
#include <Logic/Node.h>

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QJsonArray>
#include <UI/Connection.h>

namespace UI
{
	template<class XModel> requires std::derived_from<XModel, ver::Node>
		class XNode : public IXNode
		{
		public:
			template<class ...Args>
			XNode(QJsonObject document, std::string_view name, Args&&... e)
				:style(document, name), model(document, std::forward<Args>(e)...)
			{
			}
			XNode(const XNode& in)
				:style(in.style)
				, model(in.model)
				, l_main(new GraphicsLinearLayout(Qt::Orientation::Horizontal))
			{
				l_left.emplace(Qt::Orientation::Vertical);
				l_central.emplace(Qt::Orientation::Vertical);
				l_right.emplace(Qt::Orientation::Vertical);

				sinks.reserve(in.model.SinksCount());
				for (uint8_t i = 0; i < in.model.SinksCount(); i++)
					sinks.emplace_back(*this, i, model.GetSink(i));

				sources.reserve(in.model.SourcesCount());
				for (uint8_t i = 0; i < in.model.SourcesCount(); i++)
					sources.emplace_back(*this, i, model.GetSource(i));
				Init();
			}
			~XNode()
			{
				b_destroyed = true;
				XConnMapper::Trim(*this);
			}
		public:
			virtual void paint(QPainter* painter,
				const QStyleOptionGraphicsItem* option,
				QWidget* widget = nullptr) override
			{
				DrawBackground(painter);
				DrawCaptionName(painter);

				//DrawConnectionPoints(painter);
				QGraphicsWidget::paint(painter, option, widget);
				[[unlikely]] if (b_update) {
					UpdateLayouts();
					b_update = false;
				}
			}
			virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value)override
			{
				if (change == ItemPositionChange)
				{
					auto delta = value.toPointF() - pos();
					for (auto& x : sinks)x.MoveConnections(delta);
					for (auto& x : sources)x.MoveConnections(delta);
				}
				return IXNode::itemChange(change, value);
			}

			virtual std::unique_ptr<IXNode> Clone(std::string&& name)const override
			{
				printf("copy called\n");
				auto x = std::make_unique<XNode>(*this);
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
				if (b_destroyed)return;
				model.Update();
				for (auto& x : modules)
					x.Update();
				XConnMapper::UpdateGraph(*this);
				update();
			}
			virtual std::string Export()override
			{
				return model.Export();
			}
			virtual void ExportSilent(std::string_view in)override
			{
				return model.ExportSilent(in);
			}
		private:
			void DrawBackground(QPainter* painter)
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
				painter->setBrush(style.Title());
				painter->drawPath(path_title.simplified());


				// path for the content of this node
				QPainterPath path_content;
				path_content.setFillRule(Qt::WindingFill);
				path_content.addRoundedRect(QRectF(offset, NodeStyle::title_height, xwidth, geometry().height() - NodeStyle::title_height), edge_size, edge_size);
				path_content.addRect(offset, NodeStyle::title_height, xwidth, edge_size);
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

				const auto off_l = sinks.empty() ? PortStyle::port_bbox : 0.0f;
				const auto off_r = sources.empty() ? PortStyle::port_bbox : 0.0f;
				for (auto& x : sinks)l_left->addItem(&x);
				for (auto& x : sources)l_right->addItem(&x);

				l_main->setContentsMargins(off_l, NodeStyle::title_height + PortStyle::port_bbox, off_r, PortStyle::port_bbox);
				l_central->setSpacing(NodeStyle::item_padding);
				ConstructModules();

				l_main->addItem(std::addressof(*l_left));
				l_main->addItem(std::addressof(*l_central));
				l_main->addItem(std::addressof(*l_right));
				setLayout(l_main);
				Update();
			}
			void ConstructModules()
			{
				auto r = model.GetLayout();
				modules.reserve(r.size());
				for (const auto& x : r)
					l_central->addItem(&modules.emplace_back(x));
			}

			virtual QJsonObject Serialize()override 
			{
				QJsonObject j;
				QJsonArray xpos;
				QJsonObject node = model.Serialize();

				xpos.append(scenePos().x());
				xpos.append(scenePos().y());
				node.insert("Position", xpos);

				j.insert(style.StyleName().data(), node);
				return j;
			};
			virtual void Deserialize(QJsonObject)override {};

			void UpdateLayouts()
			{
				auto h = l_main->contentsRect().height();
				auto sink_delta = (h - sinks.size() * PortStyle::port_bbox) / (sinks.size() + 1);
				auto source_delta = (h - sources.size() * PortStyle::port_bbox) / (sources.size() + 1);

				l_left->setContentsMargins(0.0f, sink_delta, 0.0f, 0.0f);
				l_left->setSpacing(sink_delta);

				l_right->setContentsMargins(0.0f, source_delta, 0.0f, 0.0f);
				l_right->setSpacing(source_delta);
			}
		private:
			NodeStyle style;
			XModel model;

			GraphicsLinearLayout* l_main;

			std::vector<Source> sources;
			std::vector<Sink> sinks;

			std::optional<QGraphicsLinearLayout> l_left;
			std::optional<QGraphicsLinearLayout> l_central;
			std::optional<QGraphicsLinearLayout> l_right;

			std::vector<Module> modules;
			bool b_update = true;
			bool b_destroyed = false;
		};
}

