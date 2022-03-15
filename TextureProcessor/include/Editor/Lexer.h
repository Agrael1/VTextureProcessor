#pragma once
#include <utils/Promises.h>
#include <Editor/Language.h>



struct token
{
public:
	enum class type
	{
		keyword,
		statement,
		identifier,
		macro_stmt,
		str_literal,
		macro,

		open_sq,
		close_sq,

		open_br,
		close_br,

		open_cbr, //{
		close_cbr,//}

		semicolon,
		comma,
		_operator,
		eq,

		other,
	}xtype;
	size_t offset;
	size_t line;
	std::wstring_view value;
public:
	token(type xtype, size_t offset, size_t line = 0, std::wstring_view value = L"")
		:xtype(xtype), offset(offset), value(value), line(line) {}
public:
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
};

ver::generator<token> GetToken(std::wstring_view code , size_t offset = 0);

#undef ENUM_LEX
