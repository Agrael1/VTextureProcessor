#include <Editor/Editor.h>
#include <format>

const QRegularExpression sz_re{ "^\\d{0,3} ?%?$" };

Editor::Editor()
	:hl(code.document()), value_range(sz_re)
{
	QFont font;
	font.setFamily("Consolas");
	font.setFixedPitch(true);
	font.setPointSize(font_defsz * font_defc / 100);
	code.setFont(font);

	font_szbox.setEditable(true);
	font_szbox.setInsertPolicy(QComboBox::NoInsert);
	font_szbox.setValidator(&value_range);
	font_szbox.addItems({ "20 %", "50 %","70 %", "100 %", "150 %", "200 %", "400 %" });
	connect(font_szbox.lineEdit(), &QLineEdit::editingFinished, this, &Editor::ParseFontSize);
	connect(&font_szbox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged), this, &Editor::SelectFontSize);
	font_szbox.setEditText(QString{ std::format("{} %", font_defc).data() });

	vl.addWidget(&code);
	vl.addWidget(&font_szbox);
	setLayout(&vl);
}

void Editor::wheelEvent(QWheelEvent* event)
{
	auto text = font_szbox.currentText();
	const QRegularExpression sz{ "^\\d{0,3}" };
	auto x = sz.match(text);
	int i = x.captured().toInt();
	int n = event->delta() > 0 ? int(i * 1.1f) : int(i / 1.1f);
	n = n > 400 ? 400 : n;
	n = n < 20 ? 20 : n;

	QFont font = code.font();
	font.setPointSize(font_defsz * n / 100);
	code.setFont(font);

	font_szbox.setEditText(QString{ std::format("{} %", n).data() });
	font_szbox.clearFocus();
}

void Editor::ParseFontSize()
{
	auto text = font_szbox.currentText();
	const QRegularExpression sz{ "^\\d{0,3}" };
	auto x = sz.match(text);
	int i = x.captured().toInt();

	QFont font = code.font();
	font.setPointSize(font_defsz * i / 100);
	code.setFont(font);

	font_szbox.setEditText(QString{ std::format("{} %", i).data() });
	font_szbox.clearFocus();
}
void Editor::SelectFontSize(const QString& text)
{
	const QRegularExpression sz{ "^\\d{0,3}" };
	auto x = sz.match(text);
	int i = x.captured().toInt();

	QFont font = code.font();
	font.setPointSize(font_defsz * i / 100);
	code.setFont(font);

	font_szbox.setEditText(QString{ std::format("{} %", i).data() });
	font_szbox.clearFocus();
}

CodeEditor::CodeEditor(QWidget* parent) : QPlainTextEdit(parent), line_number_area(this)
{
	connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
	connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
	connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightLine);
	setWordWrapMode(QTextOption::WrapMode::NoWrap);

	updateLineNumberAreaWidth(0);
	highlightLine();
}

int CodeEditor::lineNumberAreaWidth()
{
	int max = std::max(1, blockCount());
	int digits = std::log10(max) + 1;
	return 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}
void CodeEditor::updateLineNumberArea(const QRect& rect, int dy)
{
	if (dy)
		line_number_area.scroll(0, dy);
	else
		line_number_area.update(0, rect.y(), line_number_area.width(), rect.height());

	if (rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}
void CodeEditor::resizeEvent(QResizeEvent* e)
{
	QPlainTextEdit::resizeEvent(e);
	QRect cr = contentsRect();
	line_number_area.setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}
void CodeEditor::wheelEvent(QWheelEvent* e)
{
	if (!(qApp->keyboardModifiers() & Qt::ControlModifier))
		return QPlainTextEdit::wheelEvent(e);


}
void CodeEditor::highlightLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly() && !textCursor().hasSelection()) {
		QTextEdit::ExtraSelection selection;
		selection.format.setProperty(QTextFormat::Property::BackgroundBrush, QBrush{ qApp->palette().alternateBase().color() });
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}
void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent* event)
{
	QPainter painter(&line_number_area);
	painter.fillRect(event->rect(), Qt::gray);

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int)std::lround(blockBoundingGeometry(block).translated(contentOffset()).top());
	int bottom = top + (int)std::lround(blockBoundingRect(block).height());
	while (block.isValid() && top <= event->rect().bottom()) {
		if (block.isVisible() && bottom >= event->rect().top()) {
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::black);
			painter.drawText(0, top, line_number_area.width(), fontMetrics().height(),
				Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int)std::lround(blockBoundingRect(block).height());
		++blockNumber;
	}
}

QSize CodeEditor::LineNumberArea::sizeHint() const
{
	return QSize(code_editor->lineNumberAreaWidth(), 0);
}

void CodeEditor::LineNumberArea::paintEvent(QPaintEvent* event)
{
	code_editor->lineNumberAreaPaintEvent(event);
}
