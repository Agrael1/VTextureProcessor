#pragma once
#include <Logic/DynamicConstant.h> 

#include <QBoxLayout>
#include <QToolButton>
#include <QListView>

#include <unordered_map>


namespace ver
{
	class ShaderNode;
}
class Box;
class Adder;

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
		void Modify(Box* box);
		//ver::dc::ElementRef PlaceNewProperty(ver::dc::Type t);
	private:
		ver::ShaderNode& model;
		ver::dc::Buffer dum;
	private:
		QVBoxLayout vl;
		QHBoxLayout hl;
		QLabel heading;
		QToolButton new_prop;
		//QListView props;

		//std::unordered_map<Box*, std::unique_ptr<QListWidgetItem>> old;
		//std::unordered_map<Adder*, std::unique_ptr<QListWidgetItem>> added;
	};
}