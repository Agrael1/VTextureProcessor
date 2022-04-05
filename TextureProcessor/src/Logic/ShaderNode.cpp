/**
 * @file ShaderNode.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief ShaderNode model logic
 */
#include <ranges>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>

#include <Logic/ShaderNode.h>
#include <Logic/SourcesT.h>
#include <Logic/SinksT.h>
#include <Logic/Engine.h>
#include <utils/utils.h>

#include <UI/PropertyGenerator.h>

using namespace ver;


ver::ShaderNode::ShaderNode(TextureDescriptor& td)
	:desc(td), Node(std::format("{}_{}", td.style.StyleName().toStdString(), td.use_count())), buf(td.buffer, td.params)
{
	sinks.reserve(desc.sinks.size());
	sources.reserve(desc.sources.size());

	inputs.reserve(desc.sinks.size());
	outputs.reserve(desc.sources.size());

	// Copies and registers sources
	for (auto&& s : desc.sources | std::views::filter([](const ver::PortDesc& pd) ->bool {return any(pd.type); }))
	{
		auto r = std::make_shared<QImage>();
		RegisterSource(DirectTextureSource::Make(s.name.toStdString(),
			outputs.emplace_back(std::move(r)), s.type));
	}

	// Copies and registers sinks
	for (auto& s : desc.sinks | std::views::filter([](const ver::PortDesc& pd) ->bool {return any(pd.type); }))
	{
		RegisterSink(DirectTextureSink::Make(s.name.toStdString(),
			inputs.emplace_back(), s.type));
	}
}

/**
 * @brief Called on input change
 *
 * @return QImage Generated texture preview
 */
void ver::ShaderNode::Update()
{
	Engine::Instance().Render(desc.shader, inputs, tiling, outputs, buf);
}

void ver::ShaderNode::GetProperties(UI::Windows::PropertyElement& props)
{
	UI::PropertyBuffer(props, buf, desc.params);

}

QJsonObject ver::ShaderNode::Serialize()
{
	QJsonObject node = Node::Serialize();
	node.insert("Tiling", tiling);
	node.insert("BufferState", buffer);

	QJsonObject buffer;
	for (auto&& x : buf)
		buffer.insert(x.GetName().data(), QJsonValue::fromVariant(x.ToVariant()));
	node.insert("Buffer", buffer);
	return node;
}

void ver::ShaderNode::Deserialize(QJsonObject in)
{
	Node::Deserialize(in);
	if (in.contains("Buffer"))
	{
		auto v = in["Buffer"].toObject();
		auto keys = v.keys();
		for (const auto& k : keys)
		{
			auto sk = k.toStdString();
			buf[sk].SetIfExists(v[k].toVariant());
		}
	}
	if (in.contains("Tiling"))
	{
		tiling = in["Tiling"].toBool(false);
	}
	if (in.contains("Buffer"))
	{
		buffer = in["Buffer"].toBool(true);
	}
}

void ver::ShaderNode::ExportSilent(std::string_view name)
{}
std::string ver::ShaderNode::Export()
{
	auto str = QFileDialog::getSaveFileName(nullptr,
		"Export As",
		"",
		"PNG (*.png);;BMP (*.bmp);;CUR (*.cur);;GIF (*.gif);;ICNS (*.icns);;ICO (*.ico);;JPEG (*.jpeg);;JPG (*.jpg);;PBM (*.pbm);;PGM (*.pgm);;PPM (*.ppm);;SVG (*.svg);;SVGZ (*.svgz);;TGA (*.tga);;TIF (*.tif);;TIFF (*.tiff);;WBMP (*.wbmp);;WEBP (*.webp);;XBM (*.xbm);;XPM (*.xpm);;All files (*.*);;"
	);
	if (str.isEmpty())return"";

	std::ranges::for_each(outputs, [&str](auto& in) {in->mirrored().save(str); });
	return str.toStdString();
}

ver::TextureDescriptor::TextureDescriptor(QJsonObject document, std::string_view styleName)
	:style(document, styleName)
{
	auto node = document["Node"].toObject();

	if (node.contains("Properties"))
		SetProperties(node["Properties"].toArray());

	auto xsinks = node["Sinks"].toArray();
	sinks.reserve(xsinks.size());
	// Register all sinks
	for (auto it : xsinks)
	{
		auto sink = it.toObject();
		auto type = from_str(sink["Type"].toString().toStdString());
		auto xname = sink["Name"].toString();
		auto sname = xname.toStdString();
		auto& s = sinks.emplace_back();
		s.name = std::move(xname);
		s.type = type;
	}

	auto xsources = node["Sources"].toArray();
	sources.reserve(xsources.size());

	// Register all sources
	for (auto it : xsources)
	{
		auto source = it.toObject();
		auto type = from_str(source["Type"].toString().toStdString());
		auto xname = source["Name"].toString();
		auto sname = xname.toStdString();
		auto& s = sources.emplace_back();
		s.name = std::move(xname);
		s.type = type;
	}

	auto val = document["Value"];

	for (auto x : val.toArray())
		shader_body += x.toString();

	Assemble();
}

std::unique_ptr<Node> ver::TextureDescriptor::MakeModel()
{
	refcount++;
	return std::make_unique<ShaderNode>(*this);
}

bool ver::TextureDescriptor::Assemble()
{
	QString xshader{ "#version 420 \n" };
	for (auto& i : buffer.Get())
		xshader += std::format("uniform {} {};\n",
			i.second.GetSignature().data(), i.first).c_str();

	for (auto i = 0; i < sinks.size(); i++)
		xshader += std::format("layout(binding = {})uniform sampler2D {};\n", i, sinks[i].name.toStdString()).c_str();
	for (auto i = 0; i < sources.size(); i++)
		xshader += std::format("layout(location = {})out vec4 {};\n", i, sources[i].name.toStdString()).c_str();
	xshader += "in vec2 sv_texc;\n"; //load texture coordinates
	xshader += shader_body;
	return CompileShader(xshader);
}

void ver::TextureDescriptor::SetProperties(const QJsonArray& props)
{
	for (size_t i = 0; auto it : props)
	{
		auto p = it.toObject();
		auto type = dc::LayoutElement{ p["Type"].toString().toStdString() };
		buffer += { p["Tag"].toString().toStdString(), type};
		if (p.contains("Val"))
		{
			auto& r = params.emplace_back(type.Get());
			r.index = i;
			if (p["Val"].isObject())
				SetOptions(p["Val"].toObject(), r);
			else
			{
				r.enable_def = 1;
				r.param.set_default(p["Val"].toVariant());
			}
		}
		i++;
	}
}
void ver::TextureDescriptor::SetOptions(QJsonObject obj, dc::Options& opt)
{
	auto def = obj.find("default");
	if (opt.enable_def = def != obj.end())
		opt.param.set_default(def->toVariant());
	auto min = obj.find("min");
	if (opt.enable_min = min != obj.end())
		opt.param.set_min(min->toVariant());
	auto max = obj.find("max");
	if (opt.enable_max = max != obj.end())
		opt.param.set_max(max->toVariant());
}
