/**
 * @file ShaderNode.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief ShaderNode model logic
 */
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>

#include <Logic/ShaderNode.h>
#include <Logic/SourcesT.h>
#include <Logic/SinksT.h>
#include <Logic/Engine.h>
#include <Logic/ShaderProbe.h>
#include <utils/utils.h>

#include <Windows/Properties.h>
#include <unordered_set>
#include <ranges>


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
	Engine::Instance().Render(desc.shader, inputs, outputs, buf);
}

void ver::ShaderNode::GetProperties(UI::Windows::PropertyElement& props)
{
	props.AppendBuffer(buf, desc.params);
}

void ver::ShaderNode::Serialize(QJsonObject& doc)
{
	QJsonObject buffer;
	for (auto&& x : buf)
		buffer.insert(x.GetName().data(), QJsonValue::fromVariant(x.ToVariant()));
	doc.insert(u"Buffer", buffer);
	doc.insert(u"Type", desc.style.StyleName());
	doc.insert(u"Name", GetName().data());
}

bool ver::ShaderNode::Deserialize(QJsonObject in)
{
	if (in.contains(u"Buffer"))
	{
		auto v = in[u"Buffer"].toObject();
		auto keys = v.keys();
		for (const auto& k : keys)
		{
			auto sk = k.toStdString();
			buf[sk].SetIfExists(v[k].toVariant());
		}
	}
	return true;
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


#include <Editor/NodeParser.h>
#include <Logic/CodeTransformer.h>

void ver::ShaderNode::Accept(ver::ShaderProbe& probe)
{
	std::unordered_map<std::wstring, input_info> inputs;
	for (auto& i : sinks)
	{
		if (!i)continue;
		probe.ReadNode(i->GetOutputNodeName().data());

		auto on = i->GetOutputNodeName();
		auto ou = i->GetSourceName();

		input_info ii;
		ii.node = std::wstring{ on.begin(), on.end() };
		ii.complex = probe.IsComplex(on);
		ii.output = std::wstring{ ou.begin(), ou.end() };

		auto r = i->GetRegisteredName();
		inputs.emplace(std::wstring{ r.begin(), r.end() },std::move(ii));
	}

	std::unordered_set<std::wstring> outs;
	for (auto&& i : sources)
	{
		auto r = i->GetName();
		outs.emplace(std::wstring{ r.begin(), r.end() });
	}

	if (sources.size() > 1)
		probe.AddDesc(&desc);

	std::unordered_map<std::wstring, std::wstring> cbuffer;
	for (auto&& i : buf)
	{
		auto r = i.GetName();
		cbuffer.emplace(std::wstring{ r.begin(), r.end() }, i.to_wstring());
	}

	auto n = GetName();
	std::wstring node_name{ n.begin(), n.end() };

	auto code = desc.shader_body.toStdWString();
	NodeParser np(code);
	np.Parse();
	auto rcode = CodeTransformer(np.GetTypesInfo(), np.GetMacrosInfo(), np.GetFuncsInfo())
		.SetCBuf(cbuffer)
		.SetOutputs(outs)
		.SetInputs(inputs)
		.Transform(node_name, code);

	probe.RegisterNode(n, sources.size() > 1);
	probe.AddShader(std::move(rcode));
}

//////////////////////////////////////////////////////////////////////////////////////////////////

ver::TextureDescriptor::TextureDescriptor(QJsonObject node)
	:style(node)
{
	if (node.contains(u"Properties"))
		SetProperties(node[u"Properties"].toArray());

	auto xsinks = node[u"Sinks"].toArray();
	sinks.reserve(xsinks.size());
	// Register all sinks
	for (auto it : xsinks)
	{
		auto sink = it.toObject();
		auto type = from_str(sink[u"Type"].toString());
		auto& s = sinks.emplace_back();
		s.name = sink[u"Name"].toString();
		s.type = type;
	}

	auto xsources = node[u"Sources"].toArray();
	sources.reserve(xsources.size());

	// Register all sources
	for (auto it : xsources)
	{
		auto source = it.toObject();
		auto type = from_str(source[u"Type"].toString());
		auto& s = sources.emplace_back();
		s.name = source[u"Name"].toString();
		s.type = type;
	}

	auto val = node[u"Value"];

	if (val.isArray())
		for (auto x : val.toArray())
			shader_body += x.toString();
	else
		shader_body = val.toString();
	Assemble();
}

std::unique_ptr<Node> ver::TextureDescriptor::MakeModel()
{
	refcount++;
	return std::make_unique<ShaderNode>(*this);
}

void ver::TextureDescriptor::Assemble()
{
	QString xshader{ QStringLiteral("#version 420 \n") };
	for (auto& i : buffer.Get())
		xshader += std::format("uniform {} {};\n",
			i.second.GetSignature().data(), i.first).c_str();

	for (auto i = 0; i < sinks.size(); i++)
		xshader += std::format("layout(binding = {})uniform sampler2D {};\n", i, sinks[i].name.toStdString()).c_str();
	for (auto i = 0; i < sources.size(); i++)
		xshader += std::format("layout(location = {})out vec4 {};\n", i, sources[i].name.toStdString()).c_str();
	xshader += u"in vec2 sv_texc;\n"; //load texture coordinates
	//xshader += shader_body;
	if (!CompileShader(xshader + shader_body))
	{
		last_error = shader.log();
		CompileShader(xshader + QStringLiteral("void main(){}"));
	}
}
void ver::TextureDescriptor::SetProperties(const QJsonArray& props)
{
	for (auto it : props)
	{
		auto p = it.toObject();
		auto type = dc::LayoutElement{ p[u"Type"].toString().toStdString() };
		buffer += { p[u"CName"].toString().toStdString(), type};
		auto& opt = params.emplace_back(type.Get());

		auto al = p.find(u"Tag");
		if (opt.enable_alias = al != p.end())
			opt.alias = al->toString().toStdString();

		if (p.contains(u"Val"))
		{
			if (p[u"Val"].isObject())
				SetOptions(p[u"Val"].toObject(), opt);
			else
			{
				opt.enable_def = 1;
				opt.param.set_default(p[u"Val"]);
			}
		}
	}
}



bool ver::TextureDescriptor::valid() const noexcept {
	std::unordered_set<QStringView> distinct;
	std::ranges::for_each(sinks, [&distinct](const ver::PortDesc& a) {distinct.emplace(a.name); });
	if (distinct.size() != sinks.size())
	{
		last_error = QStringLiteral("Sink names are not unique");
		return false;
	}
	distinct.clear();
	std::ranges::for_each(sources, [&distinct](const ver::PortDesc& a) {distinct.emplace(a.name); });
	if (distinct.size() != sources.size()) {
		last_error = QStringLiteral("Sources names are not unique"); return false;
	}
	if (!buffer.is_distinct()) {
		last_error = QStringLiteral("Buffer Layout is not distinct"); return false;
	}
	return shader.isCompiled();
}
void ver::TextureDescriptor::SetOptions(QJsonObject obj, dc::Options& opt)
{
	auto def = obj.find(u"default");
	if (opt.enable_def = def != obj.end())
		opt.param.set_default(*def);
	auto min = obj.find(u"min");
	if (opt.enable_min = min != obj.end())
		opt.param.set_min(*min);
	auto max = obj.find(u"max");
	if (opt.enable_max = max != obj.end())
		opt.param.set_max(*max);
}

