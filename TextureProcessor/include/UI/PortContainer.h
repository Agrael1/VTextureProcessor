#pragma once
#include <QBoxLayout>
#include <QToolButton>
#include <QListWidget>
#include <Logic/PortType.h>

#include <unordered_map>


namespace ver
{
	class Node;
}

class Adder;

namespace UI
{
	class PortContainer
	{
	public:
		struct PortDesc
		{
			QString name;
			PortType type;
		};
	public:
		PortContainer();
	public:
		Adder* AddEmpty();
		void ClearEmpty(QWidget* box);
		QLayout* Layout()
		{
			return &vl;
		}
		void SetHeader(const QString& head)
		{
			heading.setText(head);
		}

		std::vector<PortDesc> GetPorts()const noexcept;
	private:
		QVBoxLayout vl;
		QHBoxLayout hl;
		QLabel heading;
		QToolButton new_prop;
		QListWidget props;

		std::unordered_map<QWidget*, std::unique_ptr<QListWidgetItem>> added;
	};

	class PortsProperty :public QWidget
	{
	public:
		PortsProperty();
	public:
		void LoadPorts(ver::Node& model);
		auto GetSinks()const noexcept
		{
			return sinks.GetPorts();
		}
		auto GetSources()const noexcept
		{
			return sources.GetPorts();
		}
	private:
		QVBoxLayout vl;
		PortContainer sinks;
		PortContainer sources;
	};
}