#pragma once
#include <QBoxLayout>
#include <QToolButton>
#include <QListWidget>

#include <unordered_map>


namespace ver
{
	class ShaderNode;
}

namespace UI
{
	namespace Windows {
		class PropertyElement;
	}

	class PortContainer : public QWidget
	{
	public:
		PortContainer(ver::ShaderNode& model);
	public:
		void MakeProps();
		void AddEmpty();
		void ClearEmpty(QWidget* box);
	private:
		QVBoxLayout vl;
		QHBoxLayout hl;
		QLabel heading;
		QToolButton new_prop;
		QListWidget props;

		std::unordered_map<QWidget*, std::unique_ptr<QListWidgetItem>> added;
	};
}