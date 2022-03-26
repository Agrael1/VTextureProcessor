/**
 * @file Sliders.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Controls for properties and constant buffers
 */
#include <UI/Sliders.h>
#include <Interfaces/INode.h>
#include <Logic/DynamicConstant.h>
#include <QVector2D>


using namespace UI;

/**
 * @brief Construct a new Float Slider:: Float Slider object
 *
 * @param value Value changed by the slider
 * @param min Minimum value
 * @param max Maximum value
 */
FloatSlider::FloatSlider(float& value, float min, float max, QWidget* parent)
	:slider(Qt::Horizontal), value(value), valid(min, max, 2), Updater(parent), min(min), max(max)
{
	dpi = (max - min) / 100.0f;

	setLayout(&lay);
	text.setValidator(&valid);
	text.connect(&text, &QLineEdit::textEdited, this, &FloatSlider::TextEdited);

	// Display with 2 floating point precision
	slider.setRange(0, 100);
	slider.setValue(int(value / dpi));
	text.setText(std::format("{:.2f}", value).c_str());

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
	text.setText(std::format("{:.2f}", value).c_str());

	// Rescale according to DPI
	if (xvalue == 100 && value < max)
	{
		dpi = value < 10 ? (max - min) / 100.0f : 0.2f;
	}
	else if (xvalue <= 1)
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

void UI::FloatSlider::SetMin(float m)
{
	valid.setBottom(m);
	min = m;
}

void UI::FloatSlider::SetMax(float m)
{
	valid.setTop(m);
	max = m;
}

void UI::FloatSlider::Revalidate()
{
	dpi = (max - min) / 100.0f;
	slider.setValue(int(value / dpi));
}

/**
 * @brief Construct a new Vec 2 Slider:: Vec 2 Slider object
 *
 * @param value Vector values that is modified by the slider
 * @param min Minimum vector value
 * @param max Maximum vector value
 */
Vec2Slider::Vec2Slider(QVector2D& value, QWidget* parent)
	:value(value), upper(value[0]), lower(value[1]), Updater(parent)
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

void UI::Vec2Slider::SetMin(QVector2D& m)
{
	upper.SetMin(m[0]);
	lower.SetMin(m[1]);
}

void UI::Vec2Slider::SetMax(QVector2D& m)
{
	upper.SetMax(m[0]);
	lower.SetMax(m[1]);
}

void UI::Vec2Slider::Revalidate()
{
	upper.Revalidate();
	lower.Revalidate();
}



/**
 * @brief Construct a new Float Slider:: Float Slider object
 *
 * @param value Value changed by the slider
 * @param min Minimum value
 * @param max Maximum value
 */
IntSlider::IntSlider(int& value, QWidget* parent)
	:slider(Qt::Horizontal), value(value), valid(0, 20), Updater(parent)
{
	setLayout(&lay);
	text.setValidator(&valid);
	text.connect(&text, &QLineEdit::textEdited, this, &IntSlider::TextEdited);

	// Display with 2 floating point precision
	slider.setRange(min, max);
	slider.setValue(value);
	text.setText(QString::number(value));

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
void IntSlider::ValueChanged(int xvalue)
{
	value = xvalue;
	text.setText(std::format("{}", value).c_str());
	slider.setValue(value);
}

/**
 * @brief If value is set manually trough the text field
 *
 * @param text
 */
void IntSlider::TextEdited(const QString& text)
{
	slider.setValue(text.toInt());
}

/**
 * @brief Callback to node on slider value change
 *
 * @param to
 */
void IntSlider::SetChangedCallback(INode* to)
{
	connect(&slider, &QSlider::valueChanged, [this, to](int v)
		{
			ValueChanged(v);
			to->Update();
		});
}
void IntSlider::SetMin(int m)
{
	valid.setBottom(m);
	min = m;
}
void IntSlider::SetMax(int m)
{
	valid.setTop(m);
	max = m;
}
void IntSlider::Revalidate()
{
	slider.setRange(min, max);
	slider.setValue(value);
}


/**
 * @brief Creates value dependent check box
 * @param value ref to changed value
 * @param name Label to go with
*/
CheckBox::CheckBox(bool& value, const QString& name, QWidget* parent)
	:value(value), box(name), Updater(parent)
{
	box.setChecked(value);
	lay.addWidget(&box);
	setLayout(&lay);
}

/**
 * @brief Sets check box callback
 * @param to calls update upon
*/
void CheckBox::SetChangedCallback(INode* to)
{
	connect(&box, &QCheckBox::stateChanged, [this, to](int v)
		{
			value = bool(v);
			to->Update();
		});
}

UI::RefreshCheckBox::RefreshCheckBox(bool& value, const QString& name)
	:value(value), box(name)
{
	box.setChecked(value);
	lay.addWidget(&box);
	setLayout(&lay);
}


void UI::RefreshCheckBox::SetChangedCallback(INode* to, Windows::PropertyElement& self)
{
	connect(&box, &QCheckBox::stateChanged, [this, to, &self](int v)
		{
			value = bool(v);
			to->UpdateProperties(self);
		});
}