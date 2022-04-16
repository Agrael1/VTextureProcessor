#include <Logic/PortType.h>

using namespace UI;
const PortStyle PortStyle::Grayscale;
const PortStyle PortStyle::Color{ .brSink{QColor{u"#1fff35"}},.brSource{QColor{u"#15b024"}} };
const PortStyle PortStyle::Normal{ .brSink{QColor{u"#0055ff"}},.brSource{QColor{u"#003399"}} };
const PortStyle PortStyle::Universal{ .brSink{},.brSource{} };