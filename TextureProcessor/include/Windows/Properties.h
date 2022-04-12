#pragma once
#include <UI/AwareWidget.h>
#include <QDockWidget>
#include <QGroupBox>
#include <QScrollArea>
#include <QBoxLayout>
#include <vector>

namespace UI
{
	struct INode;
}
namespace ver::dc
{
	class Buffer;
	struct Options;
}

namespace UI::Windows
{
	class PropertyElement : public QGroupBox
	{
	public:
		PropertyElement(INode& parent, std::string_view title);
		PropertyElement(PropertyElement&& o)noexcept;
		~PropertyElement()
		{
			DetachAll();
		}
	public:
		void AppendBuffer(ver::dc::Buffer& buf, std::span<ver::dc::Options> opts);
		void AppendWidget(std::unique_ptr<AwareWidget> r);
		void AppendUpdaterWidget(std::unique_ptr<AwareWidget> r);
		void Clear()noexcept
		{
			widgets.clear();
			DetachAll();
		}
		void Attach(std::shared_ptr<QWidget> w)
		{
			lay.addWidget(attached.emplace_back(std::move(w)).get());
		}
		void DetachAll()
		{
			for (auto& i : attached)
				i->setParent(nullptr);
		}
	private:
		INode& parent;
		QVBoxLayout lay;
		std::vector<std::unique_ptr<AwareWidget>> widgets;
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
		QScrollArea s;
		Dummy dum;
		std::vector<PropertyElement> props;
	};
}