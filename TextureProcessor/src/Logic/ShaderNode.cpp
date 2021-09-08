/**
 * @file ShaderNode.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief ShaderNode model logic
 */
#include <QJsonObject>
#include <QJsonArray>

#include <Logic/ShaderNode.h>
#include <Logic/SourcesT.h>
#include <Logic/SinksT.h>
#include <Logic/Engine.h>
#include <utils/utils.h>

using namespace ver;

constexpr uint32_t nullcolor = 0;

/**
 * @brief Construct a new Shader Node:: Node Private:: Node Private object
 *
 * @param code GLSL source code
 */
ShaderNode::NodePrivate::NodePrivate(QString&& code)
	:shader(QOpenGLShader::Fragment), shadercode(code.toStdString())
{
	if (!shader.compileSourceCode(code))throw RGC_EXCEPTION("Failed to compile shader code");
}

/**
 * @brief Construct a new Shader Node:: Shader Node object
 *
 * @param document JSON specification of the shader node
 * @param e Engine for compiling textures
 */
ShaderNode::ShaderNode(QJsonObject document)
{
	auto node = document["Node"].toObject();
	QString xshader{ "#version 420 \n" };

	if (node.contains("Properties"))
		SetProperties(node["Properties"].toArray(), xshader);

	auto sinks = node["Sinks"].toArray();
	inputs.reserve(sinks.size());
	// Register all sinks
	for (auto it : sinks)
	{
		auto sink = it.toObject();
		auto type = from_str(sink["Type"].toString().toStdString());

		switch (type)
		{
		case PortType::None:
			break;
		case PortType::Grayscale:
		{
			auto sname = sink["Name"].toString().toStdString();
			xshader += std::format("layout(binding = {})uniform sampler2D {};\n", inputs.size(), sname).c_str();
			RegisterSink(GrayscaleSink::Make(sname, inputs.emplace_back()));
		}
		break;
		case PortType::Color:
			break;
		case PortType::Normal:
			break;
		default:
			break;
		}
	}

	auto sources = node["Sources"].toArray();
	outputs.reserve(sources.size());

	// Register all sources
	for (auto it : sources)
	{
		auto source = it.toObject();
		auto type = from_str(source["Type"].toString().toStdString()); //TODO: switch

		switch (type)
		{
		case PortType::None:
			break;
		case PortType::Grayscale:
		{
			auto sname = source["Name"].toString().toStdString();
			xshader += std::format("layout(location={})out vec4 {};\n", outputs.size(), sname).c_str();
			RegisterSource(GrayscaleSource::Make(sname,
				"", outputs.emplace_back(std::make_shared<QImage>())));
			break;
		}
		case PortType::Color:
			break;
		case PortType::Normal:
			break;
		default:
			break;
		}
	}
	xshader += "in vec2 sv_texc;\n"; //load texture coordinates

	auto val = document["Value"];
	for (auto x : val.toArray())
		xshader += x.toString();

	shader = std::make_shared<NodePrivate>(std::move(xshader));
}

/**
 * @brief Construct a new Shader Node:: Shader Node object
 *
 * @param other Shader node to be duplicated (without recompiling shader)
 */
ShaderNode::ShaderNode(const ShaderNode& other)
	:shader(other.shader), buf(other.buf)
{
	sinks.reserve(other.SinksCount());
	sources.reserve(other.SourcesCount());
	outputs.reserve(other.SourcesCount());
	inputs.reserve(other.SinksCount());

	// Copies and registers sources
	for (auto& s : other.sources)
	{
		switch (s->GetType())
		{
		case PortType::Grayscale:
		{
			auto r = std::make_shared<QImage>();
			RegisterSource(GrayscaleSource::Make(s->GetName(),
				shader->shadercode, outputs.emplace_back(std::move(r))));
			break;
		}
		default:
			break;
		}
	}

	// Copies and registers sinks
	for (auto& s : other.sinks)
	{
		switch (s->GetType())
		{
		case PortType::Grayscale:
			RegisterSink(GrayscaleSink::Make(s->GetRegisteredName(),
				inputs.emplace_back()));
			break;
		default:
			break;
		}
	}
}

