#include <Projects/FrameControl.h>

UI::Internal::FrameControl::FrameControl(QWidget* parent)
	:QWidget(parent)
{
	lay.setAlignment(Qt::AlignTop | Qt::AlignRight);
	setSizePolicy({ QSizePolicy::Maximum ,QSizePolicy::Maximum });

	close.setIcon(QIcon{ u":/icon_window_close.png"_qs });
	maximize.setIcon(QIcon{ u":/icon_window_maximize.png"_qs });
	minimize.setIcon(QIcon{ u":/icon_window_minimize.png"_qs });

	minimize.setStyleSheet(style);
	maximize.setStyleSheet(style);
	close.setStyleSheet(closestyle);

	buttons.setAlignment(Qt::AlignTop | Qt::AlignRight);
	buttons.addWidget(&minimize);
	buttons.addWidget(&maximize);
	buttons.addWidget(&close);

	connect(&close, &QToolButton::clicked, this, &FrameControl::OnCloseClicked);
	connect(&minimize, &QToolButton::clicked, this, &FrameControl::OnMinimizedClicked);
	connect(&maximize, &QToolButton::clicked, this, &FrameControl::OnMaximizedClicked);
	lay.addLayout(&buttons);
	setLayout(&lay);
}