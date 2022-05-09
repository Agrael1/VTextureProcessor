/**
 * @file Sliders.cpp
 * @author Ilya Doroshenko (xdoros01)
 * @brief Controls for properties and constant buffers
 */
#include <UI/Sliders.h>
#include <QMatrix4x4>


using namespace UI;

UI::CheckBox::CheckBox(bool& value, const QString& name)
	: box(name)
{
	box.setChecked(value);
	lay.addWidget(&box);
	setLayout(&lay);
	connect(&box, &QCheckBox::stateChanged, [this, &value](int a) {value = bool(a); emit ValueChanged(); });
}

UI::Matrix::Matrix(QMatrix4x4& value, const QString& name)
{
	lay.addWidget(new QLabel(name), 0, 0, -1, 1, Qt::AlignLeft);
	float* mat = value.data();
	for (size_t i = 1; i < 5; i++)
		for (size_t j = 0; j < 4; j++)
		{
			auto le = new QLineEdit;
			lay.addWidget(le, i, j);
			le->setValidator(&valid);
			le->setText(QString::number(*mat));
			connect(le, &QLineEdit::textEdited, [mat, this](const QString& v) {
				*mat = v.toFloat();
				emit ValueChanged();
				});
			mat++;
		}
}

UI::IntSlider::IntSlider(int& xvalue, const QString& name, int xmin, int xmax)
	:valid(min, max), slider(Qt::Horizontal), min(xmin), max(xmax), value(xvalue)
{
	vl.setContentsMargins(0, 0, 0, 0);
	lay.setContentsMargins(0, 0, 0, 0);
	if (!name.isEmpty())vl.addWidget(new QLabel(name));

	slider.setRange(min, max);
	text.setValidator(&valid);

	text.setText(QString::number(value));
	slider.setValue(value);

	connect(&text, &QLineEdit::textEdited,
		[this](const QString& v) {
			value = v.toInt();
			slider.setValue(value);
		});
	connect(&slider, &QSlider::valueChanged,
		[this](int v) {
			value = v;
			text.setText(QString::number(value));
			emit ValueChanged();
		});



	// Set text size policy
	QSizePolicy spText(QSizePolicy::Preferred, QSizePolicy::Fixed);
	spText.setHorizontalStretch(1);
	text.setSizePolicy(spText);

	// Set slider size policy
	QSizePolicy spSlider(QSizePolicy::Preferred, QSizePolicy::Fixed);
	spSlider.setHorizontalStretch(3);
	slider.setSizePolicy(spSlider);

	lay.addWidget(&slider);
	lay.addWidget(&text);
	vl.addLayout(&lay);
	setLayout(&vl);
}
void UI::IntSlider::SetMin(int xmin)
{
	slider.setRange(min = xmin, max);
	valid.setBottom(xmin);
}
void UI::IntSlider::SetMax(int xmax)
{
	slider.setRange(min, max = xmax);
	valid.setTop(xmax);
}

void UI::IntSlider::Revalidate()
{
	slider.setValue(value);
}

UI::FloatSlider::FloatSlider(float& value, const QString& name, float xmin, float xmax)
	:valid(xmin, xmax, 4), min(xmin), max(xmax), slider(Qt::Horizontal), val(value), dpi(fabs(xmax - xmin) / fprecision)
{
	vl.setContentsMargins(0, 0, 0, 0);
	lay.setContentsMargins(0, 0, 0, 0);
	if (!name.isEmpty())vl.addWidget(new QLabel(name));


	slider.setRange(0, precision);
	text.setValidator(&valid);
	connect(&text, &QLineEdit::textEdited,
		[this](const QString& v) {
			val = v.toFloat();
			slider.setValue(int(roundf((val - min) / dpi)));
			emit ValueChanged();
		});
	connect(&slider, &QSlider::sliderMoved,
		[this](int v) {
			val = v * dpi + min;
			text.setText(QString::number(val));
			emit ValueChanged();
		});

	text.setText(QString::number(value));
	slider.setValue(int(roundf((value - min) / dpi)));

	// Set text size policy
	QSizePolicy spText(QSizePolicy::Preferred, QSizePolicy::Fixed);
	spText.setHorizontalStretch(1);
	text.setSizePolicy(spText);

	// Set slider size policy
	QSizePolicy spSlider(QSizePolicy::Preferred, QSizePolicy::Fixed);
	spSlider.setHorizontalStretch(3);
	slider.setSizePolicy(spSlider);

	lay.addWidget(&slider);
	lay.addWidget(&text);
	vl.addLayout(&lay);
	setLayout(&vl);
}
void UI::FloatSlider::SetMin(float xmin)
{
	valid.setBottom(min = xmin);
	dpi = fabs(max - min) / fprecision;
	slider.setValue(int(roundf((val - min) / dpi)));
}
void UI::FloatSlider::SetMax(float xmax)
{
	valid.setTop(max = xmax);
	dpi = fabs(max - min) / fprecision;
	slider.setValue(int(roundf((val - min) / dpi)));
}
