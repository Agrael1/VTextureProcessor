#include <UI/UINode.h>
#include <Logic/ShaderNode.h>

class Engine;

namespace UI
{
	class Properties;
	class TextureNode : public Node
	{
	public:
		TextureNode(QJsonObject document, std::string_view name, Engine& engine);
	public:
		std::string_view GetName()const noexcept override { return model.GetName(); }
		void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;
		void UpdateProperties(Properties& properties);
	private:
		void OnConnect(uint8_t sinkN, Node& source, uint8_t sourceN)override
		{
			model.GetSink(sinkN).Bind(source.Model().GetSource(sourceN));
			Update();
		}
		void OnDisconnect(uint8_t sinkN)override
		{
			model.GetSink(sinkN).Unlink();
			Update();
		}
		void Update()override;
		ver::Node& Model()override { return model; }
		void SetUniqueName(std::string_view xname)override { model.SetUniqueName(xname); }
		std::string Export()override;
		void ExportSilent(std::string_view hint)override;

		void DrawTexture(QPainter* painter);
	private:
		QImage texture;
		ver::ShaderNode model;
	};
}