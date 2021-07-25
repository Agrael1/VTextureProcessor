/**
 * @file ProjectsWindow.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Node header color specification
 */
#pragma once
#include <QMainWindow>
#include <QToolButton>
#include <QLabel>
#include <QBoxLayout>
#include <QTreeWidget>
#include <QLineEdit>

#include <UI/Frameless.h>
#include <Logic/ProjectsData.h>

namespace UI::Internal
{
/**
  * @brief represents non-client area of a custom window
 */
class ProjectsCW : public QWidget
{
public:
	ProjectsCW(QWidget* parent);
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
}



class ProjectsWindow : public QMainWindow
{
	class Project : public QWidget
	{
	public:
		Project(const std::filesystem::path& xpath);
	private:
		QHBoxLayout hlay;
		QVBoxLayout vlay;

		QLabel ico;
		QLabel filename;
		QLabel path;
	};
	class XButton : public QToolButton
	{
		static constexpr const char* style = R"(QToolButton{
				background-color:#292929;
				border:none;
				height:60px;
				padding:4px;
			}
			QToolButton:hover{
				background-color:gray;
			}
			QToolButton:pressed{
				background-color:palette(highlight);
			})";
	public:
		XButton(std::string_view head, std::string_view descr = "", std::string_view xicon = "");
	private:
		QHBoxLayout lay;
		QVBoxLayout vlay;
		QLabel icon;
		QLabel header;
		QLabel desc;
	};
public:
	ProjectsWindow(int32_t width, int32_t height, QObject& app);
protected:
	void mouseMoveEvent(QMouseEvent* e)override;
	void OnCreateClicked(bool checked);
	void OnOpenClicked(bool checked);
	void OpenApp(std::filesystem::path&& projPath);
private:
	void FillTree();
	void OnFilterChanged(const QString& text);
	void OnItemDoubleClicked(QTreeWidgetItem* item, int column);

private:
	QObject& app;
	UI::Internal::ProjectsCW window;
	UI::FrameLess f;

	QVBoxLayout lay;
	QHBoxLayout hlay;
	QVBoxLayout innerLay;
	QVBoxLayout buttonLay;

	QLabel name;
	QLabel recent;
	QLineEdit search;
	QTreeWidget selection;

	QLabel begin;
	XButton create;
	XButton open;
	ProjectsData pdata;
};

