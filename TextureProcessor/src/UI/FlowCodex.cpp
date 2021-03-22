#include <UI/FlowCodex.h>


FlowCodex::FlowCodex(QJsonDocument nodes)
{
	QJsonObject topLevelObject = nodes.object();
	for (auto key : topLevelObject)
	{
		QJsonObject x = key.toObject();
		codex.emplace(key.toString().toStdWString(), key.toObject());
	}

}