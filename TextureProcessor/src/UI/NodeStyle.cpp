/**
 * @file NodeStyle.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief Node header color specification
 */

#include <UI/NodeStyle.h>
#include <QJsonArray>
#include <array>



using namespace UI;

const ConnectionStyle ConnectionStyle::Grayscale;


/**
 * @brief Loads color from color map based on name
 *
 * @param values All mapped colors
 * @param varname Target color name
 * @return QColor Found target color
 */
static inline QColor ReadColor(QJsonValueRef value) noexcept
{
	if (value.isString())
		return value.toString();

	if (value.isArray())
	{
		auto colorArray = value.toArray();
		std::array<int, 3> rgb;
		for (size_t i = 0; i < 3; i++)
			rgb[i] = colorArray[i].toInt();
		return { rgb[0], rgb[1], rgb[2] };
	}
	return Qt::black;
}

/**
 * @brief Gets Node style based from document on name
 *
 * @param document Node JSON data
 * @param name Name of the Node style
 */
NodeStyle::NodeStyle(QJsonObject document)
{
	Deserialize(document);
}

void UI::NodeStyle::Serialize(QJsonObject& doc)
{
	doc.insert(u"TitleColor", brTitle.color().name());
	doc.insert(u"FontColor", font_color.name());
}

bool UI::NodeStyle::Deserialize(QJsonObject doc)
{
	if (!doc.contains(u"Name"))return false;
	styleName = doc[u"Name"].toString();
	if (doc.contains(u"TitleColor"))
		brTitle.setColor(ReadColor(doc[u"TitleColor"]));
	if (doc.contains(u"TitleColor"))
		font_color = ReadColor(doc[u"FontColor"]);
	return true;
}
