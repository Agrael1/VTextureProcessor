/**
 * @file Sliders.h
 * @author Ilya Doroshenko (xdoros01)
 * @brief Controls for properties and constant buffers
 */
#pragma once
#include <UI/AwareWidget.h>
#include <QCheckBox>
#include <QSlider>
#include <QLineEdit>
#include <QValidator>
#include <QBoxLayout>

namespace UI
{
	class FloatSlider : public AwareWidget
	{
		constexpr static auto precision = 1000;
		constexpr static auto fprecision = float(precision);
	public:
		FloatSlider(float& value, const QString& name, float min = -20, float max = 20);
	public:
		void SetMin(float min);
		void SetMax(float max);
	private:
		QVBoxLayout vl;
		QHBoxLayout lay;
		QSlider slider;
		QLineEdit text;
		QDoubleValidator valid;
		float& val;
		float dpi;
		float max;
		float min;
	};

	class IntSlider : public AwareWidget
	{
	public:
		IntSlider(int& value, const QString& name, int min = -20, int max = 20);
	public:
		void SetMin(int min);
		void SetMax(int max);
	private:
		QVBoxLayout vl;
		QHBoxLayout lay;
		QSlider slider;
		QLineEdit text;
		QIntValidator valid;
		int min;
		int max;
	};

	class CheckBox : public AwareWidget
	{
	public:
		CheckBox(bool& value, const QString& name);
	private:
		QVBoxLayout lay;
		QCheckBox box;
	};

	class Matrix : public AwareWidget
	{
	public:
		Matrix(QMatrix4x4& value, const QString& name);
	private:
		QGridLayout lay;
		QDoubleValidator valid;
	};
}
