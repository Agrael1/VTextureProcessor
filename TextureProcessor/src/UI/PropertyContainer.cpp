#include <UI/PropertyContainer.h>
#include <UI/Sliders.h>
#include <QComboBox>
#include <Logic/DynamicNode.h>


class Adder : public QWidget
{
	static const QRegularExpression varname;
public:
	Adder(UI::PropertyContainer* parent)
		:head_name("New Property:")
		, type("Type:")
		, name("Name:")
		, cname("Code Name:")
	{
		close.setIcon(QIcon{ ":/icon_window_close.png" });
		head.addWidget(&head_name);
		head.addWidget(&close);
		vl.addLayout(&head);


		lst.addWidget(&type);
		lst.addWidget(&select);
		vl.addLayout(&lst);

		l_name.addWidget(&name);
		l_name.addWidget(&name_line);
		vl.addLayout(&l_name);

		cname_line.setValidator(new QRegularExpressionValidator(varname, &cname_line));
		l_cname.addWidget(&cname);
		l_cname.addWidget(&cname_line);
		vl.addLayout(&l_cname);
		setLayout(&vl);

		connect(&close, &QToolButton::pressed, [parent]() {parent->ClearEmpty(); });

		select.addItem("None"); size_t i = size_t(ver::dc::Type::Empty);
		std::ranges::for_each(ver::dc::type_strings, [&](auto x) {select.addItem(x, ++i); });

		connect(&select, QOverload<int>::of(&QComboBox::currentIndexChanged), [parent]() {parent->ClearEmpty(); });
	}
public:
	void MakeControl(ver::dc::Type t, ver::dc::ElementRef ref)
	{
		using namespace UI;
		switch (t)
		{
		case ver::dc::Type::Float:
			vl.addWidget(new FloatSlider{ (float&)ref, this});
			break;
		case ver::dc::Type::Float2:
			vl.addWidget(new Vec2Slider{ (QVector2D&)ref, this });
			break;
		case ver::dc::Type::Float3:
			break;
		case ver::dc::Type::Float4:
			break;
		case ver::dc::Type::Matrix:
			break;
		case ver::dc::Type::Bool:
			vl.addWidget(new CheckBox{ (bool&)ref,name.text(), this });
			break;
		case ver::dc::Type::Integer:
			vl.addWidget(new IntSlider{ (int&)ref, this });
			break;
		default:
			break;
		}
	}
private:
	QVBoxLayout vl;
	QHBoxLayout head;
	QToolButton close;
	QLabel head_name;

	QHBoxLayout lst;
	QLabel type;
	QComboBox select;

	QHBoxLayout l_name;
	QLabel name;
	QLineEdit name_line;

	QHBoxLayout l_cname;
	QLabel cname;
	QLineEdit cname_line;
};

const QRegularExpression Adder::varname{ "^[_a-z]\\w*$" };



UI::PropertyContainer::PropertyContainer(ver::ShaderNode& model)
	:model(model), heading("Edited Node:")
{
	new_prop.setMinimumSize(24, 24);
	new_prop.setIconSize({ 24, 24 });
	new_prop.setIcon(QIcon{ ":/icons8-add-property.png" });

	connect(&new_prop, &QToolButton::pressed, this, &PropertyContainer::AddEmpty);

	hl.addWidget(&heading);
	hl.addWidget(&new_prop);
	vl.addLayout(&hl);
	vl.addWidget(&props);
	setLayout(&vl);

	props.setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	props.setDragDropMode(QAbstractItemView::InternalMove);
	props.setDragEnabled(true);
}

void UI::PropertyContainer::AddEmpty()
{
	auto& r = added.emplace_back();

	auto* x = new Adder(this); //is removed with list
	r.setSizeHint(x->sizeHint());
	props.addItem(&r);
	props.setItemWidget(&r, x);
}

void UI::PropertyContainer::ClearEmpty()
{
	
}

//ver::dc::ElementRef UI::PropertyContainer::PlaceNewProperty(ver::dc::Type t)
//{
//	
//}
