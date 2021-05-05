#pragma once
#include <Logic/Node.h>
#include <Logic/DynamicConstant.h>

class Engine;

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
		ShaderNode(QJsonObject document, Engine& e);
		ShaderNode(const ShaderNode& other);
	public:
		QImage Update();
	private:
		void SetProperties(const QJsonArray& props, QString& scode);
	private:
		std::vector<std::shared_ptr<QOpenGLTexture>> inputs;
		std::vector<std::shared_ptr<QOpenGLTexture>> outputs;
		std::shared_ptr<NodePrivate> shader;
		dc::Buffer buf;
		Engine& e;
	};
}

