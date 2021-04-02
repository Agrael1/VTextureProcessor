#include <UI/UINode.h>
#include <ShaderNode.h>

namespace UI
{
	class TextureNode : public Node
	{
	public:
		TextureNode(QJsonObject document, std::string_view name) 
			:Node(document, name), model(document)
		{}
	public:
		std::string_view GetName()const noexcept override { return model.GetName(); }
		size_t SourcesCount()const noexcept override { return model.SourcesCount(); }
		size_t SinksCount()const noexcept override { return model.SinksCount(); }
	private:
		void SetUniqueName(std::string_view xname)override{	model.SetUniqueName(xname);}
	private:
		ver::ShaderNode model;
	};
}