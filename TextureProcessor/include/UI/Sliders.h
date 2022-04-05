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
#include <QBoxLayout.h>

namespace ver::dc
{
	struct Options;
}

namespace UI
{
	using ver::dc::Options;

	struct INode;
	class FloatSlider : public IUpdater
	{
	public:
		FloatSlider(float& value, float min = -20, float max = 20, QWidget* parent = nullptr);
	public:
		void SetChangedCallback(INode* to)override;
		void SetMin(float m);
		void SetMax(float m);
		void Revalidate();
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
		float min;
		float max;
	};

	class Vec2Slider : public IUpdater
	{
	public:
		Vec2Slider(QVector2D& value, QWidget* parent = nullptr);
	public:
		void SetChangedCallback(INode* to)override;
		void SetMin(QVector2D& m);
		void SetMax(QVector2D& m);
		void Revalidate();
	private:
		QVBoxLayout lay;
		FloatSlider upper;
		FloatSlider lower;
		QVector2D& value;
	};

	class IntSlider : public IUpdater
	{
	public:
		IntSlider(int& value, QWidget* parent = nullptr);
	public:
		void SetChangedCallback(INode* to)override;
		void SetMin(int m);
		void SetMax(int m);
		void Revalidate();
	private:
		void ValueChanged(int value);
		void TextEdited(const QString& text);
	private:
		QHBoxLayout lay;
		QSlider slider;
		QLineEdit text;
		QIntValidator valid;

		int& value;
		int min = -20;
		int max = 20;
	};

	class CheckBox : public IUpdater
	{
	public:
		CheckBox(bool& value, const QString& name, QWidget* parent = nullptr);
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
