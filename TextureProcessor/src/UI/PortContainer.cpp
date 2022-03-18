#include <UI/PortContainer.h>
#include <QComboBox>
#include <Logic/PortType.h>
#include <QStylePainter>
#include <UI/NodeStyle.h>

class ComboBox :public QComboBox
{
public:
	ComboBox(){
		std::ranges::for_each(PortStrings, [this](auto x) {addItem(x.data()); });
		setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	}
protected:
	void paintEvent(QPaintEvent* ev)override
	{
		QStylePainter painter(this);
		painter.setPen(palette().color(QPalette::Text));
		// draw the combobox frame, focusrect and selected etc.	

		QStyleOptionComboBox cb_tmp;
		initStyleOption(&cb_tmp);
		cb_tmp.currentText = "";
		cb_tmp.subControls &= ~(QStyle::SC_ComboBoxArrow | QStyle::SC_ComboBoxFrame);

		painter.drawComplexControl(QStyle::CC_ComboBox, cb_tmp);
		// draw the icon and text
		painter.setPen(UI::PortStyle::Grayscale.port);
		switch (currentIndex())
		{
		case 1:
			painter.setBrush(UI::PortStyle::Grayscale.brSink);
			break;
		case 2:
			painter.setBrush(UI::PortStyle::Color.brSink);
			break;
		case 3:
			painter.setBrush(UI::PortStyle::Normal.brSink);
			break;
		default:
			break;
		}
		painter.drawEllipse(QPointF{ rect().center() }, UI::PortStyle::diameter / 2, UI::PortStyle::diameter / 2);
	}
};

class Adder : public QWidget
{
	static const QRegularExpression varname;
public:
	Adder(UI::PortContainer* parent)
	{
		bclose.setIcon(QIcon{ ":/icon_window_close.png" });

		hl.addWidget(&name);
		hl.addWidget(&cbox);
		hl.addWidget(&bclose);
		setLayout(&hl);

		name.setPlaceholderText("Port Name");
		connect(&bclose, &QToolButton::clicked, [parent, this]() {parent->ClearEmpty(this); });
	}
private:
	QHBoxLayout hl;
	QLineEdit name;
	QToolButton bclose;
	ComboBox cbox;
};

const QRegularExpression Adder::varname{ "^[_a-z]\\w*$" };



UI::PortContainer::PortContainer(ver::ShaderNode& model)
	: heading("Edited Node:")
{
	new_prop.setMinimumSize(24, 24);
	new_prop.setIconSize({ 24, 24 });
	new_prop.setIcon(QIcon{ ":/icons8-add-property.png" });

	connect(&new_prop, &QToolButton::pressed, this, &PortContainer::AddEmpty);

	hl.addWidget(&heading);
	hl.addWidget(&new_prop);
	vl.addLayout(&hl);
	vl.addWidget(&props);
	setLayout(&vl);

	MakeProps();

	props.setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
	props.setDragDropMode(QAbstractItemView::InternalMove);
	props.setDragEnabled(true);
}

void UI::PortContainer::MakeProps()
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

void UI::PortContainer::AddEmpty()
{
	auto lw = std::make_unique<QListWidgetItem>();
	auto* x = new Adder(this);
	lw->setSizeHint(x->sizeHint());
	auto* r = added.emplace(x, std::move(lw)).first->second.get();
	props.addItem(r);
	props.setItemWidget(r, x);
}

void UI::PortContainer::ClearEmpty(QWidget* box)
{
	added.erase(box);
}
