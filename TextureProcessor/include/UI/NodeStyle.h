/**
 * @file NodeStyle.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Node, Port and Connection style specification
 */
#pragma once
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QJsonObject>
#include <Interfaces/ISerialize.h>

namespace UI
{
	class NodeStyle : public ISerialize
	{
		friend class NodeUI;
	public:
		constexpr static const qreal h_offset = 10.0;
		constexpr static const qreal pen_width = 1.0;
		constexpr static const qreal item_padding = 10.0;
	public:
		NodeStyle() = default;
		NodeStyle(QJsonObject document);
	public:
		virtual void Serialize(QJsonObject& doc)override;
		virtual bool Deserialize(QJsonObject doc)override;

		const QString& StyleName()const noexcept { return styleName; }
		void SetStyleName(QString name)noexcept { styleName = std::move(name); }
		const QPen& Boundary(bool selected)const noexcept { return selected ? boundary.Selected : boundary.Normal; }
	private:
		struct
		{
			QPen Selected{ QColor{u"#FFFF36A7"}, pen_width };
			QPen Normal{ QColor{u"#7F000000"} , pen_width };
		}boundary;
		QBrush brTitle{ QColor{u"#E3212121"} };
		QBrush brBackground{ QColor{u"#E31a1a1a"} };
		QColor font_color{ Qt::white };
		QString styleName;
	};

	struct ConnectionStyle
	{
		constexpr static const int line_width = 3;
	public:
		QPen sketch{ QColor{u"#FF878787"}, line_width, Qt::DashLine };
		QPen connected{ QColor{u"#FFD9DDDC"}, line_width };
		QPen selected{ Qt::white, line_width * 2 };
	public:
		static const ConnectionStyle Grayscale;
	};
}
