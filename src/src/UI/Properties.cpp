#include <UI/Properties.h>

using namespace UI;

PropertyElement::PropertyElement(INode& parent, std::string_view title)
    :QGroupBox(title.data()), parent(parent)
{
    lay.setAlignment(Qt::AlignTop);
    setLayout(&lay);
}

PropertyElement::PropertyElement(PropertyElement&& o)
    :QGroupBox(o.title()), widgets(std::move(o.widgets)), parent(o.parent)
{
    lay.setAlignment(Qt::AlignTop);
    setLayout(&lay);
    for (auto& w : widgets)
        lay.addWidget(w.get());
}


Properties::Dummy::Dummy() {
    lay.setAlignment(Qt::AlignTop); setLayout(&lay);
    setSizePolicy(QSizePolicy{ QSizePolicy::Ignored, QSizePolicy::Preferred });
}

Properties::Properties(QWidget* parent)
    :QDockWidget("Properties", parent)
{
    setMinimumWidth(150);
    setWidget(&dum);
}

PropertyElement& Properties::AppendProperty(INode& node, std::string_view name)
{
    return props.emplace_back(node, name);
}

void Properties::AppendProperty(PropertyElement&& prop)
{
    props.emplace_back(std::move(prop));
}

void Properties::Set()
{
    auto s = size();
    for (auto& w : props)
        dum.lay.addWidget(&w);
    resize(s);
}

void Properties::Clear()
{
    for (auto& w : props)
        dum.lay.removeWidget(&w);
    props.clear();
}
