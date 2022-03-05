#pragma once
#include <Logic/DynamicConstant.h> 

#include <QBoxLayout>
#include <QToolButton>
#include <QListWidget>


namespace ver
{
	class DynamicNode;
}

namespace UI
{
	namespace Windows {
		class PropertyElement;
	}

	class PropertyContainer : public QWidget
	{
	public:
		PropertyContainer(ver::DynamicNode& model);
	public:
		void AddEmpty();
		void ClearEmpty();
		ver::dc::ElementRef PlaceNewProperty(ver::dc::Type t);
	private:
		ver::DynamicNode& model;
	private:
		QVBoxLayout vl;
		QHBoxLayout hl;
		QLabel heading;
		QToolButton new_prop;
		QListWidget props;

		std::unique_ptr<QListWidgetItem> added;
	};
}