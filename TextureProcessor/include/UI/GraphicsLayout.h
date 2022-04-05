#pragma once
#include <QGraphicsLinearLayout>

namespace UI
{
	class GraphicsLinearLayout : public QGraphicsLinearLayout
	{
	public:
		static constexpr const size_t npos = std::numeric_limits<size_t>::max();
	public:
		GraphicsLinearLayout(Qt::Orientation orientation = Qt::Orientation::Vertical)
			:QGraphicsLinearLayout(orientation)
		{
			setOwnedByLayout(false);
		}
	public:
		~GraphicsLinearLayout()override
		{
			clear();
		}
	public:
		void clear(){
			for (int i = count() - 1; i >= 0; removeAt(i--));
		}
		void remove(size_t begin, size_t end = npos)
		{
			auto xend = end == npos ? count() - 1 : end;
			for (int i = xend; i >= begin; removeAt(i--));
		}
		void append(QGraphicsLayoutItem* item){addItem(item);}
	};
}