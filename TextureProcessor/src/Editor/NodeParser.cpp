#include <Editor/NodeParser.h>
#include <Editor/Lexer.h>
#include <Editor/Highlight.h>

bool NodeParser::Parse(std::wstring_view code)
{
	size_t start_offset = 0;
	size_t end_offset = 0;
	size_t block = 0;

	auto gen = GetToken(code);
	using enum token::type;

	for (auto& i : gen)
	{
		block += i.xtype == open_cbr;
		block -= i.xtype == close_cbr;

		if (i.is_struct())
			if (auto x = gen.begin(); x != gen.end() && x->xtype == identifier)
				types.emplace(x->value, Highlighter::Format::user_type);



		if (i.xtype == keyword || types.contains(std::wstring{ i.value }))
			if (auto x = gen.begin(); x != gen.end() && x->xtype == identifier) // var or func
				;//types.emplace(x->value);

		if (gen.finished())
			break;
	}
	return true;
}
