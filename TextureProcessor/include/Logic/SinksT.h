/**
 * @file SinksT.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Templated sinks with custom logic 
 * Modified from Veritas engine WinD3D project (file SinksT.h)
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once
#include <Logic/Sink.h>
#include <memory>
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
			auto msg = std::format("Binding input[{}] to output [{}.{}]. Source has no texture in it",
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