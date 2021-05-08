#include <UI/FlowScene.h>
#include <fmt/printf.h>
#include <UI/Properties.h>
#include <UI/Connection.h>

std::filesystem::path generate(const std::filesystem::path& p)
{
	namespace fs = std::filesystem;
	std::filesystem::path rp{ p };
	auto f = rp.filename().replace_extension().string();
	auto x = rp.extension().string();
	f += "_";
	auto v = 0;

	while (fs::exists(rp))
		rp.replace_filename(f + std::to_string(v++) + x);
	return rp;
}

using namespace UI;

FlowScene::FlowScene(QObject* parent, Properties& props)
	:QGraphicsScene(parent)
	, Cbackground("#393939")
	, Clight("#2F2F2F")
	, Cdark("#292929")
	, Plight(Clight)
	, Pdark(Cdark)
	, Bbackground(Cbackground)
	, props(props)
{
	Plight.setWidth(0);
	Pdark.setWidth(0);
	setBackgroundBrush(Bbackground);
	setItemIndexMethod(QGraphicsScene::NoIndex);
	connect(this, &QGraphicsScene::selectionChanged, this, &FlowScene::OnSelectionChanged);
}

void FlowScene::drawBackground(QPainter* painter, const QRectF& rect)
{
	// call parent method
	QGraphicsScene::drawBackground(painter, rect);

	// augment the painted with grid
	const int gridsize = 20;
	auto left = static_cast<int>(std::floor(rect.left()));
	auto right = static_cast<int>(std::ceil(rect.right()));
	auto top = static_cast<int>(std::floor(rect.top()));
	auto bottom = static_cast<int>(std::ceil(rect.bottom()));

	// compute indices of lines to draw
	const auto first_left = left - (left % gridsize);
	const auto first_top = top - (top % gridsize);

	// compute lines to draw and
	std::vector<QLine> lines_light;
	std::vector<QLine> lines_dark;

	for (auto x = first_left; x <= right; x += gridsize) {
		if (x % 100 != 0)
			lines_light.push_back(QLine(x, top, x, bottom));
		else
			lines_dark.push_back(QLine(x, top, x, bottom));
	}
	for (auto y = first_top; y <= bottom; y += gridsize) {
		if (y % 100 != 0)
			lines_light.push_back(QLine(left, y, right, y));
		else
			lines_dark.push_back(QLine(left, y, right, y));
	}

	// draw calls
	painter->setPen(Plight);
	painter->drawLines(lines_light.data(), int(lines_light.size()));

	painter->setPen(Pdark);
	painter->drawLines(lines_dark.data(), int(lines_dark.size()));
}
void FlowScene::OnSelectionChanged()
{
	props.Clear();
	for (auto* i : selectedItems())
	{
		auto* focus = dynamic_cast<TextureNode*>(i);
		if (focus) { focus->UpdateProperties(props); }
	}
	props.Set();
}

UI::Node& FlowScene::CreateNode(std::string_view name)
{
	auto &node = InsertNode(name, fmt::sprintf("%s_%zu", name, codex.AddRef(name)));
	addItem(&node);
	if (name == "Output")
		outputs.push_back(&node);
	return node;
}
UI::Node* FlowScene::LocateNode(QPointF pos)noexcept
{
	auto xitems = items(pos);

	std::vector<QGraphicsItem*> filteredItems;
	std::copy_if(xitems.begin(), xitems.end(), std::back_inserter(filteredItems),
		[](QGraphicsItem* item)
		{
			return (dynamic_cast<UI::Node*>(item) != nullptr);
		});
	if (filteredItems.empty())
		return nullptr;

	return static_cast<UI::Node*>(filteredItems.front());
}
UI::Node& FlowScene::InsertNode(std::string_view name, std::string&& unique_name)
{
	const auto &r = codex.GetNode(name);
	auto x = nodes.emplace(std::piecewise_construct,
		std::forward_as_tuple(std::move(unique_name)),
		std::forward_as_tuple(r)
	);
	x.first->second->SetUniqueName(x.first->first);
	return *x.first->second;
}

void FlowScene::DeleteSelected()
{
	for (QGraphicsItem* item : selectedItems())
	{
		if (auto c = dynamic_cast<Connection*>(item))
			c->RemoveForce();
	}
	for (QGraphicsItem* item : selectedItems())
	{
		if (auto n = dynamic_cast<Node*>(item))
		{
			if (n->GetStyleName() == "Output")
				outputs.erase(std::find(outputs.begin(), outputs.end(), n));
			nodes.erase(n->GetName().data());
		}
	}
}
void FlowScene::Clear()
{
	nodes.clear();
	codex.ClearCounts();
	clear();
}
void FlowScene::ExportAll()
{
	std::filesystem::path name;
	for (auto* x : outputs)
	{
		if (name.empty())
		{
			name = x->Export();
			continue;
		}
		x->ExportSilent(generate(name).string());
	}
}

QJsonObject FlowScene::Serialize()
{
	QJsonObject sc;
	QJsonArray xnodes;
	QJsonArray conns;

	for (auto& x : nodes)
	{
		xnodes.append(x.second->Serialize());
		auto* node = &*(x.second);
		for (auto* c : ConnMapper::Get(node))
			conns.append(c->Serialize());
	}
	sc.insert("Nodes", xnodes);
	sc.insert("Connections", conns);

	return sc;
}
void FlowScene::Deserialize(QJsonObject obj)
{
	if (!obj.contains("Nodes"))return;
	QJsonArray arr{ obj["Nodes"].toArray() };
	for (auto ref : arr)
	{
		QJsonObject obj{ ref.toObject() };
		auto stype = obj.keys()[0];
		auto type = stype.toStdString();
		auto node = obj[stype].toObject();
		if (!node.contains("Ref"))continue;
		auto xref = node["Ref"].toInt();

		auto& xnode = InsertNode(type, fmt::sprintf("%s_%zu", type, xref));
		codex.SetMaxRef(type, xref + 1);
		addItem(&xnode);
		if (type == "Output")
			outputs.push_back(&xnode);
		xnode.Deserialize(node);
	}

	if (obj.contains("Connections"))
	{
		QJsonArray conns{ obj["Connections"].toArray() };
		for (auto c : conns)
		{
			auto o = c.toObject();
			if (!(o.contains("Sink") && o.contains("Source")))continue;
			auto source = o["Source"].toArray();
			UI::Node* node = nullptr;
			uint8_t sourceN = 0;

			for (auto v : source)
			{
				if (v.isString())
				{
					auto key = v.toString().toStdString();
					auto xnode = nodes.find(key);
					if (xnode == nodes.end())break;
					node = xnode->second.operator->();
					continue;
				}
				sourceN = v.toInt();
			}
			if (!node) continue;
			ConnMapper::MakeTemporary(*node, Port::Source, sourceN);

			auto sink = o["Sink"].toArray();
			node = nullptr;
			uint8_t sinkN = 0;
			for (auto v : sink)
			{
				if (v.isString())
				{
					auto key = v.toString().toStdString();
					auto xnode = nodes.find(key);
					if (xnode == nodes.end())break;
					node = xnode->second.operator->();
					continue;
				}
				sinkN = v.toInt();
			}
			if (!node) { ConnMapper::ClearTemporary(); continue; }
			ConnMapper::ConnectTemporary(*node, Port::Sink, sinkN);
		}
	}
}