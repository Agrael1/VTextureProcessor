#pragma once
#include <Logic/PortType.h>
#include <memory>
#include <string>

class QOpenGLTexture;

class Source
{
public:
	std::string_view GetName()const noexcept;
	auto GetType()const noexcept { return type; }

	virtual ~Source() = default;
	virtual std::string_view YieldShader();
	virtual std::shared_ptr<QOpenGLTexture> YieldTexture();
protected:
	Source(std::string_view name, PortType ty);
private:
	std::string name;
	PortType type;
	//std::vector<Node*> connections; //tmp update
};