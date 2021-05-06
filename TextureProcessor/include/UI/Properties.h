#pragma once
#include <QDockWidget>
#include <QGroupBox>
#include <memory>
#include <vector>
#include <UI/INode.h>
#include <UI/Updater.h>

namespace UI
{
	class PropertyElement : public QGroupBox
	{
	public:
		PropertyElement(INode& parent, std::string_view title)
			:QGroupBox(title.data()), parent(parent)
		{
			lay.setAlignment(Qt::AlignTop);
			setLayout(&lay);
		}
		PropertyElement(PropertyElement&& o)
			:QGroupBox(o.title()), widgets(std::move(o.widgets)), parent(o.parent)
		{
			lay.setAlignment(Qt::AlignTop);
			setLayout(&lay);
			for (auto& w : widgets)
				lay.addWidget(w.get());
		};
	public:
		template <typename W, typename ...Args> requires std::derived_from<W, QWidget>
		W& AppendWidget(Args&&... args)
		{
			auto& r = static_cast<W&>(*widgets.emplace_back(std::make_unique<W>(std::forward<Args>(args)...)));
			if constexpr (std::derived_from<W, Updater>)
				((Updater&)r).SetChangedCallback(&parent);

			lay.addWidget(&r);
			return r;
		}
	private:
		INode& parent;
		QVBoxLayout lay;
		std::vector<std::unique_ptr<QWidget>> widgets;
	};

	class Properties : public QDockWidget
	{
		class Dummy :public QWidget
		{
		public:
			Dummy() { lay.setAlignment(Qt::AlignTop); setLayout(&lay); }
		public:
			QVBoxLayout lay;
		};
	public:
		Properties(QWidget* parent = nullptr)
			:QDockWidget("Properties", parent)
		{
			setMinimumWidth(150);
			setWidget(&dum);
		}
	public:
		PropertyElement& AppendProperty(INode& node, std::string_view name)
		{
			return props.emplace_back(node, name);
		}
		void AppendProperty(PropertyElement&& prop)
		{
			props.emplace_back(std::move(prop));
		}
		void Set()
		{
			for (auto& w : props)
				dum.lay.addWidget(&w);
		}
		void Clear()
		{
			for (auto& w : props)
				dum.lay.removeWidget(&w);
			props.clear();
		}
	private:
		Dummy dum;
		std::vector<PropertyElement> props;
	};
}
