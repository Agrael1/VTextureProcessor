#include <Editor/NodeParser.h>
#include <Editor/Lexer.h>
#include <Editor/Highlight.h>

//[(property, name = "god sake")]
//uniform float x;
//
//[(property, name = "god sake2")]
//uniform float x2;
//
//void main()
//{
//}

consteval int T(int a)
{
	return ((int)token::type::T0) + a;
}

#define PUSH(a)vec.push_back((token::type)a)
bool f_open_br(std::vector<token::type>& vec, token::type top)
{
	using enum token::type;
	switch ((int)top)
	{
	case T(0):
		PUSH(T(2));
		PUSH(identifier);
		PUSH(tt_type);
		PUSH(tt_uniform);
		PUSH(close_sq);
		PUSH(close_br);
		PUSH(T(1));
		PUSH(open_br);
		break;
	default:
		return false;
	}
	return true;
}
bool f_close_br(std::vector<token::type>& vec, token::type top)
{
	switch ((int)top)
	{
	case T(3):
		break;
	default:
		return false;
	}
	return true;
}
bool f_eq(std::vector<token::type>& vec, token::type top)
{
	using enum token::type;
	switch ((int)top)
	{
	case T(2):
		PUSH(semicolon);
		PUSH(T(4));
		PUSH(eq);
		break;
	default:
		return false;
	}
	return true;
}
bool f_property(std::vector<token::type>& vec, token::type top)
{
	using enum token::type;
	switch ((int)top)
	{
	case T(1):
		PUSH(T(3));
		PUSH(tt_property);
		break;
	default:
		return false;
	}
	return true;
}
bool f_comma(std::vector<token::type>& vec, token::type top)
{
	using enum token::type;
	switch ((int)top)
	{
	case T(3):
		PUSH(T(3));
		PUSH(T(5));
		PUSH(comma);
		break;
	case T(7):
		PUSH(T(11));
		PUSH(comma);
		break;
	default:
		return false;
	}
	return true;
}
bool f_min(std::vector<token::type>& vec, token::type top)
{
	using enum token::type;
	switch ((int)top)
	{
	case T(5):
		PUSH(T(8));
		break;
	case T(8):
		PUSH(T(12));
		PUSH(eq);
		PUSH(tt_min);
		break;
	default:
		return false;
	}
	return true;
}
bool f_max(std::vector<token::type>& vec, token::type top)
{
	using enum token::type;
	switch ((int)top)
	{
	case T(5):
		PUSH(T(9));
		break;
	case T(9):
		PUSH(T(12));
		PUSH(eq);
		PUSH(tt_max);
		break;
	default:
		return false;
	}
	return true;
}
bool f_name(std::vector<token::type>& vec, token::type top)
{
	using enum token::type;
	switch ((int)top)
	{
	case T(5):
		PUSH(T(10));
		break;
	case T(10):
		PUSH(str_literal);
		PUSH(eq);
		PUSH(tt_name);
		break;
	default:
		return false;
	}
	return true;
}
bool f_num(std::vector<token::type>& vec, token::type top)
{
	using enum token::type;
	switch ((int)top)
	{
	case T(4):
	case T(6):
	case T(11):
	case T(12):
		PUSH(tt_num_literal);
		break;
	default:
		return false;
	}
	return true;
}
bool f_open_cbr(std::vector<token::type>& vec, token::type top)
{
	using enum token::type;
	switch ((int)top)
	{
	case T(4):
	case T(11):
	case T(12):
		PUSH(close_cbr);
		PUSH(T(7));
		PUSH(T(6));
		PUSH(open_cbr);
		break;
	default:
		return false;
	}
	return true;
}
bool f_close_cbr(std::vector<token::type>& vec, token::type top)
{
	using enum token::type;
	switch ((int)top)
	{
	case T(7):
		break;
	default:
		return false;
	}
	return true;
}
bool f_semicolon(std::vector<token::type>& vec, token::type top)
{
	using enum token::type;
	switch ((int)top)
	{
	case T(2):
		break;
	default:
		return false;
	}
	return true;
}
#undef PUSH

bool LLTable(std::vector<token::type>& stack, token::type input, token::type top)
{
	switch (input)
	{
	case token::type::open_br:return f_open_br(stack, top);
	case token::type::close_br:return f_close_br(stack, top);
	case token::type::eq:return f_eq(stack, top);
	case token::type::tt_property:return f_property(stack, top);
	case token::type::comma:return f_comma(stack, top);
	case token::type::tt_min:return f_min(stack, top);
	case token::type::tt_max:return f_max(stack, top);
	case token::type::tt_name:return f_name(stack, top);
	case token::type::tt_num_literal:return f_num(stack, top);
	case token::type::open_cbr:return f_open_cbr(stack, top);
	case token::type::close_cbr:return f_close_cbr(stack, top);
	case token::type::semicolon:return f_semicolon(stack, top);
	default:return false;
	}
}


