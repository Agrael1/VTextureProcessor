#pragma once
#include <Logic/DynamicConstant.h> 

#include <QBoxLayout>
#include <QToolButton>
#include <QListWidget>


namespace ver
{
	class ShaderNode;
}

namespace UI
{
	namespace Windows {
		class PropertyElement;
	}

	class PropertyContainer : public QWidget
	{
	public:
		PropertyContainer(ver::ShaderNode& model);
	public:
		void MakeProps();
		void AddEmpty();
		void ClearEmpty();

		//ver::dc::ElementRef PlaceNewProperty(ver::dc::Type t);
	private:
		ver::ShaderNode& model;
		ver::dc::Buffer dum;
	private:
		QVBoxLayout vl;
		QHBoxLayout hl;
		QLabel heading;
		QToolButton new_prop;
		QListWidget props;

		std::vector<std::unique_ptr<QListWidgetItem>> added;
	};
}