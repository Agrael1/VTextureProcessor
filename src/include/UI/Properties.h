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
		PropertyElement(INode& parent, std::string_view title);
		PropertyElement(PropertyElement&& o);
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
			Dummy();
		public:
			QVBoxLayout lay;
		};
	public:
		Properties(QWidget* parent = nullptr);

	public:
		PropertyElement& AppendProperty(INode& node, std::string_view name);
		void AppendProperty(PropertyElement&& prop);
		void Set();
		void Clear();
	private:
		Dummy dum;
		std::vector<PropertyElement> props;
	};
}