#include <UI/FlowScene.h>
#include <fmt/printf.h>
#include <UI/Properties.h>
#include <UI/Connection.h>
#include <QMessageBox>

/**
 * @brief Generates file name with incremented count if name already exists
 *
 * @param p File path
 * @return std::filesystem::path Final filename
 */
std::filesystem::path generate(const std::filesystem::path& p)
{
	namespace fs = std::filesystem;
	std::filesystem::path rp{ p };
	auto f = rp.filename().replace_extension().string();
	auto x = rp.extension().string();
	f += "_";
	auto v = 0;

	// Increment while file exists
	while (fs::exists(rp))
		rp.replace_filename(f + std::to_string(v++) + x);

	return rp;
}

using namespace UI;

/**
 * @brief Construct a new Flow Scene:: Flow Scene object
 *
 * @param parent
 * @param props
 */
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

/**
 * @brief Draws background grid
 *
 * @param painter Painter object to use
 * @param rect Bounding rectangle to draw in
 */
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

	constexpr int gridSize = 100;
	// Create vertical lines
	for (auto x = first_left; x <= right; x += gridsize) {
		if (x % gridSize != 0)
			lines_light.push_back(QLine(x, top, x, bottom));
		else
			lines_dark.push_back(QLine(x, top, x, bottom));
	}

	// Create vertical lines
	for (auto y = first_top; y <= bottom; y += gridsize) {
		if (y % gridSize != 0)
			lines_light.push_back(QLine(left, y, right, y));
		else
			lines_dark.push_back(QLine(left, y, right, y));
	}

	// Draw vertical and horizontal lines
	painter->setPen(Plight);
	painter->drawLines(lines_light.data(), int(lines_light.size()));

	painter->setPen(Pdark);
	painter->drawLines(lines_dark.data(), int(lines_dark.size()));
}

/**
 * @brief Event to update properties of Nodes on selection change
 *
 */
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

/**
 * @brief Creates new Node on the canvas
 *
 * @param name Name of the new Node
 * @return UI::Node& Newly created node
 */
UI::Node& FlowScene::CreateNode(std::string_view name)
{
	auto &node = InsertNode(name, fmt::sprintf("%s_%zu", name, codex.AddRef(name)));
	addItem(&node);

	// Add to Output nodes if output
	if (name == "Output")
		outputs.push_back(&node);

	return node;
}

/**
 * @brief Locates item that is at the target position
 *
 * @param pos Position to look at
 * @return UI::Node* Found first found object
 */
UI::Node* FlowScene::LocateNode(QPointF pos)noexcept
{
	auto xitems = items(pos);

	// TODO: Refactor?
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

/**
 * @brief Creates new Node of selected type
 *
 * @param name Name of the Node type
 * @param unique_name Name of the new Node (must be unique)
 * @return UI::Node&
 */
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
UI::Node* FlowScene::TryInsertNode(std::string_view name, std::string&& unique_name)
{
	const auto* r = codex.TryGetNode(name);
	if (!r)return nullptr;
	auto x = nodes.emplace(std::piecewise_construct,
		std::forward_as_tuple(std::move(unique_name)),
		std::forward_as_tuple(*r)
	);
	x.first->second->SetUniqueName(x.first->first);
	return &*x.first->second;
}


/**
 * @brief Deletes the object in focus
 *
 */
void FlowScene::DeleteSelected()
{
	// Delete connections first to avoid double free
	for (QGraphicsItem* item : selectedItems())
	{
		// Disconnects object from other scene objects
		if (auto c = dynamic_cast<Connection*>(item))
			c->RemoveForce();
	}

	// Delete Nodes
	for (QGraphicsItem* item : selectedItems())
	{
		if (auto n = dynamic_cast<Node*>(item))
		{
			// Deregister output
			if (n->GetStyleName() == "Output")
				outputs.erase(std::find(outputs.begin(), outputs.end(), n));

			nodes.erase(n->GetName().data());
		}
	}
}

/**
 * @brief Clears all objects on canvas
 *
 */
void FlowScene::Clear()
{
	nodes.clear();
	codex.ClearCounts();
	clear();
}

/**
 * @brief Exports all outputs on canvas
 *
 */
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
		// Exports output silently if name already valid
		x->ExportSilent(generate(name).string());
	}
}

/**
 * @brief Serializes all objects on canvas to JSON
 *
 * @return QJsonObject
 */
QJsonObject FlowScene::Serialize()
{
	/*
	Output JSON is in the following format:

	{
		"Nodes": [
			{ ... node data ... }, ...
		],
		"Connections": [
			{ ... connection data ... }, ...
		]
	}
	*/
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

/**
 * @brief
 *
 * @param obj
 */
void FlowScene::Deserialize(QJsonObject xobj)
{
	// Nothing to draw if no Nodes
	if (!xobj.contains("Nodes")) return;

	bool missing = false;

	QJsonArray arr = xobj["Nodes"].toArray();
	for (auto ref : arr)
	{
		QJsonObject obj = ref.toObject();

		if(obj.isEmpty())continue;
		auto stype = obj.keys().first();
		auto type = stype.toStdString();
		auto node = obj[stype].toObject();

		// Skip if Node is not uniquely identifiable
		if (!node.contains("Ref")) continue;
		auto xref = node["Ref"].toInt();

		// Create unique name from Ref and Type
		auto* xnode = TryInsertNode(type, fmt::sprintf("%s_%zu", type, xref));
		if (!xnode) { missing = true; continue; }

		codex.SetMaxRef(type, xref + 1);
		addItem(xnode);

		// Register output
		if (type == "Output")
			outputs.push_back(xnode);

		// Load config from JSON into the new Node
		xnode->Deserialize(node);
	}

	if (!xobj.contains("Connections")) return;

	QJsonArray conns = xobj["Connections"].toArray();
	for (auto c : conns)
	{
		auto o = c.toObject();
		// Skip incomplete connections
		if (!(o.contains("Sink") && o.contains("Source"))) continue;
		auto source = o["Source"].toArray();
		UI::Node* node = nullptr;
		uint8_t sourceN = 0;

		// Lookup source index
		for (auto v : source)
		{
			if (v.isString())
			{
				auto key = v.toString().toStdString();
				auto xnode = nodes.find(key);
				if (xnode == nodes.end()) break;
				node = xnode->second.operator->();
				continue;
			}
			sourceN = v.toInt();
		}

		if (!node) continue;
		// Similar to manual drag and drop connection
		ConnMapper::MakeTemporary(*node, Port::Source, sourceN);

		auto sink = o["Sink"].toArray();
		node = nullptr;
		uint8_t sinkN = 0;
		// Lookup sink index
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

	if (missing)
		QMessageBox{ QMessageBox::Warning, "Warning", "Some nodes were missing, because their type was not loaded properly",
		QMessageBox::Ok}.exec();
}
