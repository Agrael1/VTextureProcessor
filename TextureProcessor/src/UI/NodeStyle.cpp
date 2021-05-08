#include <UI/NodeStyle.h>
#include <array>

using namespace UI;
const PortStyle PortStyle::Grayscale;
const ConnectionStyle ConnectionStyle::Grayscale;


/**
 * @brief Loads color from color map based on name
 *
 * @param values All mapped colors
 * @param varname Target color name
 * @return QColor Found target color
 */
static inline QColor ReadColor(QJsonObject values, std::string_view varname) noexcept
{
    auto valueRef = values[varname.data()];
    // RGB color name
    if (valueRef.isArray())
    {
        auto colorArray = valueRef.toArray();
        std::array<int, 3> rgb;
        for (size_t i = 0; const auto & it : colorArray)
            rgb[i++] = it.toInt();
        return { rgb[0], rgb[1], rgb[2] };
    }
    // Non-RGB color name ('green', 'red', etc.)
    return {valueRef.toString()};
}

/**
 * @brief Gets Node style based from document on name
 *
 * @param document Node JSON data
 * @param name Name of the Node style
 */
NodeStyle::NodeStyle(QJsonObject document, std::string_view name)
    :styleName(name)
{
	QJsonValueRef nodeStyleValues = document["NodeStyle"];
	QJsonObject obj = nodeStyleValues.toObject();

    brTitle.setColor(ReadColor(obj, "TitleColor"));
    font_color = ReadColor(obj, "FontColor");
}
