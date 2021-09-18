#pragma once
#include <QDockWidget>
#include <QTableWidget>


namespace UI::Windows
{
	class TableProperties : public QDockWidget
	{
	public:
		TableProperties() :QDockWidget("Node Properties"),
			table(1,2)
		{
			setWidget(&table);
		}
	private:
		QTableWidget table;
	};
}
