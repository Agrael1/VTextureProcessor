#pragma once
#include <QDockWidget>
#include <QGroupBox>
#include <UI/Updater.h>
#include <vector>

namespace UI::Windows
{
	class XPropertyElement : public QGroupBox
	{
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

				lay.addWidget(&r);
				return r;
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
		void AppendProperty(XPropertyElement&& prop);
		void Set();
		void Clear();
	private:
		Dummy dum;
		std::vector<XPropertyElement> props;
	};
}