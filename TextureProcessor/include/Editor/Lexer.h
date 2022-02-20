#pragma once
#include <utils/Promises.h>

struct token
{
	enum class type
	{
		keyword,
		property,
		other
	}type;
	std::string_view value;
	size_t offset;
};

//ver::generator<token> GetToken(std::string_view code);
