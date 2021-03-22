#include <UI/FlowScene.h>
#include <fmt/printf.h>

constexpr std::string_view json = R"(
{
  "Node": {
    "Group": "Example",
    "Name": "Example",
    "Sources": [{"Name": "Source1", "Type": "Grayscale"}, {"Name": "Source2", "Type": "Grayscale"}],
    "Sinks": [{"Name": "Sink1", "Type": "Grayscale"}, {"Name": "Sink2", "Type": "Grayscale"}],
  },
  "NodeStyle": {
    "NormalBoundaryColor": [255, 255, 255],
    "SelectedBoundaryColor": [255, 165, 0],
    "GradientColor0": "gray",
    "GradientColor1": [80, 80, 80],
    "GradientColor2": [64, 64, 64],
    "GradientColor3": [58, 58, 58],
    "ShadowColor": [20, 20, 20],
    "FontColor" : "white",
    "FontColorFaded" : "gray",
    "ConnectionPointColor": [169, 169, 169],
    "FilledConnectionPointColor": "cyan",
    "ErrorColor": "red",
    "WarningColor": [128, 128, 0],

    "PenWidth": 1.0,
    "HoveredPenWidth": 1.5,

    "ConnectionPointDiameter": 8.0,

    "Opacity": 0.8
  })";


constexpr std::string_view y = R"({
	"Example": {
		"Node": {
			"Group": "Example",
			"Sources": [{"Name": "Source1", "Type": "Grayscale"}, {"Name": "Source2", "Type": "Grayscale"}],
			"Sinks": [{"Name": "Sink1", "Type": "Grayscale"}, {"Name": "Sink2", "Type": "Grayscale"}]
		},
		"NodeStyle": {
			"TitleColor":[128,0,0]
		}
	},
	
	"Example2": {
		"Node": {
			"Group": "Example",
			"Sources": [{"Name": "Source1", "Type": "Grayscale"}, {"Name": "Source2", "Type": "Grayscale"}],
			"Sinks": [{"Name": "Sink1", "Type": "Grayscale"}, {"Name": "Sink2", "Type": "Grayscale"}]
		},
		"NodeStyle": {
			"TitleColor":"green"
		}
	},
	
	"Example3": {
		"Node": {
			"Group": "Example2",
			"Sources": [{"Name": "Source1", "Type": "Grayscale"}, {"Name": "Source2", "Type": "Grayscale"}],
			"Sinks": [{"Name": "Sink1", "Type": "Grayscale"}, {"Name": "Sink2", "Type": "Grayscale"}]
		},
		"NodeStyle": {
			"TitleColor":"cyan"
		}
	}
})";

QJsonParseError e;

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

UI::Node& FlowScene::CreateNode(std::wstring_view name)
{
	return nodes.emplace_back(codex.MakeNode(name));
}