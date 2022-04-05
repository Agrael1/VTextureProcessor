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
		void SetFontColor(QColor c)noexcept { font_color = c; }
		const QString& StyleName()const noexcept {return styleName;}
		void SetStyleName(const QString& name)noexcept { styleName = name; }
	private:
		struct
		{
			QColor Selected{ "#FFFF36A7" };
			QColor Normal{ "#7F000000" };
		}boundary;
		QBrush brTitle{ "#E3212121" };
		QBrush brBackground{ "#E31a1a1a" };
		QColor font_color{ Qt::white };
		QString styleName;
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
