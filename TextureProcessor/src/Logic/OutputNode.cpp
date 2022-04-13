#include <Logic/OutputNode.h>
#include <Logic/SinksT.h>

#include <QFileDialog>

constexpr uint32_t nullcolor = 0;



ver::OutputNode::OutputNode(size_t ref)
	:Node(std::format("{}_{}", "Output", ref))
	,out(std::make_shared<QImage>())
{
	style.SetStyleName(QStringLiteral(u"Output"));
	RegisterSink(DirectTextureSink::Make("Out", in, PortType::Grayscale));
}

void ver::OutputNode::Update()
{
	if (in)
		*out = *in;
	else
		*out = QImage((const uchar*)(&nullcolor), 1, 1, QImage::Format::Format_ARGB32);
}

std::string ver::OutputNode::Export()
{
	auto str = QFileDialog::getSaveFileName(nullptr,
		QStringLiteral("Export As"),
		"",
		QStringLiteral("PNG (*.png);;BMP (*.bmp);;CUR (*.cur);;GIF (*.gif);;ICNS (*.icns);;ICO (*.ico);;JPEG (*.jpeg);;JPG (*.jpg);;PBM (*.pbm);;PGM (*.pgm);;PPM (*.ppm);;SVG (*.svg);;SVGZ (*.svgz);;TGA (*.tga);;TIF (*.tif);;TIFF (*.tiff);;WBMP (*.wbmp);;WEBP (*.webp);;XBM (*.xbm);;XPM (*.xpm);;All files (*.*);;")
	);
	if (str.isEmpty())return"";
	out->mirrored().save(str);
	return str.toStdString();
}

void ver::OutputNode::ExportSilent(std::string_view name)
{
	if (name.empty())return;
	out->save(name.data());
}

void ver::OutputNode::Serialize(QJsonObject& doc)
{
	doc.insert(u"Type", QStringLiteral("Output"));
	doc.insert(u"Name", GetName().data());
}

std::unique_ptr<ver::Node> ver::OutputDescriptor::MakeModel()
{
	return std::make_unique<OutputNode>(refcount++);
}