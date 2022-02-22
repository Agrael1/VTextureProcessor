#pragma once
#include <utils/Promises.h>

#define ENUM_LEX()\
	X(keyword)\
	X(statement)\
	X(property)\
	X(identifier)\
	X(other)


struct token
{
	static constexpr std::string_view type_s[] {
#define X(a) #a, 
		ENUM_LEX()
#undef X
	};
	enum class type
	{
#define X(a) a, 
		ENUM_LEX()
#undef X
	}xtype;

	std::string_view value;
	size_t offset;

	size_t length()const noexcept
	{
		return value.length();
	}
	size_t index()const noexcept
	{
		return size_t(xtype);
	}
	std::string to_string()const noexcept;
};

ver::generator<token> GetToken(std::string_view code);

#undef ENUM_LEX
