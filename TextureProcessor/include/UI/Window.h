/**
 * @file Window.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Main window event handling
 */
#pragma once
#include <QMainWindow>
#include <UI/Properties.h>
#include <UI/TabRelay.h>


class Window : public QMainWindow
{
	class Internal
	{
	public:
		Internal(QMainWindow* x, std::filesystem::path&& projPath);
	public:
		void OnClearTriggered()
		{
			if (cur_scene)
				cur_scene->Clear();
		}
		void OnProps()
		{
			props.show();
		}
		void OnExport()
		{
			cur_scene->Export();
		}
		void OnSave()
		{
			tab.OnSave();
		}
		void OnSaveAs()
		{
			tab.OnSaveAs();
		}
		void OnLoad();
		void OnCreateNode();
		void OnViewDelete()
		{
			if (cur_scene)
				cur_scene->DeleteSelected();
		}
		void OnViewClrSel()
		{
			if (cur_scene)
				cur_scene->ClearSelection();
		}
	private:
		TabRelay tab;

		UI::Properties props;

		QMenu file;
		QMenu windows;
		QAction Aclear;
		QAction Aexport;
		QAction Asave;
		QAction Asaveas;
		QAction Aload;
		QAction Aprops;

		QMenu nodes;
		QAction Acreaten;
		QAction Aloadn;

		QMenu view;
		QAction Adelet;
		QAction Aclrselect;
		SceneTab* cur_scene = nullptr;
	};
public:
	static constexpr std::string_view AppName = "VTexEditor";
public:
	Window(int32_t width, int32_t height, std::filesystem::path&& projPath);
public:
	void ShowMaximized()
	{
		showMaximized();
	}
	void Show()
	{
		showNormal();
	}
	void closeEvent(QCloseEvent* event)override
	{
		a.reset();
	}
private:
	std::optional<Internal> a;
};