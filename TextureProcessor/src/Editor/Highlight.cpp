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

constexpr auto var_c = ver::rgb_to_hex(156, 220, 254);
constexpr auto fun_c = ver::rgb_to_hex(220, 220, 170);

Highlighter::Highlighter(QTextDocument* parent)
	:QSyntaxHighlighter(parent)
{
	using enum ver::detail::Format;
	formats[statements].setForeground({ "#569cd6" });
	formats[kwords].setForeground({ "#d8a0df" });
	formats[comment].setForeground({ "#56a64a" });
	formats[user_type].setForeground({ "#4ec9b0" });
	formats[variable].setForeground({ var_c.c_str() });
	formats[function].setForeground({ fun_c.c_str() });
}

void Highlighter::highlightBlock(const QString& text)
{
	setCurrentBlockState(0);

	int startIndex = 0;
	int endIndex = 0;
	if (previousBlockState() != 1)
		startIndex = text.indexOf(startExpression); //if not =-1

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
		Parse(std::wstring_view(text.toStdWString()).substr(endIndex, startIndex));
	}
	Parse(std::wstring_view(text.toStdWString()).substr(endIndex));
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

void Highlighter::Parse(std::wstring_view part)
{
	for (auto& i : GetToken(part))
	{
		if (i.index() <= 2)
			setFormat(i.offset, i.length(), formats[i.index()]);

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
