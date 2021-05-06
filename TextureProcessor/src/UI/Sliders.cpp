#include <UI/Sliders.h>
#include <fmt/printf.h>
#include <UI/INode.h>


using namespace UI;

FloatSlider::FloatSlider(float& value, float min, float max)
	:slider(Qt::Horizontal), value(value), valid(min, max, 2)
{
	dpi = (value * 2) / 100.0f;

	setLayout(&lay);
	text.setValidator(&valid);
	text.connect(&text, &QLineEdit::textEdited, this, &FloatSlider::TextEdited);

	slider.setRange(0, 100);
	slider.setValue(int(value / dpi));
	text.setText(fmt::sprintf("%.2g", value).c_str());

	QSizePolicy spText(QSizePolicy::Preferred, QSizePolicy::Fixed);
	spText.setHorizontalStretch(1);
	text.setSizePolicy(spText);
	QSizePolicy spSlider(QSizePolicy::Preferred, QSizePolicy::Fixed);
	spSlider.setHorizontalStretch(3);
	slider.setSizePolicy(spSlider);

	lay.addWidget(&slider);
	lay.addWidget(&text);
}

void FloatSlider::ValueChanged(int xvalue)
{
	value = xvalue * dpi;
	text.setText(fmt::sprintf("%.2g", value).c_str());
	if (xvalue == 100 && value < 20)
	{
		dpi = value < 10 ? (value * 2.0f) / 100.0f : 0.2f;
		slider.setValue(int(value / dpi));
	}
	if (xvalue == 1)
	{
		dpi = 0.01f;
		slider.setValue(int(value / dpi));
	}
}

void FloatSlider::TextEdited(const QString& text)
{
	slider.setValue(int(text.toFloat() / dpi));
}
void FloatSlider::SetChangedCallback(INode* to)
{
	connect(&slider, &QSlider::valueChanged, [this, to](int v)
		{
			ValueChanged(v);
			to->Update();
		});
}