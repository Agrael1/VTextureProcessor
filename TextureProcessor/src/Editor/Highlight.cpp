#include <Editor/Highlight.h>

const std::array<QRegularExpression, 16> Highlighter::statements{
	QRegularExpression{"\\bbreak\\b"}, QRegularExpression{"\\bcontinue\\b"}, QRegularExpression{"\\bdo\\b"}, QRegularExpression{"\\bfor\\b"}, QRegularExpression{"\\bwhile\\b"}, QRegularExpression{"\\bswitch\\b"}, QRegularExpression{"\\bcase\\b"}, QRegularExpression{"\\bdefault\\b"},
	QRegularExpression{"\\bdiscard\\b"}, QRegularExpression{"\\breturn\\b"},
	QRegularExpression{"\\bin\\b"}, QRegularExpression{"\\bout\\b"}, QRegularExpression{"\\binout\\b"},
	QRegularExpression{"\\bif\\b"}, QRegularExpression{"\\belse\\b"},
	QRegularExpression{"\\blayout\\b"}
};
const std::array<QRegularExpression, 92> Highlighter::kwords{
	QRegularExpression{"\\battribute\\b"}, QRegularExpression{"\\bconst\\b"}, QRegularExpression{"\\buniform\\b"}, QRegularExpression{"\\bvarying\\b"},
	QRegularExpression{"\\bfloat\\b"}, QRegularExpression{"\\bdouble\\b"}, QRegularExpression{"\\bint\\b"}, QRegularExpression{"\\bvoid\\b"}, QRegularExpression{"\\bbool\\b"}, QRegularExpression{"\\btrue\\b"}, QRegularExpression{"\\bfalse\\b"},
	QRegularExpression{"\\bmat2\\b"}, QRegularExpression{"\\bmat3\\b"}, QRegularExpression{"\\bmat4\\b"}, QRegularExpression{"\\bdmat2\\b"}, QRegularExpression{"\\bdmat3\\b"}, QRegularExpression{"\\bdmat4\\b"},
	QRegularExpression{"\\bmat2x2\\b"}, QRegularExpression{"\\bmat2x3\\b"}, QRegularExpression{"\\bmat2x4\\b"}, QRegularExpression{"\\bdmat2x2\\b"}, QRegularExpression{"\\bdmat2x3\\b"}, QRegularExpression{"\\bdmat2x4\\b"},
	QRegularExpression{"\\bmat3x2\\b"}, QRegularExpression{"\\bmat3x3\\b"}, QRegularExpression{"\\bmat3x4\\b"}, QRegularExpression{"\\bdmat3x2\\b"}, QRegularExpression{"\\bdmat3x3\\b"}, QRegularExpression{"\\bdmat3x4\\b"},
	QRegularExpression{"\\bmat4x2\\b"}, QRegularExpression{"\\bmat4x3\\b"}, QRegularExpression{"\\bmat4x4\\b"}, QRegularExpression{"\\bdmat4x2\\b"}, QRegularExpression{"\\bdmat4x3\\b"}, QRegularExpression{"\\bdmat4x4\\b"},
	QRegularExpression{"\\bvec2\\b"}, QRegularExpression{"\\bvec3\\b"}, QRegularExpression{"\\bvec4\\b"}, QRegularExpression{"\\bivec2\\b"}, QRegularExpression{"\\bivec3\\b"}, QRegularExpression{"\\bivec4\\b"}, QRegularExpression{"\\bbvec2\\b"}, QRegularExpression{"\\bbvec3\\b"}, QRegularExpression{"\\bbvec4\\b"}, QRegularExpression{"\\bdvec2\\b"}, QRegularExpression{"\\bdvec3\\b"}, QRegularExpression{"\\bdvec4\\b"},
	QRegularExpression{"\\buint\\b"}, QRegularExpression{"\\buvec2\\b"}, QRegularExpression{"\\buvec3\\b"}, QRegularExpression{"\\buvec4\\b"},
	QRegularExpression{"\\bsampler1D\\b"}, QRegularExpression{"\\bsampler2D\\b"}, QRegularExpression{"\\bsampler3D\\b"}, QRegularExpression{"\\bsamplerCube\\b"},
	QRegularExpression{"\\bsampler1DShadow\\b"}, QRegularExpression{"\\bsampler2DShadow\\b"}, QRegularExpression{"\\bsamplerCubeShadow\\b"},
	QRegularExpression{"\\bsampler1DArray\\b"}, QRegularExpression{"\\bsampler2DArray\\b"},
	QRegularExpression{"\\bsampler1DArrayShadow\\b"}, QRegularExpression{"\\bsampler2DArrayShadow\\b"},
	QRegularExpression{"\\bisampler1D\\b"}, QRegularExpression{"\\bisampler2D\\b"}, QRegularExpression{"\\bisampler3D\\b"}, QRegularExpression{"\\bisamplerCube\\b"},
	QRegularExpression{"\\bisampler1DArray\\b"}, QRegularExpression{"\\bisampler2DArray\\b"},
	QRegularExpression{"\\busampler1D\\b"}, QRegularExpression{"\\busampler2D\\b"}, QRegularExpression{"\\busampler3D\\b"}, QRegularExpression{"\\busamplerCube\\b"},
	QRegularExpression{"\\busampler1DArray\\b"}, QRegularExpression{"\\busampler2DArray\\b"},
	QRegularExpression{"\\bsampler2DRect\\b"}, QRegularExpression{"\\bsampler2DRectShadow\\b"}, QRegularExpression{"\\bisampler2DRect\\b"}, QRegularExpression{"\\busampler2DRect\\b"},
	QRegularExpression{"\\bsamplerBuffer\\b"}, QRegularExpression{"\\bisamplerBuffer\\b"}, QRegularExpression{"\\busamplerBuffer\\b"},
	QRegularExpression{"\\bsampler2DMS\\b"}, QRegularExpression{"\\bisampler2DMS\\b"}, QRegularExpression{"\\busampler2DMS\\b"},
	QRegularExpression{"\\bsampler2DMSArray\\b"}, QRegularExpression{"\\bisampler2DMSArray\\b"}, QRegularExpression{"\\busampler2DMSArray\\b"},
	QRegularExpression{"\\bsamplerCubeArray\\b"}, QRegularExpression{"\\bsamplerCubeArrayShadow\\b"}, QRegularExpression{"\\bisamplerCubeArray\\b"}, QRegularExpression{"\\busamplerCubeArray\\b"},
	QRegularExpression{"\\bstruct\\b"}
};

Highlighter::Highlighter(QTextDocument* parent)
	:QSyntaxHighlighter(parent)
{
	kwd.setForeground({ "#569cd6" });
	stmt.setForeground({ "#d8a0df" });
}

void Highlighter::highlightBlock(const QString& text)
{
	for (const auto& rule : kwords) 
	{
		QRegularExpressionMatchIterator matchIterator = rule.globalMatch(text);
		while (matchIterator.hasNext()) {
			QRegularExpressionMatch match = matchIterator.next();
			setFormat(match.capturedStart(), match.capturedLength(), kwd);
		}
	}
	for (const auto& rule : statements)
	{
		QRegularExpressionMatchIterator matchIterator = rule.globalMatch(text);
		while (matchIterator.hasNext()) {
			QRegularExpressionMatch match = matchIterator.next();
			setFormat(match.capturedStart(), match.capturedLength(), stmt);
		}
	}
}
