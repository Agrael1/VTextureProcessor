#include <Editor/Editor.h>
#include <format>
#include <QLineEdit>
#include <QApplication>
#include <QPainter>

const QRegularExpression sz_re{ "^\\d{0,3} ?%?$" };
constexpr std::string_view szbox_ss{
R"(QComboBox {
	border-radius: 0px;
	max-width: 6em;
})"
};

Editor::Editor()
	:hl(code.document()), value_range(sz_re), scroll(Qt::Horizontal)
{
	QFont font;
	font.setFamily("Consolas");
	font.setFixedPitch(true);
	font.setPointSize(font_defsz * font_defc / 100);
	code.setFont(font);
	code.setHorizontalScrollBar(&scroll);
	code.setCenterOnScroll(true);

	font_szbox.setEditable(true);
	font_szbox.setInsertPolicy(QComboBox::NoInsert);
	font_szbox.setValidator(&value_range);
	font_szbox.setStyleSheet(szbox_ss.data());
	font_szbox.addItems({ "20 %", "50 %","70 %", "100 %", "150 %", "200 %", "400 %" });
	connect(font_szbox.lineEdit(), &QLineEdit::editingFinished, this, &Editor::ParseFontSize);
	connect(&font_szbox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged), this, &Editor::SelectFontSize);
	font_szbox.setEditText(QString{ std::format("{} %", font_defc).data() });

	sbar.addWidget(&font_szbox);
	sbar.addWidget(&scroll);

	vl.addWidget(&code);
	vl.addLayout(&sbar);
	vl.setContentsMargins(0, 0, 0, 0);
	vl.setSpacing(0);

	setLayout(&vl);

	connect(&code, &CodeEditor::modificationChanged, this, &Editor::Modified);
}

void Editor::wheelEvent(QWheelEvent* event)
{
	if (!(qApp->keyboardModifiers() & Qt::ControlModifier))
		return;

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
void Editor::LoadText(const QString& in)
{
	code.setPlainText(in);
}
