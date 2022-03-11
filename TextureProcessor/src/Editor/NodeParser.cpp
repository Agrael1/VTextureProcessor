#include <Editor/NodeParser.h>
#include <Editor/Lexer.h>
#include <Editor/Highlight.h>

bool NodeParser::Parse(std::wstring_view code)
{
	using enum token::type;

	auto gen = GetToken(code);
	size_t block = 0;

	while (auto x = GetTokenInternal(gen))
	{
		if (x->is_struct())
		{
			if ((x = GetTokenInternal(gen)) && x->xtype == identifier)
				types.emplace(x->value);
			continue;
		}

		if (x->xtype == keyword || types.contains(std::wstring{ x->value }))
			if(!block)TryParseFunction(gen);

		block += x->xtype == open_cbr;
		block -= x->xtype == close_cbr;
	}
	return true;
}

std::optional<token> NodeParser::GetTokenInternal(ver::generator<token>& gen) 
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

void NodeParser::TryParseFunction(ver::generator<token>& gen)
{
	auto x = GetTokenInternal(gen);
	if (!x)return;
	if (x->xtype != token::type::identifier){
		tokens.push_back(*x);
		return;
	}
	auto y = GetTokenInternal(gen);
	if (!y)return;
	if (y->xtype != token::type::open_br)
	{
		tokens.push_back(*x);
		tokens.push_back(*y);
		return;
	}

	funcs.emplace(x->value, x->line);
}
