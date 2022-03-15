#include <Editor/Highlight.h>
#include <QRegularExpression>
#include <Editor/Lexer.h>
#include <utils/const_utils.h>

//struct c { int a; };
//
//void a()
//{
//}
//
//int a_main()
//{
//	c a;
//}


const QRegularExpression startExpression("/\\*");
const QRegularExpression endExpression("\\*/");
const QRegularExpression single_line("//");
const QRegularExpression dbl_quotes("\"");

constexpr auto var_c = ver::rgb_to_hex(156, 220, 254);
constexpr auto fun_c = ver::rgb_to_hex(220, 220, 170);
constexpr auto str_lit_c = ver::rgb_to_hex(214, 157, 133);

Highlighter::Highlighter(QTextDocument* parent)
	:QSyntaxHighlighter(parent)
{
	using enum ver::detail::Format;
	formats[kwords].setForeground({ "#569cd6" });
	formats[statements].setForeground({ "#d8a0df" });
	formats[comment].setForeground({ "#56a64a" });
	formats[user_type].setForeground({ "#4ec9b0" });
	formats[variable].setForeground({ var_c.c_str() });
	formats[function].setForeground({ fun_c.c_str() });
	formats[str_lit].setForeground({ str_lit_c.c_str() });
}

void Highlighter::highlightBlock(const QString& text)
{
	setCurrentBlockState(0);
	auto xtext = text.toStdWString();
	auto view = std::wstring_view(xtext);

	int startIndex = text.indexOf(single_line);
	if (startIndex >= 0)
	{
		setFormat(startIndex, text.length() - startIndex, formats[ver::detail::Format::comment]);
		view.remove_suffix(text.length() - startIndex);
	}	
	
	startIndex = 0;
	int endIndex = 0;
	if (previousBlockState() != 1)
	{
		startIndex = text.indexOf(startExpression); //if not =-1
		Parse(view.substr(0, startIndex));
	}



	while (startIndex >= 0)
	{
		QRegularExpressionMatch endMatch;
		endIndex = text.indexOf(endExpression, startIndex, &endMatch);
		int commentLength = 0;

		if (endIndex == -1)
		{
			setCurrentBlockState(1); // full transitive block
			commentLength = text.length() - startIndex;
			setFormat(startIndex, commentLength, formats[2]);
			return;
		}

		commentLength = endIndex - startIndex
			+ endMatch.capturedLength();
		setFormat(startIndex, commentLength, formats[2]);
		startIndex = text.indexOf(startExpression,
			startIndex + commentLength);
		Parse(view.substr(endIndex, startIndex), endIndex);
	}
	Parse(view.substr(endIndex), endIndex);


}

void Highlighter::SetTypeInfo(std::unordered_set<std::wstring> xtypes)
{
	types = std::move(xtypes);
	rehighlight();
}

void Highlighter::SetFuncInfo(std::unordered_map<std::wstring, size_t> xfuncs)
{
	funcs = std::move(xfuncs);
	rehighlight();
}

void Highlighter::Parse(std::wstring_view part, size_t offset)
{
	using enum token::type;
	for (auto& i : GetToken(part, offset))
	{
		switch (i.xtype)
		{
		case str_literal:
			setFormat(i.offset, i.length(), formats[ver::detail::Format::str_lit]);
			continue;
		case keyword:
			setFormat(i.offset, i.length(), formats[ver::detail::Format::kwords]);
			continue;
		case statement:
			setFormat(i.offset, i.length(), formats[ver::detail::Format::statements]);
			continue;
		default:
			break;
		}

		if (i.xtype == token::type::identifier)
		{
			if (types.contains({ i.value.data(), i.value.size() }))
				setFormat(i.offset, i.length(), formats[ver::detail::Format::user_type]);
			if (auto x = funcs.find({ i.value.data(), i.value.size() }); x != funcs.end()
				&& x->second == currentBlock().blockNumber())
				setFormat(i.offset, i.length(), formats[ver::detail::Format::function]);
		}
	}
}
