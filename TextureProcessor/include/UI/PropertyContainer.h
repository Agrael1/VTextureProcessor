#pragma once
#include <QListWidget>
#include <QComboBox>
#include <QToolButton>
#include <Logic/DynamicConstant.h>


namespace UI
{
	class Editor : public QWidget
	{
	public:
		Editor(const ver::dc::Layout::Entry& entry, ver::dc::Options* opt);
		Editor(Editor&& in)noexcept
			:opt(in.opt)
		{
			;
		}
		void OnEdit();

	private:
		QVBoxLayout vl;
		QComboBox type;

		QLabel lName;
		QLineEdit name;
		QLabel lcname;
		QLineEdit code_name;

		QToolButton edit;

		std::unique_ptr<QWidget> editor;
		QHBoxLayout hl;
		QToolButton save;
		QToolButton discard;

		ver::dc::Options* opt;
	};

	class PropertyContainer : public QWidget
	{
	public:
		PropertyContainer(ver::dc::Layout buffer, std::span<ver::dc::Options> params);
	private:
		QVBoxLayout vl;
		std::vector<Editor> editors;
	};
}