#include <Editor/Highlight.h>
#include <QRegularExpression>
#include <Editor/Lexer.h>
#include <utils/const_utils.h>


std::unordered_set<std::wstring_view> Highlighter::builtin_functions{ builtin_funcs.begin(), builtin_funcs.end() };

const QRegularExpression startExpression("/\\*");
const QRegularExpression endExpression("\\*/");
const QRegularExpression single_line("//");
const QRegularExpression dbl_quotes("\"");

constexpr auto var_c = ver::rgb_to_hex(156, 220, 254);
constexpr auto fun_c = ver::rgb_to_hex(220, 220, 170);
constexpr auto str_lit_c = ver::rgb_to_hex(214, 157, 133);
constexpr auto macro_lit_c = ver::rgb_to_hex(190, 183, 255);
constexpr auto define_c = ver::rgb_to_hex(154, 154, 154);
constexpr auto const_c = ver::rgb_to_hex(156, 220, 254);
constexpr auto builtin_c = ver::rgb_to_hex(227, 106, 20);
constexpr auto numlit_c = ver::rgb_to_hex(177, 177, 177);

Highlighter::Highlighter(QTextDocument* parent)
	:QSyntaxHighlighter(parent)
{
	using enum ver::detail::Format;
	formats[kwords].setForeground(QColor{ u"#569cd6" });
	formats[statements].setForeground(QColor{ u"#d8a0df" });
	formats[comment].setForeground(QColor{ u"#56a64a" });
	formats[user_type].setForeground(QColor{ u"#4ec9b0" });
	formats[variable].setForeground(QColor{ var_c });
	formats[function].setForeground(QColor{ fun_c });
	formats[str_lit].setForeground(QColor{ str_lit_c });
	formats[define].setForeground(QColor{ define_c });
	formats[macro_lit].setForeground(QColor{ macro_lit_c });
	formats[cbuf].setForeground(QColor{ const_c });
	formats[builtin].setForeground(QColor{ builtin_c });
	formats[num_lit].setForeground(QColor{ numlit_c });
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

void Highlighter::SetCBufInfo(std::unordered_set<std::wstring> xcbuf)
{
	cbuf = std::move(xcbuf);
}

void Highlighter::SetInfo(
	std::unordered_set<std::wstring> xtypes,
	std::unordered_set<std::wstring> xmacros,
	std::unordered_map<std::wstring, size_t> xfuncs)
{
	types = std::move(xtypes);
	macros = std::move(xmacros);
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
			break;
		case keyword:
			setFormat(i.offset, i.length(), formats[ver::detail::Format::kwords]);
			break;
		case statement:
			setFormat(i.offset, i.length(), formats[ver::detail::Format::statements]);
			break;
		case macro_stmt:
			setFormat(i.offset, i.length(), formats[ver::detail::Format::define]);
			break;
		case tt_num_literal:
			setFormat(i.offset, i.length(), formats[ver::detail::Format::num_lit]);
			break;
		case identifier:
		{
			if (i.value == L"sv_texc")
				return setFormat(i.offset, i.length(), formats[ver::detail::Format::builtin]);
			std::wstring x{ i.value };
			if (auto x = funcs.find({ i.value.data(), i.value.size() }); x != funcs.end()
				&& x->second == currentBlock().blockNumber())
				setFormat(i.offset, i.length(), formats[ver::detail::Format::function]);
			if (builtin_functions.contains(i.value))
				setFormat(i.offset, i.length(), formats[ver::detail::Format::function]);
			if (macros.contains(x))
				setFormat(i.offset, i.length(), formats[ver::detail::Format::macro_lit]);
			if (types.contains(x))
				setFormat(i.offset, i.length(), formats[ver::detail::Format::user_type]);
			if (cbuf.contains(x))
				setFormat(i.offset, i.length(), formats[ver::detail::Format::cbuf]);
			break;
		}
		default:
			break;
		}
	}
}
