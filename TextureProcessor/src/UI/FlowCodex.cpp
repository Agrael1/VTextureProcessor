#include <UI/FlowCodex.h>


FlowCodex::FlowCodex(QJsonDocument nodes)
{
	QJsonObject topLevelObject = nodes.object();

	for (auto key : topLevelObject.keys())
	{
		QJsonObject obj = topLevelObject[key].toObject();
		auto wkey = key.toStdWString();
		codex.emplace(wkey, obj);

		auto x = obj["Node"].toObject()["Group"].toString().toStdWString();
		cats[x].emplace_back(wkey);
	}
}