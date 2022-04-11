#pragma once
#include <QDockWidget>
#include <QTableWidget>
#include <QHeaderView>

class QComboBox;

namespace UI
{
	class ColorWidget;
}

namespace UI::Windows
{
	class TableProperties : public QDockWidget
	{
		Q_OBJECT
	public:
		TableProperties(QWidget* tab);
	public:
		void OnItemChanged(QTableWidgetItem* item);
		void SetName(const QString& xname);
		void SetCatList(std::vector<QString> c);
		QString GetCategory()const noexcept;
		void SetCategory(const QString& c) noexcept;
		void SetTitleColor(const QColor& c) noexcept;
		void SetFontColor(const QColor& c) noexcept;
	signals:
		void NameChanged(const QString& name);
		void HeadColorChanged(const QColor& color);
		void FontColorChanged(const QColor& color);
	private:
		QTableWidget table;
		QWidget* tab;
		QComboBox* cats;
		ColorWidget* head;
		ColorWidget* font;
	};
}
