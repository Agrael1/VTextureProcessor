#include <UI/Sliders.h>
#include <fmt/printf.h>
#include <UI/INode.h>


using namespace UI;

FloatSlider::FloatSlider(float& value, float min, float max)
	:slider(Qt::Horizontal), value(value), valid(min, max, 2)
{
	dpi = (max - min) / 100.0;

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