#include <UI/NodeStyle.h>
#include <QJsonArray>
#include <array>

using namespace UI;

constexpr auto json = R"(
"NodeStyle": {
        "NormalBoundaryColor": [255, 255, 255] ,
        "SelectedBoundaryColor" : [255, 165, 0] ,
        "GradientColor0" : "gray",
        "GradientColor1" : [80, 80, 80] ,
        "GradientColor2" : [64, 64, 64] ,
        "GradientColor3" : [58, 58, 58] ,
        "ShadowColor" : [20, 20, 20] ,
        "FontColor" : "white",
        "FontColorFaded" : "gray",
        "ConnectionPointColor" : [169, 169, 169] ,
        "FilledConnectionPointColor" : "cyan",
        "ErrorColor" : "red",
        "WarningColor" : [128, 128, 0] ,

        "PenWidth" : 1.0,
        "HoveredPenWidth" : 1.5,

        "ConnectionPointDiameter" : 8.0,

        "Opacity" : 0.8
})";

void NODE_STYLE_READ_COLOR(QJsonObject values, QColor& variable, std::string_view varname)
{
    auto valueRef = values[varname.data()];
    if (valueRef.isArray())
    {
        auto colorArray = valueRef.toArray();
        std::array<int, 3> rgb;
        for (size_t i = 0; const auto & it : colorArray) {
            rgb[i] = it.toInt();
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