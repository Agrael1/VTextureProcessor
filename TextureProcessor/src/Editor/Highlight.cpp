#include <Editor/Highlight.h>
#include <QRegularExpression>
#include <Editor/Lexer.h>

const QRegularExpression startExpression("/\\*");
const QRegularExpression endExpression("\\*/");

Highlighter::Highlighter(QTextDocument* parent)
	:QSyntaxHighlighter(parent)
{
	formats[statements].setForeground({ "#569cd6" });
	formats[kwords].setForeground({ "#d8a0df" });
	formats[comment].setForeground({ "#56a64a" });
	formats[user_type].setForeground({ "#4ec9b0" });
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

void Highlighter::SetTypeInfo(std::unordered_map<std::wstring, size_t> xtypes)
{
	types = std::move(xtypes);
	rehighlight();
}

void Highlighter::Parse(std::wstring_view part)
{
	for (auto& i : GetToken(part))
	{
		if (i.index() <= 2)
			setFormat(i.offset, i.length(), formats[i.index()]);
		if (i.xtype == token::type::identifier)
			if(auto x = types.find({ i.value.data(), i.value.size() }); x!=types.end())
			setFormat(i.offset, i.length(), formats[x->second]);
	}
}
