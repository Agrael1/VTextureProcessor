#include <UI/FlowScene.h>
#include <fmt/printf.h>
#include <UI/Properties.h>

constexpr std::string_view y = R"({
	"Square": {
		"Node": {
			"Class": "Texture",
			"Group": "Shapes",
			"Sources": [{"Name": "Shape", "Type": "Grayscale"}],
			"Sinks": []
		},
		"NodeStyle": {
			"TitleColor":[128,0,0],
			"FontColor" : "white"
		},
		"Value":["out vec4 col;\n",
				"void main() {\n",
				"   col = vec4(1.0);\n",
				"}\n"]
	},

	"Dumpster": {
		"Node": {
			"Class": "Texture",
			"Group": "Output",
			"Sinks": [{"Name": "In", "Type": "Grayscale"}]
		},
		"NodeStyle": {
			"TitleColor": "black",
			"FontColor" : "white"
		},
		"Value":["uniform sampler2D u_Sampler;								\n",
				"in vec2 texcoords;\n",
				"out vec4 color;\n",
				"void main() {												\n",
				"    color = texture2D(u_Sampler, texcoords);\n",
				"}"]
	},

	"Triangle": {
		"Node": {
			"Class": "Texture",
			"Group": "Shapes",
			"Properties": [{"Tag": "S", "Type":"float", "Val":0.2}],
			"Sources": [{"Name": "Shape", "Type": "Grayscale"}]
		},
		"NodeStyle": {
			"TitleColor": [128,0,0],
			"FontColor" : "white"
		},
		"Value":["out vec4 col;\n",
				"void main() {\n",
				"   col = vec4(S, S, 0.0, 1.0);\n",
				"}\n"]
	}
})";

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
	auto r = codex.MakeNode(name);
	auto x = nodes.emplace(std::piecewise_construct,
		std::forward_as_tuple(fmt::sprintf("%s_%zu",name, r.second)),
		std::forward_as_tuple(r.first)
	);
	x.first->second->SetUniqueName(x.first->first);
	addItem(&*x.first->second);
	return *x.first->second;
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
			nodes.erase(n->GetName().data());
	}
}
void FlowScene::Clear()
{
	nodes.clear();
	codex.ClearCounts();
	clear();
}
