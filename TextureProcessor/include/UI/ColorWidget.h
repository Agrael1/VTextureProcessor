#pragma once
#include <QBoxLayout>
#include <QLabel>

namespace UI
{
	class ColorWidget : public QWidget
	{
		Q_OBJECT
	private:
		struct ColorFrame : public QFrame
		{
			void paintEvent(QPaintEvent*)override;
			QColor cache;
		};
	public:
		ColorWidget();
	public:
		void SetColor(const QColor& c);
	protected:
		virtual void mouseDoubleClickEvent(QMouseEvent* event)override;
	signals:
		void ColorChanged(const QColor& color);
	private:
		QHBoxLayout hl;
		QLabel name;
		ColorFrame button;
	};
}