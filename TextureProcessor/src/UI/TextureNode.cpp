#include <UI/TextureNode.h>
#include <UI/Connection.h>
#include <UI/Properties.h>
#include <UI/Sliders.h>
#include <QLabel>
#include <QFileDialog>


UI::TextureNode::TextureNode(QJsonObject document, std::string_view name, Engine& engine)
	:Node(document, name), model(document, engine), texture(128, 128, QImage::Format::Format_ARGB32)
{
	for (auto& s : model.GetSinks())
		sinks.push_back(s->GetType());
	for (auto& s : model.GetSources())
		sources.push_back(s->GetType());
	CalculateSize({ 128,128 });
}

void UI::TextureNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Node::paint(painter, option);
	DrawTexture(painter);
}
void UI::TextureNode::DrawTexture(QPainter* painter)
{
	QPointF point{ body_size.width() / 2 - 64, EffectiveHeight() / 2 - 64 + NodeStyle::title_height + NodeStyle::item_padding };
	texture = model.Update().scaled(texture.size());
	painter->drawImage(point, texture);
}

void UI::TextureNode::Update()
{
	model.Update();
	Node::Update();
	update();
}
void UI::TextureNode::UpdateProperties(Properties& props)
{
	auto& buf = model.GetBuffer();
	if (!buf)return;

	PropertyElement prop{*this, model.GetName()};
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
			break;
		case ver::dc::Type::Integer:
			break;
		default:
			break;
		}
	}
	props.AppendProperty(std::move(prop));
}

std::string UI::TextureNode::Export()
{
	auto str = QFileDialog::getSaveFileName(nullptr,
		"Export As",
		"",
		"All files (*.*);;BMP (*.bmp);;CUR (*.cur);;GIF (*.gif);;ICNS (*.icns);;ICO (*.ico);;JPEG (*.jpeg);;JPG (*.jpg);;PBM (*.pbm);;PGM (*.pgm);;PNG (*.png);;PPM (*.ppm);;SVG (*.svg);;SVGZ (*.svgz);;TGA (*.tga);;TIF (*.tif);;TIFF (*.tiff);;WBMP (*.wbmp);;WEBP (*.webp);;XBM (*.xbm);;XPM (*.xpm)"
	);
	model.Update().save(str);
	return str.toStdString();
}
void UI::TextureNode::ExportSilent(std::string_view hint)
{
	model.Update().save(hint.data());
}

