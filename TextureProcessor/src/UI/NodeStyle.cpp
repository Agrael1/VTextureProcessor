#include <UI/NodeStyle.h>
#include <array>

using namespace UI;

void NODE_STYLE_READ_COLOR(QJsonObject values, QColor& variable, std::string_view varname)
{
    auto valueRef = values[varname.data()];
    if (valueRef.isArray())
    {
        auto colorArray = valueRef.toArray();
        std::array<int, 3> rgb;
        for (size_t i = 0; const auto & it : colorArray) {
            rgb[i++] = it.toInt();
        }
        variable = QColor(rgb[0], rgb[1], rgb[2]);
    }
    else
        variable = QColor(valueRef.toString());
}

NodeStyle::NodeStyle(QJsonObject document)
{
	QJsonValueRef nodeStyleValues = document["NodeStyle"];
	QJsonObject obj = nodeStyleValues.toObject();
}