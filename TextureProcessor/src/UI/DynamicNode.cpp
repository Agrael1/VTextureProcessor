#include <UI/DynamicNode.h>
#include <QGraphicsProxyWidget>
#include <Windows/Properties.h>
#include <UI/PropertyContainer.h>


using namespace UI;

DynamicNode::DynamicNode()
	:base_class({}, "")
{
	l_main = new GraphicsLinearLayout(Qt::Orientation::Horizontal);
	Init();
}

UI::DynamicNode::DynamicNode(const std::filesystem::path& p)
	:base_class(Parse(p))
{
	l_main = new GraphicsLinearLayout(Qt::Orientation::Horizontal);

	sinks.reserve(model.SinksCount());
	sources.reserve(model.SourcesCount());

	for (uint8_t i = 0; i < model.SinksCount(); i++)
		sinks.emplace_back(*this, i, model.GetSink(i));
	for (uint8_t i = 0; i < model.SourcesCount(); i++)
		sources.emplace_back(*this, i, model.GetSource(i));

	Init();
	UpdateLayouts();
}

void UI::DynamicNode::Rename(const QString& name)
{
	style->SetStyleName(name);
	auto& h = Header();
	h.setText(name);
	h.adjustSize();
	h.setMinimumSize(h.size());
	l_central->invalidate();
	Update();
}

std::pair<QJsonObject, std::string> UI::DynamicNode::Parse(const std::filesystem::path& p)
{
	if (p.empty())return{};

	std::ifstream t(p);
	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	QJsonParseError e{};
	// Deserialize loaded JSON
	auto json = QJsonDocument::fromJson(QByteArray::fromStdString(str), &e);
	if (e.error != QJsonParseError::NoError) { qDebug() << e.errorString(); return{}; }

	QJsonObject topLevelObject = json.object();

	auto key = topLevelObject.keys()[0];
	{
		QJsonObject obj = topLevelObject[key].toObject();
		// Name of the node style
		auto wkey = key.toStdString();
		auto node = obj["Node"].toObject();

		// Loads texture Node style
		if (node["Class"].toString() == "Texture")
			return { obj, wkey };
		return {};
	}
}

void UI::DynamicNode::UpdateProperties(Windows::PropertyElement& properties)
{
	properties.AppendWidget<PropertyContainer>(model);
}
