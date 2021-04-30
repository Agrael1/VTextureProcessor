#include <UI/NodeStyle.h>
#include <array>

using namespace UI;
const PortStyle PortStyle::Grayscale;
const ConnectionStyle ConnectionStyle::Grayscale;


static inline QColor ReadColor(QJsonObject values, std::string_view varname)
{
    auto valueRef = values[varname.data()];
    if (valueRef.isArray())
    {
        auto colorArray = valueRef.toArray();
        std::array<int, 3> rgb;
        for (size_t i = 0; const auto & it : colorArray)
            rgb[i++] = it.toInt();
        return { rgb[0], rgb[1], rgb[2] };
    }
    return {valueRef.toString()};
}

NodeStyle::NodeStyle(QJsonObject document, std::string_view name)
    :styleName(name)
{
	QJsonValueRef nodeStyleValues = document["NodeStyle"];
	QJsonObject obj = nodeStyleValues.toObject();

    brTitle.setColor(ReadColor(obj, "TitleColor"));
    font_color = ReadColor(obj, "FontColor");
}