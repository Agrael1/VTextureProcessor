#include <Editor/NodeParser.h>
#include <Editor/Lexer.h>
#include <Editor/Highlight.h>


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

