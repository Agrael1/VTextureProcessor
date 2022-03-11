#include <UI/PropertyContainer.h>
#include <UI/Sliders.h>
#include <QComboBox>
#include <Logic/DynamicNode.h>

#include <QStyledItemDelegate>


class Box : public QWidget, public QStyledItemDelegate
{
public:
	Box(UI::PropertyContainer* parent)
	{
		QWidget::connect(&edit, &QToolButton::pressed, [=]() {parent->Modify(this); });
		setLayout(&vl);
	}
	~Box()
	{
		printf("boob");
	}
public:
	void MakeProp(ver::dc::ElementRef ref)
	{
		edit.setIcon(QIcon{ ":/icons8-edit.png" });
		edit.setMinimumSize(16, 16);
		edit.setIconSize({ 16, 16 });
		hl.setAlignment(Qt::AlignRight);
		hl.addWidget(&edit);
		vl.addLayout(&hl);

		auto t = ref.GetType();
		using namespace UI;
		switch (t)
		{
		case ver::dc::Type::Float:
			vl.addWidget(new QLabel(ref.GetName().data(), this));
			vl.addWidget(new FloatSlider{ (float&)ref, this });
			break;
		case ver::dc::Type::Float2:
			vl.addWidget(new QLabel(ref.GetName().data(), this));
			vl.addWidget(new Vec2Slider{ (QVector2D&)ref, this });
			break;
		case ver::dc::Type::Float3:
			break;
		case ver::dc::Type::Float4:
			break;
		case ver::dc::Type::Matrix:
			break;
		case ver::dc::Type::Bool:
			vl.addWidget(new CheckBox{ (bool&)ref,ref.GetName().data(), this });
			break;
		case ver::dc::Type::Integer:
			vl.addWidget(new QLabel(ref.GetName().data(), this));
			vl.addWidget(new IntSlider{ (int&)ref, this });
			break;
		default:
			break;
		}
	}
private:
	QVBoxLayout vl;
	QHBoxLayout hl;
	QToolButton edit;
};
class Adder : public QWidget
{
	static const QRegularExpression varname;
public:
	Adder(UI::PropertyContainer* parent, QWidget* old_box = nullptr)
		:head_name("New Property:")
		, type("Type:")
		, name("Name:")
		, cname("Code Name:")
		, old_box(old_box)
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

		accept.setIcon(QIcon{ ":/icons8-accept.png" });
		l_control.setAlignment(Qt::AlignRight);
		l_control.addWidget(&accept);
		vl.addLayout(&l_control);
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

	QHBoxLayout l_control;
	QToolButton accept;

	QWidget* old_box;
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
	//vl.addWidget(&props);
	setLayout(&vl);

	MakeProps();

	//props.setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
	//props.setDragDropMode(QAbstractItemView::InternalMove);
	//props.setDragEnabled(true);
}

void UI::PropertyContainer::MakeProps()
{
	//for (auto i : model.GetBuffer())
	//{
	//	auto* x = new Box(this); //is removed with list
	//	auto& r = *old.emplace(x,std::make_unique<QListWidgetItem>()).first->second;

	//	x->MakeProp(i);

	//	r.setSizeHint(x->sizeHint());
	//	props.addItem(&r);
	//	props.setItemWidget(&r, x);
	//}
}

void UI::PropertyContainer::AddEmpty()
{
	//auto lw = std::make_unique<QListWidgetItem>();
	//auto* x = new Adder(this);
	//lw->setSizeHint(x->sizeHint());
	//auto* r = added.emplace(x, std::move(lw)).first->second.get();
	//props.addItem(r);
	//props.setItemWidget(r, x);
}

void UI::PropertyContainer::ClearEmpty()
{
	
}

void UI::PropertyContainer::Modify(Box* box)
{
	//auto lw = std::move(old[box]);
	//old.erase(box);
	//props.removeItemWidget(lw.get());

	//

	//auto* x = new Adder(this);
	//lw->setSizeHint(x->sizeHint());
	//auto* r = added.emplace(x, std::move(lw)).first->second.get();
	//props.setItemWidget(r, x);
}

//ver::dc::ElementRef UI::PropertyContainer::PlaceNewProperty(ver::dc::Type t)
//{
//	
//}
