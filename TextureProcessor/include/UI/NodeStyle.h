#include <QBrush>
#include <QJsonObject>

namespace UI
{
	class NodeStyle
	{
	public:
		constexpr static const qreal title_height = 20.0;
	public:
		NodeStyle() = default;
		NodeStyle(QJsonObject document);
	public:
		auto MinWidth()const noexcept { return minsize.width(); }
		auto MinHeight()const noexcept { return minsize.height(); }
		auto Background()const noexcept { return brBackground; }
		auto Title()const noexcept { return brTitle; }
	private:
		QSizeF minsize{150, 120};
		struct
		{
			QColor Selected{ "#FFFF36A7" };
			QColor Normal{ "#7F000000" };
		}boundary;
		QBrush brTitle{ "#E3212121" };
		QBrush brBackground{ "#E31a1a1a" };
	};
}

