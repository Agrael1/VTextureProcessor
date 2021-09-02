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

namespace UI
{
	class NodeStyle
	{
	public:
		constexpr static const qreal title_height = 20.0;
		constexpr static const qreal h_offset = 10.0;
		constexpr static const qreal pen_width = 1.0;
		constexpr static const qreal item_padding = 10.0;
		constexpr static const qreal min_width = 150.0;
		constexpr static const qreal min_height = 120.0;
	public:
		NodeStyle() = default;
		NodeStyle(QJsonObject document, std::string_view styleName);
	public:
		auto Background()const noexcept { return brBackground; }
		auto Title()const noexcept { return brTitle; }
		auto FontColor()const noexcept { return font_color; }
		std::string_view StyleName()const noexcept {return styleName;}
	private:
		struct
		{
			QColor Selected{ "#FFFF36A7" };
			QColor Normal{ "#7F000000" };
		}boundary;
		QBrush brTitle{ "#E3212121" };
		QBrush brBackground{ "#E31a1a1a" };
		QColor font_color{ "#7FFFFFFF" };
		std::string styleName;
	};

	struct PortStyle
	{
	public:
		constexpr static const qreal diameter = 10.0;
		constexpr static const qreal port_bbox = 20.0;
	public:
		QPen port{ {"#FFFFFFFF"}, NodeStyle::pen_width};
		QBrush brSink{ "#FF828282" };
		QBrush brSinkUsed{ "#FFD9DDDC" };
		QBrush brSource{ "#FF909090" };
	public:
		static const PortStyle Grayscale;
	};

	struct ConnectionStyle
	{
		constexpr static const int line_width = 3;
	public:
		QPen sketch{ {"#FF878787"}, line_width, Qt::DashLine };
		QPen connected{ {"#FFD9DDDC"}, line_width };
		QPen selected{ Qt::white, line_width*2 };
	public:
		static const ConnectionStyle Grayscale;
	};
}
