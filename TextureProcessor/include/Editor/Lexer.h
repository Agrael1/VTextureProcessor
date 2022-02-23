#pragma once
#include <utils/Promises.h>
#include <Editor/Language.h>


#define ENUM_LEX()\
	X(keyword)\
	X(statement)\
	X(comment)\
	X(property)\
	X(identifier)\
	X(other)


struct token
{
	static constexpr std::wstring_view type_s[] {
#define X(a) L#a, 
		ENUM_LEX()
#undef X
	};
	enum class type
	{
#define X(a) a, 
		ENUM_LEX()
#undef X
	}xtype;
	size_t offset;
	std::wstring_view value;

	size_t length()const noexcept
	{
		return value.length();
	}
	size_t index()const noexcept
	{
		return size_t(xtype);
	}
	bool is_struct()const noexcept
	{
		return xtype == type::keyword && value == L"struct";
	}
	std::wstring to_string()const noexcept;
};

ver::generator<token> GetToken(std::wstring_view code);

#undef ENUM_LEX
