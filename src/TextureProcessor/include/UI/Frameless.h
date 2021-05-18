/**
 * @file Frameless.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief Class for frameless window style
 */
#pragma once

class FrameLess : public QObject
{
public:
	enum class Edge :uint8_t{
		None = 0x0,
		Left = 0x1,
		Top = 0x2,
		Right = 0x4,
		Bottom = 0x8,
		TopLeft = Top|Left,
		TopRight = Top | Right,
		BottomLeft = Bottom|Left,
		BottomRight = Bottom|Right,
	};
public:
	FrameLess(QWidget* target);
public:
	bool eventFilter(QObject* o, QEvent* e) override;
	void MouseHover(QHoverEvent*);
	void MouseLeave(QEvent*);
	void MousePress(QMouseEvent*);
	void MouseRealese(QMouseEvent*);
	void MouseMove(QMouseEvent*);
	void UpdateCursor(const QPoint&);
	Edge LocateCursor(const QPoint&, const QRect&);

private:
	QWidget& target;
	QRubberBand rubberband;
	QPoint last_pos;

	bool cursorchanged = false;
	bool bLBPress = false;
	Edge cursor_edge = Edge::None;

	static constexpr int border_width = 5;
};


