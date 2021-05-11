/**
 * @file TextureNode.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Class for drawing and exporting generated textures
 */

#include <UI/TextureNode.h>
#include <UI/Connection.h>
#include <UI/Properties.h>
#include <UI/Sliders.h>
#include <QLabel>
#include <QFileDialog>
#include <QCheckBox>
#include <charconv>

/**
 * @brief Construct a new UI::TextureNode::TextureNode object
 *
 * @param document JSON specification of the texture node
 * @param name Node name
 * @param engine Rendering engine of the texture node
 */
UI::TextureNode::TextureNode(QJsonObject document, std::string_view name, Engine& engine)
	:Node(document, name), model(document, engine), texture(128, 128, QImage::Format::Format_ARGB32)
{
	// Registers Sinks
	for (auto& s : model.GetSinks())
		sinks.push_back(s->GetType());

	// Registers Sources
	for (auto& s : model.GetSources())
		sources.push_back(s->GetType());

	// Size of the Node
	CalculateSize({ 128,128 });
}

/**
 * @brief Paint event override
 *
 * @param painter Paint device (similar to HDC)
 * @param option [unused]
 * @param widget [unused]
 */
void UI::TextureNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Node::paint(painter, option);
	DrawTexture(painter);
}

/**
 * @brief Draws texture upon node
 * @param painter Handle to device context
*/
void UI::TextureNode::DrawTexture(QPainter* painter)
{
	QPointF point{ body_size.width() / 2 - 64, EffectiveHeight() / 2 - 64 + NodeStyle::title_height + NodeStyle::item_padding };
	texture = model.Update().scaled(texture.size());
	painter->drawImage(point, texture);
}

/**
 * @brief Updates the whole node
*/
void UI::TextureNode::Update()
{
	model.Update();
	Node::Update();
	update();
}

/**
 * @brief Creates properties for dock window
 * @param props reference to properties window (visitor)
*/
void UI::TextureNode::UpdateProperties(Properties& props)
{
	PropertyElement prop{ *this, model.GetName() };
	prop.AppendWidget<CheckBox>(model.Tiling(), "Disable Tiling");

	auto& buf = model.GetBuffer();
	if (!buf)return props.AppendProperty(std::move(prop));


	for (auto ref : buf)
	{
		auto t = ref.GetType();
		switch (t)
		{
		case ver::dc::Type::Float:
			prop.AppendWidget<QLabel>(ref.GetName().data());
			prop.AppendWidget<FloatSlider>((float&)ref);
			break;
		case ver::dc::Type::Float2:
			prop.AppendWidget<QLabel>(ref.GetName().data());
			prop.AppendWidget<Vec2Slider>((QVector2D&)ref);
			break;
		case ver::dc::Type::Float3:
			break;
		case ver::dc::Type::Float4:
			break;
		case ver::dc::Type::Matrix:
			break;
		case ver::dc::Type::Bool:
			prop.AppendWidget<CheckBox>((bool&)ref, ref.GetName().data());
			break;
		case ver::dc::Type::Integer:
			prop.AppendWidget<QLabel>(ref.GetName().data());
			prop.AppendWidget<IntSlider>((int&)ref);
			break;
		default:
			break;
		}
	}
	props.AppendProperty(std::move(prop));
}

/**
 * @brief Exports all output pins as images
 * @return filename chosen
*/
std::string UI::TextureNode::Export()
{
	auto str = QFileDialog::getSaveFileName(nullptr,
		"Export As",
		"",
		"All files (*.*);;BMP (*.bmp);;CUR (*.cur);;GIF (*.gif);;ICNS (*.icns);;ICO (*.ico);;JPEG (*.jpeg);;JPG (*.jpg);;PBM (*.pbm);;PGM (*.pgm);;PNG (*.png);;PPM (*.ppm);;SVG (*.svg);;SVGZ (*.svgz);;TGA (*.tga);;TIF (*.tif);;TIFF (*.tiff);;WBMP (*.wbmp);;WEBP (*.webp);;XBM (*.xbm);;XPM (*.xpm)"
	);
	if (str.isEmpty())return"";
	model.Update().save(str);
	return str.toStdString();
}

/**
 * @brief With specified filename creates images from nodes
 * @param hint  filename hint (will be generated with)
*/
void UI::TextureNode::ExportSilent(std::string_view hint)
{
	if (hint.empty())return;
	model.Update().save(hint.data());
}

/**
 * @brief Overrides ISerializable interface
 * @return JsonObject to save
*/
QJsonObject UI::TextureNode::Serialize()
{
	QJsonObject j;
	QJsonObject node;
	QJsonArray xpos;
	xpos.append(scenePos().x());
	xpos.append(scenePos().y());
	node.insert("Position", xpos);

	auto name = model.GetName();
	auto unders = name.find_last_of('_');
	int ref;
	std::from_chars(name.data() + unders + 1, name.data() + name.size(), ref);
	node.insert("Ref", ref);

	QJsonObject buffer;
	for (auto x : model.GetBuffer())
		buffer.insert(x.GetName().data(), QJsonValue::fromVariant(x.ToVariant()));
	node.insert("Buffer", buffer);

	j.insert(GetStyleName().data(), node);
	return j;
}

/**
 * @brief Unpacks data from json
 * @param in data to unpack
*/
void UI::TextureNode::Deserialize(QJsonObject in)
{
	if (in.contains("Position"))
	{
		auto v = in["Position"].toArray();
		setPos(QPointF{ v[0].toDouble(), v[1].toDouble() });
	}
	if (in.contains("Buffer"))
	{
		auto& buf = model.GetBuffer();
		auto v = in["Buffer"].toObject();
		auto keys = v.keys();
		for (const auto& k : keys)
		{
			auto sk = k.toStdString();
			buf[sk].SetIfExists(v[k].toVariant());
		}
	}
	Update();
}
