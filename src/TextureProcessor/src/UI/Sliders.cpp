#include <UI/Sliders.h>
#include <fmt/printf.h>
#include <UI/INode.h>


using namespace UI;

/**
 * @brief Construct a new Float Slider:: Float Slider object
 *
 * @param value Value changed by the slider
 * @param min Minimum value
 * @param max Maximum value
 */
FloatSlider::FloatSlider(float& value, float min, float max)
	:slider(Qt::Horizontal), value(value), valid(min, max, 2)
{
	dpi = (value * 2) / 100.0f;

	setLayout(&lay);
	text.setValidator(&valid);
	text.connect(&text, &QLineEdit::textEdited, this, &FloatSlider::TextEdited);

	// Display with 2 floating point precision
	slider.setRange(0, 100);
	slider.setValue(int(value / dpi));
	text.setText(fmt::sprintf("%.2g", value).c_str());

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
}

/**
 * @brief Updates slider on value change
 *
 * @param xvalue New value
 */
void FloatSlider::ValueChanged(int xvalue)
{
	value = xvalue * dpi;
	text.setText(fmt::sprintf("%.2g", value).c_str());

	// Rescale according to DPI
	if (xvalue == 100 && value < 20)
	{
		dpi = value < 10 ? (value * 2.0f) / 100.0f : 0.2f;
	} else if (xvalue <= 1)
	{
		dpi = 0.01f;
	}
	slider.setValue(int(value / dpi));
}

/**
 * @brief If value is set manually trough the text field
 *
 * @param text
 */
void FloatSlider::TextEdited(const QString& text)
{
	slider.setValue(int(text.toFloat() / dpi));
}

/**
 * @brief Callback to node on slider value change
 *
 * @param to
 */
void FloatSlider::SetChangedCallback(INode* to)
{
	connect(&slider, &QSlider::valueChanged, [this, to](int v)
		{
			ValueChanged(v);
			to->Update();
		});
}

/**
 * @brief Construct a new Vec 2 Slider:: Vec 2 Slider object
 *
 * @param value Vector values that is modified by the slider
 * @param min Minimum vector value
 * @param max Maximum vector value
 */
Vec2Slider::Vec2Slider(QVector2D& value, float min, float max)
	:value(value), upper(value[0], min, max), lower(value[1], min, max)
{
	lay.setAlignment(Qt::AlignmentFlag::AlignTop);
	setLayout(&lay);
	lay.addWidget(&upper);
	lay.addWidget(&lower);
}

/**
 * @brief Sets callback to both sliders in vector
 *
 * @param to New callback
 */
void Vec2Slider::SetChangedCallback(INode* to)
{
	upper.SetChangedCallback(to);
	lower.SetChangedCallback(to);
}


CheckBox::CheckBox(bool& value, const QString& name)
	:value(value), box(name)
{
	box.setChecked(value);
	lay.addWidget(&box);
	setLayout(&lay);
}
void CheckBox::SetChangedCallback(INode* to)
{
	connect(&box, &QCheckBox::stateChanged, [this, to](int v)
		{
			value = bool(v);
			to->Update();
		});
}