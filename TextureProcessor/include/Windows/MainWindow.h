/**
 * @file Window.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Main window event handling
 */
#pragma once
#include <QMainWindow>
#include <QMenu>

#include <UI/Properties.h>
#include <UI/TabRelay.h>
#include <UI/FlowCodex.h>


class MainWindow : public QMainWindow
{
	class Internal
	{
	public:
		Internal(QMainWindow* x, std::filesystem::path&& projPath, std::pair<int, int> resolution);
	public:
		void OnClearTriggered();
		void OnProps()
		{
			props.show();
		}
		void OnExport();
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
		void OnLoadNode();
		void OnViewDelete();
		void OnViewClrSel();
	private:
		UI::Windows::TabRelay tab;
		UI::Windows::Properties props;

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
		UI::Windows::SceneTab* cur_scene = nullptr;
	};
public:
	static constexpr std::string_view AppName = "VTexEditor";
public:
	MainWindow(int32_t width, int32_t height, std::filesystem::path&& projPath, std::pair<int,int> resolution);
public:
	void closeEvent(QCloseEvent* event)override
	{
		a.reset();
	}
private:
	std::optional<Internal> a;
};