#pragma once
#include <exception>
#include <fmt/printf.h>

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
		return fmt::sprintf("[File]: %s\n[Line]: %d", file, line);
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
			whatBuffer = fmt::sprintf("%s\n[message]:\n%s", Exception::what(), message);
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