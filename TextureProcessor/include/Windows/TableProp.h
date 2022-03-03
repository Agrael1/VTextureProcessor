#pragma once
#include <QDockWidget>
#include <QTableWidget>
#include <QHeaderView>

namespace UI::Windows
{
	class TableProperties : public QDockWidget
	{
	public:
		TableProperties(QWidget* tab);
		void OnItemChanged(QTableWidgetItem* item);
		void SetName(const QString& xname);
	private:
		QTableWidget table;
		QWidget* tab;
	};
}
