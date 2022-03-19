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
			for (auto& i : attached)
				i->setParent(nullptr);
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
		void Attach(std::shared_ptr<QWidget> w)
		{
			lay.addWidget(attached.emplace_back(std::move(w)).get());
		}
	private:
		INode& parent;
		QVBoxLayout lay;
		std::vector<std::unique_ptr<QWidget>> widgets;
		std::vector<std::shared_ptr<QWidget>> attached;
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