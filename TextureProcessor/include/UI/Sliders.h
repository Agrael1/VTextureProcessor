/**
 * @file Sliders.h
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Controls for properties and constant buffers
 */
#pragma once
#include <UI/Updater.h>
#include <QCheckBox>
#include <QSlider>
#include <QLineEdit>
#include <QValidator>
#include <QBoxLayout>

namespace UI
{
	struct INode;
	class FloatSlider : public IUpdater
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

	class Vec2Slider : public IUpdater
	{
	public:
		Vec2Slider(QVector2D& value, float min = 0.0f, float max = 20.0f);
	public:
		void SetChangedCallback(INode* to)override;
	private:
		QVBoxLayout lay;
		FloatSlider upper;
		FloatSlider lower;
		QVector2D& value;
	};

	class IntSlider : public IUpdater
	{
	public:
		IntSlider(int& value);
	public:
		void SetChangedCallback(INode* to)override;
	private:
		void ValueChanged(int value);
		void TextEdited(const QString& text);
	private:
		QHBoxLayout lay;
		QSlider slider;
		QLineEdit text;
		QIntValidator valid;
		int& value;
	};

	class CheckBox : public IUpdater
	{
	public:
		CheckBox(bool& value, const QString& name);
	public:
		void SetChangedCallback(INode* to)override;
	private:
		bool& value;
		QVBoxLayout lay;
		QCheckBox box;
	};
	class RefreshCheckBox : public PropertyUpdater
	{
	public:
		RefreshCheckBox(bool& value, const QString& name);
	public:
		void SetChangedCallback(INode* to, Windows::PropertyElement& self)override;
	private:
		bool& value;
		QVBoxLayout lay;
		QCheckBox box;
	};
}
