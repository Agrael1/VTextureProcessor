#pragma once

namespace UI
{
	class NodeStyle
	{
	public:
		constexpr static const qreal title_height = 20.0;
		constexpr static const qreal pen_width = 1.0;
		constexpr static const qreal item_padding = 5.0;
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
		constexpr static const qreal diameter = 8.0;
		constexpr static const qreal port_bbox = 20.0;
	public:
		QPen port{ {"#FFFFFFFF"}, NodeStyle::pen_width};
		QBrush brSink{ "#FF0AC710" };
		QBrush brSource{ "#FF129DB5" };
	public:
		static const PortStyle Grayscale;
	};
}

