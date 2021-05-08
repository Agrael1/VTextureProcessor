#pragma once
#include <QMainWindow>
#include <QToolButton>
#include <QLabel>
#include <UI/Frameless_e.h>


class ProjectsCW : public QWidget
{
public:
	ProjectsCW(QWidget* parent)
		:QWidget(parent)
	{
		lay.setAlignment(Qt::AlignTop | Qt::AlignRight);
		setSizePolicy({ QSizePolicy::Maximum ,QSizePolicy::Maximum });

		close.setIcon(QIcon{ ":/icon_window_close.png" });
		maximize.setIcon(QIcon{ ":/icon_window_maximize.png" });
		minimize.setIcon(QIcon{ ":/icon_window_minimize.png" });

		minimize.setStyleSheet(style);
		maximize.setStyleSheet(style);
		close.setStyleSheet(closestyle);

		buttons.setAlignment(Qt::AlignTop | Qt::AlignRight);
		buttons.addWidget(&minimize);
		buttons.addWidget(&maximize);
		buttons.addWidget(&close);

		connect(&close, &QToolButton::clicked, this, &ProjectsCW::OnCloseClicked);
		connect(&minimize, &QToolButton::clicked, this, &ProjectsCW::OnMinimizedClicked);
		connect(&maximize, &QToolButton::clicked, this, &ProjectsCW::OnMaximizedClicked);
		lay.addLayout(&buttons);
		setLayout(&lay);
	}
public:
	void Restore() { OnRestoreClicked(false); }
	QVBoxLayout& Layout()noexcept { return lay; }
protected:
	void OnCloseClicked(bool checked)
	{
		parentWidget()->close();
	}
	void OnMaximizedClicked(bool checked)
	{
		maximize.setIcon(QIcon{ ":/icon_window_restore.png" });
		connect(&maximize, &QToolButton::clicked, this, &ProjectsCW::OnRestoreClicked);
		parentWidget()->showMaximized();
	}
	void OnRestoreClicked(bool checked)
	{
		maximize.setIcon(QIcon{ ":/icon_window_maximize.png" });
		connect(&maximize, &QToolButton::clicked, this, &ProjectsCW::OnMaximizedClicked);
		parentWidget()->showNormal();
	}
	void OnMinimizedClicked(bool checked)
	{
		parentWidget()->showMinimized();
	}
private:
	QVBoxLayout lay;
	QHBoxLayout buttons;
	QToolButton close;
	QToolButton minimize;
	QToolButton maximize;

	static constexpr const char* style = R"(QToolButton{
		background-color:none;
		border:none;
		width:25px;
		height:16px;
		padding:4px;
	}
	QToolButton:hover{
		background-color:gray;
	}
	QToolButton:pressed{
		background-color:palette(highlight);
	})";
	static constexpr const char* closestyle = R"(QToolButton{
		background-color:none;
		border:none;
		width:25px;
		height:16px;
		padding:4px;
	}
	QToolButton:hover{
		background-color:red;
	}
	QToolButton:pressed{
		background-color:palette(highlight);
	})";
};

class ProjectsWindow : public QMainWindow
{
public:
	ProjectsWindow(int32_t width, int32_t height)
		:QMainWindow(nullptr, Qt::FramelessWindowHint), window(this), f(this), name("Veritas Texture Flow")
	{
		resize(width, height);
		setCentralWidget(&window);
		window.Layout().addLayout(&lay);
		lay.setAlignment(Qt::AlignTop | Qt::AlignLeft);
		name.setSizePolicy({ QSizePolicy::Expanding ,QSizePolicy::Preferred });
		name.setFont({"Arial", 24});
		lay.setContentsMargins({ 40,0,40,0 });
		lay.addWidget(&name);

	}

	void mouseMoveEvent(QMouseEvent* e)override
	{
		if (isMaximized())
		{
			window.Restore();
			move(e->globalPos() / 2);
			return;
		}
		f.mouseMove(e);
	}
private:
	ProjectsCW window;
	FrameLess f;
	QVBoxLayout lay;
	QLabel name;
};