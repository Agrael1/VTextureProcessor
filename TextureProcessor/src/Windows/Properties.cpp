/**
 * @file Properties.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Controls for properties dock window
 */
#include <Windows/Properties.h>

using namespace UI::Windows;

/**
 * @brief Creates Sub Layout for selected node
 * @param parent Tied parent (used for callback)
 * @param title  Name of the parent
*/
PropertyElement::PropertyElement(INode& parent, std::string_view title)
    :QGroupBox(title.data()), parent(parent)
{
    lay.setAlignment(Qt::AlignTop);
    setLayout(&lay);
}

/**
 * @brief Move constructor for vector
 * @param o previous element
*/
PropertyElement::PropertyElement(PropertyElement&& o)
    :QGroupBox(o.title()), widgets(std::move(o.widgets)), parent(o.parent)
{
    lay.setAlignment(Qt::AlignTop);
    setLayout(&lay);
    for (auto& w : widgets)
        lay.addWidget(w.get());
}

/**
 * @brief Makes Dummy for structured layout
*/
Properties::Dummy::Dummy() {
    lay.setAlignment(Qt::AlignTop); setLayout(&lay);
    setSizePolicy(QSizePolicy{ QSizePolicy::Ignored, QSizePolicy::Preferred });
}

/**
 * @brief Dock window constructor
 * @param parent parent to dock onto
*/
Properties::Properties(QWidget* parent)
    :QDockWidget("Properties", parent)
{
    setMinimumWidth(150);
    setWidget(&dum);
}

/**
 * @brief Appends layout for node with name
 * @param node layout to grab on
 * @param name name of the node
 * @return reference to newly created object
*/
PropertyElement& Properties::AppendProperty(INode& node, std::string_view name)
{
    return props.emplace_back(node, name);
}

/**
 * @brief Moves existing property in vector
 * @param prop previous property
*/
void Properties::AppendProperty(PropertyElement&& prop)
{
    props.emplace_back(std::move(prop));
}

/**
 * @brief Adds all widgets to dock window
*/
void Properties::Set()
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
