#pragma once
#include <Node.h>

namespace ver
{
	class ShaderNode : public Node
	{
		struct NodePrivate
		{
			NodePrivate(QString&& code);
			QOpenGLShader shader;
			std::string shadercode;
		};
	public:
		ShaderNode(QJsonObject document);
	public:
	private:
		std::vector<QOpenGLTexture> outs;
		std::shared_ptr<NodePrivate> shader;
	};
}

