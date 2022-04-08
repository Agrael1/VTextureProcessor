#include <Logic/PortType.h>

using namespace UI;
const PortStyle PortStyle::Grayscale;
const PortStyle PortStyle::Color{ .brSink{QColor{u"#1fff35"}},.brSource{QColor{u"#15b024"}} };
const PortStyle PortStyle::Normal{ .brSink{QColor{u"#a87dff"}},.brSource{QColor{u"#906ed4"}} };