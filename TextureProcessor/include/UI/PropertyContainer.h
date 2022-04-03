#pragma once
#include <QListWidget>
#include <QComboBox>
#include <QToolButton>
#include <Logic/DynamicConstant.h>


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
		Editor(const ver::dc::Layout::Entry& entry, ver::dc::Options* opt);
		Editor(Editor&& in)noexcept
			:opt(in.opt)
		{
			;
		}
	public:
		ver::dc::Options* GetOption()const noexcept;
		bool Accept();
	private:
		QVBoxLayout vl;
		NoScrollCBox type;

		QLabel lName;
		QLineEdit name;
		QLabel lcname;
		QLineEdit code_name;
		QRegularExpressionValidator rev;

		QToolButton edit;

		std::unique_ptr<IEditorOption> editor;
		ver::dc::Options* opt;
	};

	class PropertyContainer : public QWidget
	{
	public:
		PropertyContainer(ver::dc::Layout buffer, std::span<ver::dc::Options> params);
	public:
		template <class Func>
		void SetSaveCallback(Func&& f)
		{
			connect(&saver.save, &QToolButton::pressed, std::forward<Func>(f));
		}
		template <class Func>
		void SetDiscardCallback(Func&& f)
		{
			connect(&saver.discard, &QToolButton::pressed, std::forward<Func>(f));
		}
		std::vector<ver::dc::Options> GatherOptions();
		bool Accept();
	private:
		QVBoxLayout vl;
		std::vector<Editor> editors;
		struct Saver : public QWidget
		{
			Saver();
			QHBoxLayout hl;
			QToolButton save;
			QToolButton discard;
		}saver;
	};
}