#include <UI/ColorWidget.h>
#include <QColorDialog>
#include <QPainter>

using namespace UI;

ColorWidget::ColorWidget()
{
	hl.addWidget(&name);
	hl.addWidget(&button);

	button.setFrameShape(QFrame::Shape::Box);
	button.setFrameShadow(QFrame::Shadow::Raised);
	button.setAutoFillBackground(true);

	hl.setContentsMargins(0, 1, 1, 1);
	setLayout(&hl);
}

void ColorWidget::SetColor(const QColor& c)
{
	name.setText(c.name());
	button.cache = c;
	button.update();
	emit ColorChanged(c);
}

void UI::ColorWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	QWidget::mouseDoubleClickEvent(event);
	SetColor(QColorDialog::getColor(button.cache));
}

void UI::ColorWidget::ColorFrame::paintEvent(QPaintEvent*)
{
	QPainter paint(this);
	paint.setBrush(cache);
	drawFrame(&paint);
}
