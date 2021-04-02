#include <UI/FlowScene.h>
#include <fmt/printf.h>

constexpr std::string_view y = R"({
	"Square": {
		"Node": {
			"Group": "Shapes",
			"Sources": [{"Name": "Shape", "Type": "Grayscale"}],
			"Sinks": []
		},
		"NodeStyle": {
			"TitleColor":[128,0,0],
			"FontColor" : "white"
		},
		"Value":["varying lowp vec4 col;\n",
				"void main() {\n",
				"   gl_FragColor = col;\n",
				"}\n"]
	},
	
	"Example2": {
		"Node": {
			"Group": "Example",
			"Sources": [{"Name": "Source1", "Type": "Grayscale"}, {"Name": "Source2", "Type": "Grayscale"}],
			"Sinks": [{"Name": "Sink1", "Type": "Grayscale"}, {"Name": "Sink2", "Type": "Grayscale"}]
		},
		"NodeStyle": {
			"TitleColor": "green",
			"FontColor" : "white"
		},
		"Value":["varying lowp vec4 col;\n",
				"void main() {\n",
				"   gl_FragColor = col;\n",
				"}\n"]
	},
	
	"Example3": {
		"Node": {
			"Group": "Example2",
			"Sources": [{"Name": "Source1", "Type": "Grayscale"}, {"Name": "Source2", "Type": "Grayscale"}],
			"Sinks": [{"Name": "Sink1", "Type": "Grayscale"}, {"Name": "Sink2", "Type": "Grayscale"}]
		},
		"NodeStyle": {
			"TitleColor": "cyan",
			"FontColor" : "white"
		},
		"Value":["varying lowp vec4 col;\n",
				"void main() {\n",
				"   gl_FragColor = col;\n",
				"}\n"]
	}
})";

QJsonParseError e;

using namespace UI;

FlowScene::FlowScene(QObject* parent)
	:QGraphicsScene(parent)
	, Cbackground("#393939")
	, Clight("#2F2F2F")
	, Cdark("#292929")
	, Plight(Clight)
	, Pdark(Cdark)
	, Bbackground(Cbackground)
	, codex(QJsonDocument::fromJson(y.data(),&e))
{
	Plight.setWidth(0);
	Pdark.setWidth(0);
	setBackgroundBrush(Bbackground);
	setItemIndexMethod(QGraphicsScene::NoIndex);
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

UI::Node& FlowScene::CreateNode(std::string_view name)
{
	auto r = codex.MakeNode(name);
	auto x = nodes.emplace(std::piecewise_construct,
		std::forward_as_tuple(fmt::sprintf("%s_%zu",name, r.second)),
		std::forward_as_tuple(r.first)
	);
	x.first->second.model.SetUniqueName(x.first->first);
	addItem(&x.first->second);
	return x.first->second;
}

void FlowScene::Clear()
{
	nodes.clear();
	codex.ClearCounts();
	clear();
}
