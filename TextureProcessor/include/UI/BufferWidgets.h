//#pragma once
//#include <Logic/DynamicConstant.h>
//#include <UI/Sliders.h>
//#include <QLabel>
//
//
//namespace UI
//{
//	namespace Windows
//	{
//		class PropertyElement;
//	}
//
//	template<ver::dc::Type t>
//	class BufferWidget{};
//
//	template<>
//	class BufferWidget<ver::dc::Type::Float> : public IUpdater
//	{
//	public:
//		BufferWidget(ver::dc::ElementRef ref, float min = -20, float max = 20)
//			:sl((float&)ref, min, max), l(ref.GetName().data())
//		{
//
//		}
//	public:
//		virtual void SetChangedCallback(INode* to) override
//		{
//			sl.SetChangedCallback(to);
//		}
//		void SetMin(float m)
//		{
//			sl.SetMin(m);
//		}
//		void SetMax(float m)
//		{
//			sl.SetMax(m);
//		}
//		void Revalidate();
//	private:
//		void ValueChanged(int value);
//		void TextEdited(const QString& text);
//	private:
//		QVBoxLayout vl;
//		QLabel l;
//		FloatSlider sl;
//	};
//}
