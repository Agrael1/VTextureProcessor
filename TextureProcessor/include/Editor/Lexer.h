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
		comment,
		property,
		identifier,

		open_sq,
		close_sq,

		open_br,
		close_br,

		open_cbr,
		close_cbr,

		other,
	}xtype;
	size_t offset;
	std::wstring_view value;
public:
	token(type xtype, size_t offset, std::wstring_view value = L"")
		:xtype(xtype), offset(offset), value(value) {}
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

ver::generator<token> GetToken(std::wstring_view code);

#undef ENUM_LEX
