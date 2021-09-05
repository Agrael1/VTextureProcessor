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
		ShaderNode(QJsonObject document);
		ShaderNode(const ShaderNode& other);
	public:
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
			if (buf)
				pv.TieProp<desc[2]>(buffer);
			return pv;
		}
		virtual QJsonObject Serialize()override
		{
			QJsonObject node;
			node.insert("Tiling", tiling);
			node.insert("BufferState", buffer);

			auto name = GetName();
			auto unders = name.find_last_of('_');
			int ref;
			std::from_chars(name.data() + unders + 1, name.data() + name.size(), ref);
			node.insert("Ref", ref);

			QJsonObject buffer;
			for (auto x : buf)
				buffer.insert(x.GetName().data(), QJsonValue::fromVariant(x.ToVariant()));
			node.insert("Buffer", buffer);
			return node;
		}
		virtual void Deserialize(QJsonObject in)override
		{
			if (in.contains("Buffer"))
			{
				auto v = in["Buffer"].toObject();
				auto keys = v.keys();
				for (const auto& k : keys)
				{
					auto sk = k.toStdString();
					buf[sk].SetIfExists(v[k].toVariant());
				}
			}
			if (in.contains("Tiling"))
			{
				tiling = in["Tiling"].toBool(false);
			}
			if (in.contains("Buffer"))
			{
				buffer = in["Buffer"].toBool(true);
			}
		}
		virtual void ExportSilent(std::string_view name) {};
		std::string Export()override { return ""; };
	private:
		void SetProperties(const QJsonArray& props, QString& scode);
	private:
		std::vector<std::shared_ptr<QImage>> inputs;
		std::vector<std::shared_ptr<QImage>> outputs;
		std::shared_ptr<NodePrivate> shader;

		dc::Buffer buf;
		bool tiling = false;
		bool buffer = true;
	};

	class OutputNode : public Node
	{
	public:
		OutputNode(QJsonObject document);
		OutputNode(const OutputNode& in);
	public:
		std::span<std::shared_ptr<QImage>> GetLayout()noexcept
		{
			return { &inout, 1 };
		}
		void Update() {};
		PropertyView GetProperties()override
		{
			return {};
		}
		std::string Export()override { return ""; };
		virtual void ExportSilent(std::string_view name) {};
	private:
		std::shared_ptr<QImage> inout;
	};
}

