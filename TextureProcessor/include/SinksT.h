#pragma once
#include <Sink.h>
#include <memory>
#include <fmt/printf.h>
#include <utils/Exception.h>


template <PortType ty = PortType::Grayscale>
class DirectTextureSink : public Sink
{
public:
	static std::unique_ptr<Sink> Make(std::string_view registeredName, std::shared_ptr<QOpenGLTexture>& target)
	{
		return std::make_unique<DirectTextureSink>(registeredName, target);
	}
	void PostLinkValidate() const override
	{
		
	}
	virtual void Unlink()override
	{
		target = nullptr;
	}
	void Bind(Source& source) override
	{
		auto p = source.YieldTexture();
		if (!p)
		{
			auto msg = fmt::sprintf("Binding input[%s] to output [%s.%s]. Source has no texture in it",
				GetRegisteredName(),
				GetOutputNodeName(),
				GetSourceName());
			throw RGC_EXCEPTION(msg);
		}
		target = std::move(p);
	}
	DirectTextureSink(std::string_view registeredName, std::shared_ptr<QOpenGLTexture>& target)
		:
		Sink(registeredName, ty),
		target(target)
	{}
private:
	std::shared_ptr<QOpenGLTexture>& target;
};

using GrayscaleSink = DirectTextureSink<PortType::Grayscale>;