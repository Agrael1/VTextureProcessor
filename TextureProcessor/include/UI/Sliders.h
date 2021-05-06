#pragma once
#include <UI/Updater.h>

namespace UI
{
	struct INode;
	class FloatSlider : public Updater
	{
	public:
		FloatSlider(float& value, float min = 0.0f, float max = 20.0f);
	public:
		void SetChangedCallback(INode* to)override;
	private:
		void ValueChanged(int value);
		void TextEdited(const QString& text);
	private:
		QHBoxLayout lay;
		QSlider slider;
		QLineEdit text;
		QDoubleValidator valid;
		float& value;
		float dpi;
	};
}
