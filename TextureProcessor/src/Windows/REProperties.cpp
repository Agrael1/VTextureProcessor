#include <Windows/REProperties.h>

using namespace UI::Windows;

UI::Windows::XProperties::XProperties(QWidget* parent)
	:QDockWidget("Properties", parent)
{
    setWidget(&dum);
}

XProperties::Dummy::Dummy() {
    lay.setAlignment(Qt::AlignTop); setLayout(&lay);
    setSizePolicy(QSizePolicy{ QSizePolicy::Ignored, QSizePolicy::Preferred });
}

/**
 * @brief Creates Sub Layout for selected node
 * @param parent Tied parent (used for callback)
 * @param title  Name of the parent
*/
XPropertyElement::XPropertyElement(IXNode& parent, std::string_view title)
    :QGroupBox(title.data()), parent(parent)
{
    lay.setAlignment(Qt::AlignTop);
    setLayout(&lay);
}

/**
 * @brief Move constructor for vector
 * @param o previous element
*/
XPropertyElement::XPropertyElement(XPropertyElement&& o)
    :QGroupBox(o.title()), widgets(std::move(o.widgets)), parent(o.parent)
{
    lay.setAlignment(Qt::AlignTop);
    setLayout(&lay);
    for (auto& w : widgets)
        lay.addWidget(w.get());
}

/**
 * @brief Moves existing property in vector
 * @param prop previous property
*/
void XProperties::AppendProperty(XPropertyElement&& prop)
{
    props.emplace_back(std::move(prop));
}

/**
 * @brief Adds all widgets to dock window
*/
void XProperties::Set()
{
    for (auto& w : props)
        dum.lay.addWidget(&w);
}

/**
 * @brief clears all widgets
*/
void XProperties::Clear()
{
    for (auto& w : props)
        dum.lay.removeWidget(&w);
    props.clear();
}