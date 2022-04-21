/**
 * @file Frameless.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief Class for frameless window style
 */

#include <Projects/Frameless.h>
#include <QEvent>
#include <QMouseEvent>

using namespace UI;

/**
 * @brief Sets incoming widget to become frameless
 * @param target widget to become frameless
*/
FrameLess::FrameLess(QWidget* target) :
	target(*target),
	rubberband(QRubberBand::Rectangle)
{
	target->setMouseTracking(true);
	target->setWindowFlags(Qt::FramelessWindowHint);
	target->setAttribute(Qt::WA_Hover);
	target->installEventFilter(this);
}

/**
 * @brief overrides event filter for parent object
 * @param o object to hook onto
 * @param e incoming event
 * @return  true if event is accepted
*/
bool FrameLess::eventFilter(QObject* o, QEvent* e)
{
	switch (e->type()) {
	case QEvent::HoverMove:
		MouseHover(static_cast<QHoverEvent*>(e));
		return true;
	case QEvent::Leave:
		MouseLeave(e);
		return true;
	case QEvent::MouseButtonPress:
		MousePress(static_cast<QMouseEvent*>(e));
		return true;
	case QEvent::MouseButtonRelease:
		MouseRealese(static_cast<QMouseEvent*>(e));
		return true;
	}
	return target.eventFilter(o, e);
}

/**
 * @brief Mouse hover event
 * @param e incoming event
*/
void FrameLess::MouseHover(QHoverEvent* e)
{
	UpdateCursor(target.mapToGlobal(e->pos()));
}

/**
 * @brief Mouse leave event
 * @param e incoming event
*/
void FrameLess::MouseLeave(QEvent* e)
{
	if (!bLBPress)
		target.unsetCursor();
}

/**
 * @brief Mouse press event
 * @param e incoming event
*/
void FrameLess::MousePress(QMouseEvent* e)
{
	if (!(e->button() & Qt::LeftButton))return;

	bLBPress = true;
	cursor_edge = LocateCursor(e->globalPos(), target.frameGeometry());

	if (cursor_edge != Edge::None)
		rubberband.setGeometry(target.frameGeometry());

	if (target.rect().marginsRemoved(QMargins(border_width + 1, border_width + 1, border_width + 1, border_width + 1)).contains(e->pos()))
		last_pos = e->globalPos();
}

/**
 * @brief Mouse release event
 * @param e incoming event
*/
void FrameLess::MouseRealese(QMouseEvent* e)
{
	bLBPress = (e->button() & Qt::LeftButton) < 0;
}

/**
 * @brief Mouse move event (works if some button is pressed)
 * @param e incoming event
*/
void FrameLess::MouseMove(QMouseEvent* e)
{
	if (!(e->buttons() & Qt::MouseButton::LeftButton))
		return;

	if (cursor_edge == Edge::None)
	{
		target.move(target.pos() + e->globalPos() - last_pos);
		last_pos = e->globalPos();
		return;
	}


	//ISSUE: frame.geometry() usage leads to infinite recursion, that's why rubberband is used
	auto frame = rubberband.frameGeometry();
	int left = frame.left();
	int top = frame.top();
	int right = frame.right();
	int bottom = frame.bottom();
	auto pos = e->globalPos();

	switch (cursor_edge)
	{
	case Edge::Top:
		top = pos.y();
		break;
	case Edge::Bottom:
		bottom = pos.y();
		break;
	case Edge::Left:
		left = pos.x();
		break;
	case Edge::Right:
		right = pos.x();
		break;
	case Edge::TopLeft:
		top = pos.y();
		left = pos.x();
		break;
	case Edge::TopRight:
		right = pos.x();
		top = pos.y();
		break;
	case Edge::BottomLeft:
		bottom = pos.y();
		left = pos.x();
		break;
	case Edge::BottomRight:
		bottom = pos.y();
		right = pos.x();
		break;
	}
	QRect newRect(QPoint(left, top), QPoint(right, bottom));

	target.setGeometry(newRect);
	rubberband.setGeometry(newRect);
}

/**
 * @brief Updates cursor form according to position
 * @param pos position of the cursor
*/
void FrameLess::UpdateCursor(const QPoint& pos)
{
	if (target.isFullScreen() || target.isMaximized())
	{
		target.unsetCursor();
		return;
	}
	if (bLBPress) return;

	switch (LocateCursor(pos, target.frameGeometry()))
	{
	case FrameLess::Edge::Right:
	case FrameLess::Edge::Left:
		target.setCursor(Qt::SizeHorCursor);
		break;
	case FrameLess::Edge::Bottom:
	case FrameLess::Edge::Top:
		target.setCursor(Qt::SizeVerCursor);
		break;
	case FrameLess::Edge::BottomRight:
	case FrameLess::Edge::TopLeft:
		target.setCursor(Qt::SizeFDiagCursor);
		break;
	case FrameLess::Edge::TopRight:
	case FrameLess::Edge::BottomLeft:
		target.setCursor(Qt::SizeBDiagCursor);
		break;
	default:
		target.unsetCursor();
		break;
	}
}

/**
 * @brief Sets Edge flag if cursor is on the edge(may overlap)
 * @param pos cursor position
 * @param framerect frame rectangle to locate into
 * @return composite edge collision flag
*/
FrameLess::Edge FrameLess::LocateCursor(const QPoint& pos, const QRect& framerect)
{
	uint8_t onLeft = pos.x() >= framerect.x() - border_width && pos.x() <= framerect.x() + border_width &&
		pos.y() <= framerect.y() + framerect.height() - border_width && pos.y() >= framerect.y() + border_width;

	uint8_t onRight = pos.x() >= framerect.x() + framerect.width() - border_width && pos.x() <= framerect.x() + framerect.width() &&
		pos.y() >= framerect.y() + border_width && pos.y() <= framerect.y() + framerect.height() - border_width;

	uint8_t onBottom = pos.x() >= framerect.x() + border_width && pos.x() <= framerect.x() + framerect.width() - border_width &&
		pos.y() >= framerect.y() + framerect.height() - border_width && pos.y() <= framerect.y() + framerect.height();

	uint8_t onTop = pos.x() >= framerect.x() + border_width && pos.x() <= framerect.x() + framerect.width() - border_width &&
		pos.y() >= framerect.y() && pos.y() <= framerect.y() + border_width;

	return Edge(onLeft | onRight << 2 | onBottom << 3 | onTop << 1);
}