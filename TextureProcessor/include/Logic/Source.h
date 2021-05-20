/**
 * @file Source.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Source connector logic
 * Modified from Veritas engine WinD3D project (file Source.*)
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once
#include <Logic/PortType.h>

class QOpenGLTexture;

class Source
{
public:
	std::string_view GetName()const noexcept;
	PortType GetType()const noexcept { return type; }

	virtual ~Source() = default;
	virtual std::string_view YieldShader();
	virtual std::shared_ptr<QOpenGLTexture> YieldTexture();
protected:
	Source(std::string_view name, PortType ty);
private:
	std::string name;
	PortType type;
};