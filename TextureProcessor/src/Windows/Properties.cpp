#include <Windows/Properties.h>
//#include <UI/PropertyGenerator.h>
#include <Interfaces/INode.h>
#include <Logic/DynamicConstant.h>
#include <UI/BufferWidgets.h>

using namespace UI::Windows;

UI::Windows::Properties::Properties(QWidget* parent)
	:QDockWidget("Properties", parent)
{
	s.setWidget(&dum);
	s.setWidgetResizable(true);
	setWidget(&s);
}

Properties::Dummy::Dummy() {
	lay.setAlignment(Qt::AlignTop); setLayout(&lay);
	setSizePolicy(QSizePolicy{ QSizePolicy::Ignored, QSizePolicy::Preferred });
}

/**
 * @brief Creates Sub Layout for selected node
 * @param parent Tied parent (used for callback)
 * @param title  Name of the parent
*/
PropertyElement::PropertyElement(INode& parent, std::string_view title)
	:QGroupBox(title.data()), parent(parent)
{
	lay.setAlignment(Qt::AlignTop);
	lay.setSpacing(0);

	int a, b, c, d;
	getContentsMargins(&a, &b, &c, &d);
	setContentsMargins(0, b, 0, 0);
	setLayout(&lay);
}

/**
 * @brief Move constructor for vector
 * @param o previous element
*/
PropertyElement::PropertyElement(PropertyElement&& o)noexcept
	:QGroupBox(o.title()), widgets(std::move(o.widgets)), parent(o.parent)
{
	lay.setAlignment(Qt::AlignTop);
	lay.setContentsMargins(0,0,0,0);
	lay.setSpacing(0);

	int a, b, c, d;
	getContentsMargins(&a, &b, &c, &d);
	setContentsMargins(0, b, 0, 0);
	setLayout(&lay);
	for (auto& w : widgets)
		lay.addWidget(w.get());
}

void UI::Windows::PropertyElement::AppendBuffer(ver::dc::Buffer& buf, std::span<ver::dc::Options> opts)
{
	using enum ver::dc::Type;
	auto it = opts.begin();

	for (auto&& ref : buf)
	{
		auto* opt = std::addressof(*it);
		auto t = ref.GetType();
		QString name = opt->enable_alias ? opt->alias.c_str() : ref.GetName().data();

		switch (t)
		{
#define X(a) case ver::dc::Type::a:	AppendWidget(std::make_unique<BufferWidget<ver::dc::Type::a>>(ref, *it++));break;
			LEAF_ELEMENT_TYPES
#undef X
		default:
			break;
		}
	}
}

void UI::Windows::PropertyElement::AppendWidget(std::unique_ptr<AwareWidget> r)
{
	connect(r.get(), &AwareWidget::ValueChanged, [this]() {parent.Update(); });
	lay.addWidget(widgets.emplace_back(std::move(r)).get());
}
void UI::Windows::PropertyElement::AppendUpdaterWidget(std::unique_ptr<AwareWidget> r)
{
	connect(r.get(), &AwareWidget::ValueChanged, [this]() {
		Clear();
		parent.UpdateProperties(*this);
		});
	lay.addWidget(widgets.emplace_back(std::move(r)).get());
}

PropertyElement& UI::Windows::Properties::MakeElement(INode& parent, std::string_view title)
{
	return props.emplace_back(parent, title);
}

/**
 * @brief Adds all widgets to dock window
*/
void Properties::Show()
{
	for (auto& w : props)
		dum.lay.addWidget(&w);
}

/**
 * @brief clears all widgets
*/
void Properties::Clear()
{
	for (auto& w : props)
		dum.lay.removeWidget(&w);
	props.clear();
}