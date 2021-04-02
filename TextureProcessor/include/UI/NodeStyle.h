#pragma once

namespace UI
{
	class NodeStyle
	{
	public:
		constexpr static const qreal title_height = 20.0;
	public:
		NodeStyle() = default;
		NodeStyle(QJsonObject document, std::string_view styleName);
	public:
		auto MinWidth()const noexcept { return minsize.width(); }
		auto MinHeight()const noexcept { return minsize.height(); }
		auto Background()const noexcept { return brBackground; }
		auto Title()const noexcept { return brTitle; }
		auto FontColor()const noexcept { return font_color; }
		std::string_view StyleName()const noexcept {return styleName;}
	private:
		QSizeF minsize{150, 120};
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
}

