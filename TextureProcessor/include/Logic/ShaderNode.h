/**
 * @file ShaderNode.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief ShaderNode model logic
 */
#pragma once
#include <Logic/Node.h>
#include <Logic/DynamicConstant.h>
#include <QOpenGLShader>
#include <Logic/PropertyView.h>

class Engine;

namespace ver
{
	class ShaderNode : public Node
	{
		static constexpr auto desc = MakeDesc({
			{DescType::Buffer, "Buffer"},
			{DescType::Boolean, "Tiling"},
			{DescType::BooleanUpd, "Buffer"}
			});
		//Shared value between same type of nodes
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
		QImage XUpdate();
		void Update();
		dc::Buffer& GetBuffer() { return buf; }
		bool& Tiling() { return tiling; }
		std::span<std::shared_ptr<QImage>> GetLayout()noexcept
		{
			return outputs;
		}
		PropertyView GetProperties()override
		{
			PropertyView pv;
			if (buffer)
				pv.TieProp<desc[0]>(buf);
			pv.TieProp<desc[1]>(tiling);
			if(buf)
				pv.TieProp<desc[2]>(buffer);
			return pv;
		}
	private:
		void SetProperties(const QJsonArray& props, QString& scode);
	private:
		std::vector<std::shared_ptr<QImage>> inputs;
		std::vector<std::shared_ptr<QImage>> outputs;
		std::shared_ptr<NodePrivate> shader;
		Engine& e;

		dc::Buffer buf;
		bool tiling = false;
		bool buffer = true;
	};

	class OutputNode : public Node
	{
	public:
		OutputNode(QJsonObject document, Engine& e);
		OutputNode(const OutputNode& in);
	public:
		std::span<std::shared_ptr<QImage>> GetLayout()noexcept
		{
			return { &inout, 1 };
		}
	private:
		std::shared_ptr<QImage> inout;
	};
}

