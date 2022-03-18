#pragma once
#include <QDockWidget>
#include <QGroupBox>
#include <UI/Updater.h>
#include <vector>

namespace UI::Windows
{
	class PropertyElement : public QGroupBox
	{
	public:
		PropertyElement(INode& parent, std::string_view title);
		PropertyElement(PropertyElement&& o)noexcept;
		~PropertyElement()
		{
			for (auto* i : attached)
				Detach(i);
		}
	public:
		template <typename W, typename ...Args> requires std::derived_from<W, QWidget>
		W& AppendWidget(Args&&... args)
		{
			auto& r = static_cast<W&>(*widgets.emplace_back(std::make_unique<W>(std::forward<Args>(args)...)));
			if constexpr (std::derived_from<W, IUpdater>)
				((IUpdater&)r).SetChangedCallback(&parent);
			if constexpr (std::derived_from<W, PropertyUpdater>)
				((PropertyUpdater&)r).SetChangedCallback(&parent, *this);

			lay.addWidget(&r);
			return r;
		}
		void Clear()noexcept
		{
			widgets.clear();
		}
		void Attach(QWidget* w)
		{
			attached.push_back(w);
			lay.addWidget(w);
		}
		void Detach(QWidget* w)
		{
			lay.removeWidget(w);
		}
	private:
		INode& parent;
		QVBoxLayout lay;
		std::vector<std::unique_ptr<QWidget>> widgets;
		std::vector<QWidget*> attached;
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
		PropertyElement& MakeElement(INode& parent, std::string_view title);
		void Show();
		void Clear();
	private:
		Dummy dum;
		std::vector<PropertyElement> props;
	};
}