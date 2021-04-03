#include <UI/UINode.h>
#include <ShaderNode.h>

namespace UI
{
	class TextureNode : public Node
	{
	public:
		TextureNode(QJsonObject document, std::string_view name);
	public:
		std::string_view GetName()const noexcept override { return model.GetName(); }
		size_t SourcesCount()const noexcept override { return model.SourcesCount(); }
		size_t SinksCount()const noexcept override { return model.SinksCount(); }
		void paint(QPainter* painter,
			const QStyleOptionGraphicsItem* option,
			QWidget* widget = nullptr) override;
	private:
		void SetUniqueName(std::string_view xname)override{	model.SetUniqueName(xname);}
		void DrawConnectionPoints(QPainter* painter);
		void DrawTexture(QPainter* painter);
	private:
		QImage texture;
		ver::ShaderNode model;
	};
}