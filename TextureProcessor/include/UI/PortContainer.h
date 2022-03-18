﻿#pragma once
#include <QBoxLayout>
#include <QToolButton>
#include <QListWidget>

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
	private:
		QVBoxLayout vl;
		PortContainer sinks;
		PortContainer sources;
	};
}