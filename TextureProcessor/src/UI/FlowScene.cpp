/**
 * @file FlowScene.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Logic behind Node editor canvas
 */
#include <UI/FlowScene.h>
#include <UI/SceneEvent.h>
#include <UI/UINode.h>
#include <UI/Connection.h>

#include <Windows/Properties.h>
#include <QMessageBox>
#include <QPainter>
#include <QGraphicsView>
#include <QJsonArray>

#include <utils/utils.h>
#include <Logic/ShaderNode.h>
#include <Logic/Sink.h>
#include <Logic/Source.h>
#include <ranges>

using namespace UI;


/**
 * @brief Construct a new Flow Scene:: Flow Scene object
 *
 * @param parent
 * @param props
 */
FlowScene::FlowScene(QObject* parent, Windows::Properties& props, QSize dims)
	:QGraphicsScene(parent)
	, Cbackground("#393939")
	, Clight("#2F2F2F")
	, Cdark("#292929")
	, Plight(Clight)
	, Pdark(Cdark)
	, Bbackground(Cbackground)
	, props(props)
	, dims(dims)
{
	Plight.setWidth(0);
	Pdark.setWidth(0);
	setBackgroundBrush(Bbackground);
	setItemIndexMethod(QGraphicsScene::NoIndex);
	connect(this, &QGraphicsScene::selectionChanged, this, &FlowScene::OnSelectionChanged);

	// Create context menu
	auto& m = codex.CatMap();
	for (const auto& x : m)
	{
		menu.AppendGroup(x.first);
		for (const auto& y : x.second)
			menu.AppendItem(y);
	}
	menu.Finish();
	menu.SetItemClickCallback([this](QTreeWidgetItem* item, int) {OnItemSelected(item, 0); });
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
		auto* focus = dynamic_cast<INode*>(i);
		if (focus) focus->UpdateProperties(props.MakeElement(*focus, focus->Name()));
	}
	props.Show();
}

/**
 * @brief Creates new Node on the canvas
 *
 * @param name Name of the new Node
 * @return UI::Node& Newly created node
 */
UI::INode* FlowScene::CreateNode(std::string_view name)
{
	auto x = codex.GetNode(name);
	auto* gx = x.get();
	if (!gx)return gx;
	nodes.emplace(x->Name(), std::move(x));
	addItem(gx);

	// Add to Output nodes if output
	if (name == "Output")
		outputs.push_back(gx);

	return gx;
}

void UI::FlowScene::OnItemSelected(QTreeWidgetItem* item, int)
{
	auto modelName = item->data(0, Qt::UserRole).toString().toStdString();
	if (modelName == ContextMenu::skipper) return;

	auto* type = CreateNode(modelName);
	type->setPos(views()[0]->mapToScene(last_event));
	update();

	menu.close();
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
		if (auto c = dynamic_cast<IConnection*>(item))
			c->RemoveForce();
	}

	// Delete Nodes
	for (NodeUI& item : selectedItems()
		| std::views::filter([](auto* x) {return dynamic_cast<NodeUI*>(x); })
		| std::views::transform([](auto* x) ->NodeUI& {return *static_cast<NodeUI*>(x); }))
	{
		if (item.GetType() == NodeUI::Type::Output)
			outputs.erase(std::find(outputs.begin(), outputs.end(), &item));
		nodes.erase(item.Name().data());
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
		x->ExportSilent(ver::generate(name).string());
	}
}

QSize UI::FlowScene::Dimensions(QJsonObject in) const noexcept
{
	if (dims != QSize{}) return dims;
	if (!in.contains("Dimensions")) return{ 0,0 };
	QJsonArray arr = in["Dimensions"].toArray();
	if (arr.count() != 2) {
		qDebug() << "Bad Dimensions";
		return{ 0,0 };
	}
	return dims = QSize(arr[0].toInt(), arr[1].toInt());
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
	QJsonArray xdims;
	QJsonArray xnodes;
	QJsonArray conns;

	xdims.append(dims.width());
	xdims.append(dims.height());

	for (auto& x : nodes)
	{
		xnodes.append(x.second->Serialize());
		auto* node = &*(x.second);
		for (auto* c : ConnectionMap::Get(*node))
			conns.append(Query(c).Serialize());
	}
	sc.insert("Dimensions", xdims);
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

	std::unordered_map<QString, QString> names;

	bool missing = false;

	QJsonArray arr = xobj["Nodes"].toArray();
	for (auto ref : arr)
	{
		QJsonObject obj = ref.toObject();

		if (obj.isEmpty())continue;
		auto stype = obj.keys().first();
		auto type = stype.toStdString();
		auto node = obj[stype].toObject();

		// Skip if Node is not uniquely identifiable
		if (!node.contains("Ref")) continue;
		auto xref = node["Ref"].toInt();

		// Create unique name from Ref and Type
		auto* xnode = CreateNode(type);
		if (!xnode) { missing = true; continue; }
		names.emplace(stype + node["Ref"].toString(), xnode->Name().data());

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
		UI::INode* node = nullptr;
		uint8_t sourceN = 0;

		// Lookup source index
		for (auto v : source)
		{
			if (v.isString())
			{
				auto xkey = names.find(v.toString());
				if (xkey == names.end()) break;
				auto key = xkey->second.toStdString();
				auto xnode = nodes.find(key);
				if (xnode == nodes.end()) break;
				node = xnode->second.get();
				continue;
			}
			sourceN = v.toInt();
		}

		if (!node) continue;
		// Similar to manual drag and drop connection
		node->StartConnection(sourceN);

		auto sink = o["Sink"].toArray();
		node = nullptr;
		uint8_t sinkN = 0;
		// Lookup sink index
		for (auto v : sink)
		{
			if (v.isString())
			{
				auto xkey = names.find(v.toString());
				if (xkey == names.end()) break;
				auto key = xkey->second.toStdString();
				auto xnode = nodes.find(key);
				if (xnode == nodes.end()) break;
				node = xnode->second.get();
				continue;
			}
			sinkN = v.toInt();
		}
		if (!node) { ConnectionMap::ClearTemporary(); continue; }
		node->FinishConnection(sinkN);
	}

	if (missing)
		QMessageBox{ QMessageBox::Warning, "Warning", "Some nodes were missing, because their type was not loaded properly",
		QMessageBox::Ok }.exec();
}

UI::INode* GetNode(const auto& in)
{
	auto r = std::ranges::find_if(in, [](auto* xin) {return dynamic_cast<INode*>(xin) != nullptr; });
	if (r == in.end())
		return nullptr;
	return static_cast<INode*>(*r);
}


bool UI::FlowScene::event(QEvent* e)
{
	if (e->type() == QEvent::ContextMenu)
	{
		auto& x = static_cast<SceneEvent&>(*e);
		last_event = x.pos();

		if (auto r = GetNode(items(x.scene_pos)); r && r->isSelected())
		{
			nodemenu.Execute(r, x.globalPos());
			return true;
		}

		menu.Execute(x.globalPos());
		return true;
	}
	return QGraphicsScene::event(e);
}
