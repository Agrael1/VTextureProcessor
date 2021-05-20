/**
 * @file Exception.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Exception class for render graph model
 * Gathered from Veritas engine WinD3D project
 * https://github.com/Agrael1/VeritasD3D
 */
#pragma once
#include <exception>
#include <format>

class Exception :std::exception
{
public:
	Exception(unsigned int line, const char* file)noexcept :
		line(line), file(file) {}
	const char* what()const noexcept override
	{
		if (whatBuffer.empty())
			whatBuffer = GetType() + '\n' + GetOriginString();
		return whatBuffer.c_str();
	}
	virtual const char* GetType()const noexcept = 0;
	int GetLine()const noexcept
	{
		return line;
	}
	std::string_view GetFile()const noexcept
	{
		return file;
	}
	std::string GetOriginString()const noexcept
	{
		return std::format("[File]: {}\n[Line]: {}", file, line);
	}
private:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
};

class RenderGraphCompileException : public Exception
{
public:
	RenderGraphCompileException(std::string_view message, int line, const char* file) noexcept
		:Exception(line, file), message(message) {}
public:
	const char* what() const noexcept override
	{
		if (whatBuffer.empty())
			whatBuffer = std::format("{}\n[message]:\n{}", Exception::what(), message);
		return whatBuffer.c_str();
	}
	const char* GetType() const noexcept override
	{
		return "Render Graph Compile Exception";
	}
	std::string_view GetMessage() const noexcept
	{
		return message;
	}
private:
	std::string message;
};

#define RGC_EXCEPTION( message ) RenderGraphCompileException( (message),__LINE__,__FILE__ )