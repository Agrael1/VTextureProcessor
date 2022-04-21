#pragma once
#include <QToolButton>
#include <QBoxLayout>
#include <QLabel>
#include <Projects/ProjectsData.h>
#include <Projects/ProjectDescriptor.h>
#include <Logic/Constants.h>


namespace UI
{
	class MainPage : public QWidget
	{
		Q_OBJECT
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
			static constexpr ver::u16string_literal style = uR"(QToolButton{
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
			XButton(const QString& head, const QString& descr = u""_qs, const QString& xicon = u""_qs);
		private:
			QHBoxLayout lay;
			QVBoxLayout vlay;
			QLabel icon;
			QLabel header;
			QLabel desc;
		};
	public:
		MainPage();
	public:
		void AddProject(const std::filesystem::path& xpath);
	protected:
		void OnCreateClicked(bool checked);
		void OnOpenClicked(bool checked);
	signals:
		void CreateClicked();
		void OpenProject(ProjectDescriptor desc);
	private:
		void FillTree();
		void OnFilterChanged(const QString& text);
		void OnItemDoubleClicked(QTreeWidgetItem* item, int column);
	private:
		ProjectsData pdata;

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
	};
}