/**
 * @brief Called on input change
 *
 * @return QImage Generated texture preview
 */
void ver::ShaderNode::Update()
{
	Engine::Instance().Render(shader->shader, inputs, tiling, outputs, buf);
}

void ver::ShaderNode::EPortSilent(std::string_view name) 
{}
std::string ver::ShaderNode::EPort() 
{ 
	auto str = QFileDialog::getSaveFileName(nullptr,
		"EPort As",
		"",
		"PNG (*.png);;BMP (*.bmp);;CUR (*.cur);;GIF (*.gif);;ICNS (*.icns);;ICO (*.ico);;JPEG (*.jpeg);;JPG (*.jpg);;PBM (*.pbm);;PGM (*.pgm);;PPM (*.ppm);;SVG (*.svg);;SVGZ (*.svgz);;TGA (*.tga);;TIF (*.tif);;TIFF (*.tiff);;WBMP (*.wbmp);;WEBP (*.webp);;XBM (*.xbm);;XPM (*.xpm);;All files (*.*);;"
	);
	if (str.isEmpty())return"";

	std::ranges::for_each(outputs, [&str](auto& in) {in->mirrored().save(str); });
	return str.toStdString();
}

/**
 * @brief Sets properties of the shader node
 *
 * @param props List of properties to be added
 * @param scode Source code for dynamic replacement
 */
void ShaderNode::SetProperties(const QJsonArray& props, QString& scode)
{
	if (props.isEmpty())return;
	dc::Layout lay;

	std::vector<std::string> tags;
	std::vector<QVariant> vars;
	for (auto it : props)
	{
		auto p = it.toObject();
		scode += "uniform ";
		auto qtype = p["Type"].toString();
		scode += qtype + " ";
		auto qtag = p["Tag"].toString();
		scode += qtag + ";\n";
		lay += { tags.emplace_back(qtag.toStdString()), dc::LayoutElement{ qtype.toStdString() }};
	}

	buf.Replace(std::move(lay));
	for (size_t i = 0; auto it : props)
	{
		auto p = it.toObject();
		if (p.contains("Val"))
			buf[tags[i]] = p["Val"].toVariant();
		i++;
	}
}

ver::OutputNode::OutputNode(QJsonObject document)
{
	RegisterSink(GrayscaleSink::Make("Out", in));
}

ver::OutputNode::OutputNode(const OutputNode& in)
{
	out = std::make_shared<QImage>();
	RegisterSink(GrayscaleSink::Make("Out", this->in));
}

void ver::OutputNode::Update()
{
	if (in)
		*out = *in;
	else
		*out = QImage((const uchar*)(&nullcolor), 1, 1, QImage::Format::Format_ARGB32);
}

std::string ver::OutputNode::EPort()
{
	auto str = QFileDialog::getSaveFileName(nullptr,
		"EPort As",
		"",
		"PNG (*.png);;BMP (*.bmp);;CUR (*.cur);;GIF (*.gif);;ICNS (*.icns);;ICO (*.ico);;JPEG (*.jpeg);;JPG (*.jpg);;PBM (*.pbm);;PGM (*.pgm);;PPM (*.ppm);;SVG (*.svg);;SVGZ (*.svgz);;TGA (*.tga);;TIF (*.tif);;TIFF (*.tiff);;WBMP (*.wbmp);;WEBP (*.webp);;XBM (*.xbm);;XPM (*.xpm);;All files (*.*);;"
	);
	if (str.isEmpty())return"";
	out->mirrored().save(str);
	return str.toStdString();
}

inline void ver::OutputNode::EPortSilent(std::string_view name)
{
	if (name.empty())return;
	out->save(name.data());
}
