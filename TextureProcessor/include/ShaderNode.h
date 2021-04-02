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
		ShaderNode(const ShaderNode& other);
	public:
	private:
		std::vector<std::shared_ptr<QOpenGLTexture>> outputs;
		std::shared_ptr<NodePrivate> shader;
	};
}

