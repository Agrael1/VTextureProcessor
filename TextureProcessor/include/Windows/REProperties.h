#pragma once
#include <QDockWidget>
#include <QGroupBox>
#include <UI/Updater.h>
#include <vector>

namespace UI::Windows
{
	class XPropertyElement : public QGroupBox
	{
		friend class XProperties;
	public:
		XPropertyElement(IXNode& parent, std::string_view title);
		XPropertyElement(XPropertyElement&& o);
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
	private:
		IXNode& parent;
		QVBoxLayout lay;
		std::vector<std::unique_ptr<QWidget>> widgets;
	};

	class XProperties : public QDockWidget
	{
		class Dummy :public QWidget
		{
		public:
			Dummy();
		public:
			QVBoxLayout lay;
		};
	public:
		XProperties(QWidget* parent = nullptr);
	public:
		XPropertyElement& MakeElement(IXNode& parent, std::string_view title);
		void Set();
		void Clear();
	private:
		Dummy dum;
		std::vector<XPropertyElement> props;
	};
}