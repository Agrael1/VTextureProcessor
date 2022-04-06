#pragma once
#include <QComboBox>
#include <QToolButton>
#include <Logic/DynamicConstant.h>
#include <functional>


namespace UI
{
	namespace Windows
	{
		class PropertyElement;
	}

	struct IEditorOption : public QWidget
	{
		virtual ver::dc::Options& GetOption() noexcept = 0;
	};

	class Editor : public QWidget
	{
		struct NoScrollCBox : public QComboBox
		{
			void wheelEvent(QWheelEvent* e) override
			{}
		};
	public:
		Editor(size_t position);
		Editor(const ver::dc::Layout::Entry& entry, ver::dc::Options* opt, size_t position);
	public:
		ver::dc::Options* GetOption()const noexcept;
		ver::dc::Layout::Entry GetEntry()const noexcept;
		template<typename Func>
		void SetDesctructCallback(Func&& f)
		{
			connect(&remove, &QToolButton::pressed, [this, f]() {f(position); });
		}
		bool Accept();
		void UpdatePosition()
		{
			position--;
		}
	private:
		void Init();
	private:
		QVBoxLayout vl;
		QHBoxLayout hl;
		NoScrollCBox type;
		QToolButton remove;

		QLabel lName{"Name:"};
		QLineEdit name;
		QLabel lcname{"Code Name:"};
		QLineEdit code_name;
		QRegularExpressionValidator rev{ QRegularExpression{ "^[_a-zA-Z]\\w*$" } };

		QToolButton edit;

		std::unique_ptr<IEditorOption> editor;
		ver::dc::Options* opt;
		size_t position = 0;
	};

	class PropertyContainer : public QWidget
	{
	public:
		PropertyContainer(ver::dc::Layout buffer, std::span<ver::dc::Options> params);
		~PropertyContainer();
	public:
		template <class Func>
		void SetSaveCallback(Func&& f)
		{
			saver.c_save = f;
		}
		template <class Func>
		void SetDiscardCallback(Func&& f)
		{
			saver.c_discard = f;
		}
		std::vector<ver::dc::Options> GatherOptions();
		ver::dc::Layout GatherLayout();
		bool Accept();
	private:
		void RemoveElement(size_t xposition);
	private:
		QVBoxLayout vl2;
		QVBoxLayout vl;
		QHBoxLayout hl;
		QToolButton add;
		std::vector<std::unique_ptr<Editor>> editors;
		struct Saver : public QWidget
		{
			Saver();
			QHBoxLayout hl;
			QToolButton save;
			QToolButton discard;
			std::function<void()> c_save;
			std::function<void()> c_discard;
		}saver;
	};
}