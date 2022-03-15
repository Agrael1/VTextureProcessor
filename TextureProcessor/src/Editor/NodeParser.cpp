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

void NodeParser::TryParseProperty(token& tok)
{
	using enum token::type;

	
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

	ver::dc::LayoutElement r(x->value);
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
