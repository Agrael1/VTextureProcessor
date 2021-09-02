#pragma once
#include <QGraphicsLinearLayout>

namespace UI
{
	class GraphicsLinearLayout : public QGraphicsLinearLayout
	{
	public:
		using QGraphicsLinearLayout::QGraphicsLinearLayout;
	public:
		~GraphicsLinearLayout()override
		{
			for (std::ptrdiff_t i = count() - 1; i >= 0; removeAt(i), i--);
		}
	};
}