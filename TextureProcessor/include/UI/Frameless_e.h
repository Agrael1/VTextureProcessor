#pragma once

class FrameLess : public QObject
{
public:
	enum Edge {
		None = 0x0,
		Left = 0x1,
		Top = 0x2,
		Right = 0x4,
		Bottom = 0x8,
		TopLeft = 0x10,
		TopRight = 0x20,
		BottomLeft = 0x40,
		BottomRight = 0x80,
	};
public:
	FrameLess(QWidget* target);

	void setBorderWidth(int w) {
		_borderWidth = w;
	}
	int borderWidth() const {
		return _borderWidth;
	}

public:
	bool eventFilter(QObject* o, QEvent* e) override;
	void mouseHover(QHoverEvent*);
	void mouseLeave(QEvent*);
	void mousePress(QMouseEvent*);
	void mouseRealese(QMouseEvent*);
	void mouseMove(QMouseEvent*);
	void updateCursorShape(const QPoint&);
	void calculateCursorPosition(const QPoint&, const QRect&, Edge&);

private:
	QWidget* _target = nullptr;
	QRubberBand* _rubberband = nullptr;
	bool _cursorchanged;
	bool _leftButtonPressed;
	Edge _mousePress = Edge::None;
	Edge _mouseMove = Edge::None;
	int _borderWidth;

	QPoint _dragPos;
	bool _dragStart = false;
};

inline bool testFlag(FrameLess::Edge flag, FrameLess::Edge i) noexcept { return (i & int(flag)) == int(flag) && (int(flag) != 0 || i == int(flag)); }