bool NodeParser::Parse()
{
	using enum token::type;

	size_t block = 0;

	while (auto x = GetTokenInternal())
	{
		if (x->is_struct())
		{
			if ((x = GetTokenInternal()) && x->xtype == identifier)
				types.emplace(x->value);
			continue;
		}

		if (x->xtype == open_sq && !block)
			TryParseProperty(*x);

		if (x->xtype == keyword || types.contains(std::wstring{ x->value }))
			if(!block)TryParseFunction();

		if (x->xtype == macro_stmt)
		{
			auto r = GetTokenInternal();
			if (!r)break;
			if (r->xtype == identifier && x->line == r->line)
				macros.emplace(r->value);
		}

		block += x->xtype == open_cbr;
		block -= x->xtype == close_cbr;
	}

	return true;
}

NodeParser::NodeParser(std::wstring_view code) :
	gen(GetToken(code)) {}

std::optional<token> NodeParser::GetTokenInternal()
{
	if (!tokens.empty())
	{
		auto r = std::move(tokens.front());
		tokens.erase(tokens.begin());
		return r;
	}
	if (gen.finished())return {};
	auto r = gen.begin();
	if (r == gen.end())return {};
	return *r;
}

void NodeParser::TryParseFunction()
{
	auto x = GetTokenInternal();
	if (!x)return;
	if (x->xtype != token::type::identifier){
		tokens.push_back(*x);
		return;
	}
	auto y = GetTokenInternal();
	if (!y)return;
	if (y->xtype != token::type::open_br)
	{
		tokens.push_back(*x);
		tokens.push_back(*y);
		return;
	}

	funcs.emplace(x->value, x->line);
}


token::type GetType(const token& tk)
{
	using enum token::type;
	if (tk.xtype == keyword && tk.value == L"property")
		return tt_property;
	if (tk.xtype == identifier)
	{
		if (tk.value == L"min")return tt_min;
		if (tk.value == L"max")return tt_max;
		if (tk.value == L"name")return tt_name;
	}
	return tk.xtype;
}

#define ERR_CHECK(a) if(!a)return;

void NodeParser::TryParseProperty(token& tok)
{
	using enum token::type;

	std::vector<token::type> tkt;
	tkt.reserve(32);
	tkt.push_back(tt_none);
	tkt.push_back(T0);

	auto tk = GetTokenInternal();
	if (!tk)return;
	token::type current_tt = tt_none;

	while (!tkt.empty())
	{
		current_tt = tkt.back();
		if (current_tt >= T0 && current_tt <= TX) {
			tkt.pop_back();
			ERR_CHECK(LLTable(tkt, GetType(*tk), current_tt));
			continue;
		}
		if (current_tt == GetType(*tk))
		{
			tkt.pop_back();
			if (current_tt == tt_none)return;

			tk = GetTokenInternal();
			if (!tk)return;
		}
	}


	auto x = GetTokenInternal();
	if (!x || x->xtype!=open_br)return;
	x = GetTokenInternal();
	if (!x || x->xtype != keyword || x->value != L"property")return;
	x = GetTokenInternal();
	if (!x) return;

	PropertyDesc pdesc;
	bool finished = false;

	while (!finished)
	{
		if (!x) return;
		switch (x->xtype)
		{
		case close_br:
			finished = true; break;
		case comma:
			GetPropertyVal(pdesc);
		}
		x = GetTokenInternal();
	}
	if (!x || x->xtype!=close_sq) return;

	x = GetTokenInternal();
	if (!x || x->xtype != keyword || x->value != L"uniform")return;
	x = GetTokenInternal();
	if (!x || x->xtype != keyword)return;

	std::string a(x->value.begin(), x->value.end());

	ver::dc::LayoutElement r(a);
	if (!r)return;
	pdesc.ty = r.Get();

	x = GetTokenInternal();
	if (!x || x->xtype != identifier)return;

	pdesc.code_name = x->value;
	properties.push_back(pdesc);
}

void NodeParser::GetPropertyVal(PropertyDesc& pd)
{
	using enum token::type;
	auto x = GetTokenInternal();
	if (!x) return;
	if (x->value == L"name")
	{
		x = GetTokenInternal();
		if (!x || x->xtype != eq) return;

		x = GetTokenInternal();
		if (!x || x->xtype != str_literal) return;
		pd.name = x->value;
	}
}
