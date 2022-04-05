#include <UI/PortContainer.h>
#include <UI/NodeStyle.h>
#include <QComboBox>
#include <QLineEdit>
#include <QStylePainter>

#include <Logic/Node.h>
#include <Logic/Sink.h>
#include <Logic/Source.h>

class ComboBox :public QComboBox
{
public:
	ComboBox(){
		std::ranges::for_each(ver::PortStrings, [this](auto x) {addItem(x.data()); });
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
public:
	void SetName(std::string_view xname)
	{
		name.setText(xname.data());
	}
	void SetType(ver::PortType pt)
	{
		cbox.setCurrentIndex(int(pt));
	}
	QString Name()const noexcept
	{
		return name.text();
	}
	ver::PortType Type()const noexcept
	{
		return ver::PortType(cbox.currentIndex());
	}
private:
	QHBoxLayout hl;
	QLineEdit name;
	QToolButton bclose;
	ComboBox cbox;
};

const QRegularExpression Adder::varname{ "^[_a-z]\\w*$" };



UI::PortContainer::PortContainer()
{
	new_prop.setMinimumSize(24, 24);
	new_prop.setIconSize({ 24, 24 });
	new_prop.setIcon(QIcon{ ":/icons8-add-port.png" });

	vl.connect(&new_prop, &QToolButton::pressed, [this]() { AddEmpty(); });

	hl.addWidget(&heading);
	hl.addWidget(&new_prop);
	vl.addLayout(&hl);
	vl.addWidget(&props);

	props.setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
	props.setDragDropMode(QAbstractItemView::InternalMove);
	props.setDragEnabled(true);
}


Adder* UI::PortContainer::AddEmpty()
{
	auto lw = std::make_unique<QListWidgetItem>();
	auto* x = new Adder(this);
	lw->setSizeHint(x->sizeHint());
	auto* r = added.emplace(x, std::move(lw)).first->second.get();
	props.addItem(r);
	props.setItemWidget(r, x);
	return x;
}

void UI::PortContainer::ClearEmpty(QWidget* box)
{
	added.erase(box);
}

std::vector<ver::PortDesc> UI::PortContainer::GetPorts() const noexcept
{
	size_t length = added.size();
	std::vector<ver::PortDesc> out;
	out.reserve(length);

	for (size_t i = 0; i < length; i++)
	{
		auto* x = static_cast<Adder*>(props.itemWidget(props.item(i)));
		out.emplace_back(x->Name(), x->Type());
	}
	return out;
}

UI::PortsProperty::PortsProperty()
{
	vl.addLayout(sinks.Layout());
	vl.addLayout(sources.Layout());

	sinks.SetHeader("Inputs:");
	sources.SetHeader("Outputs:");
	setLayout(&vl);
}

void UI::PortsProperty::LoadPorts(ver::Node& model)
{
	for (auto& i : model.GetSinks())
	{
		auto* x = sinks.AddEmpty();
		x->SetName(i->GetRegisteredName());
		x->SetType(i->GetType());
	}
	for (auto& i : model.GetSources())
	{
		auto* x = sources.AddEmpty();
		x->SetName(i->GetName());
		x->SetType(i->GetType());
	}
}

