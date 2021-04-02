#pragma once
#include <memory>
#include <string>

class QOpenGLTexture;

class Source
{
public:
	enum class Type
	{
		Grayscale
	};
public:
	std::string_view GetName()const noexcept;
	auto GetType()const noexcept { return type; }

	virtual ~Source() = default;
	virtual std::string_view YieldShader();
	virtual std::shared_ptr<QOpenGLTexture> YieldTexture();
protected:
	Source(std::string_view name, Type ty);
private:
	std::string name;
	Type type;
	//std::vector<Node*> connections; //tmp update
};