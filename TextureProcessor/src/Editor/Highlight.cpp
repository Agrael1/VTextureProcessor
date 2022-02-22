#include <Editor/Highlight.h>
#include <QRegularExpression>
#include <Editor/Lexer.h>


Highlighter::Highlighter(QTextDocument* parent)
	:QSyntaxHighlighter(parent)
{
	formats[0].setForeground({"#569cd6"});
	formats[1].setForeground({"#d8a0df"});
}

void Highlighter::highlightBlock(const QString& text)
{
	auto s = text.toStdString();
	for (auto& i : GetToken(s))
	{
		if(i.index() <= 1)setFormat(i.offset, i.length(), formats[i.index()]);
	}
}
