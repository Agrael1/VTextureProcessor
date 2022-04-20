#pragma once
#include <Logic/ApplicationConfig.h>

#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QValidator>




class ApplicationConfig;

namespace UI
{
	class CreatePage : public QWidget
	{
	private:
		static constexpr auto proj_def_name = ver::project<"TextureProject">();
		static constexpr auto size_min = 16u;
		static constexpr auto size_min_s = ver::u16int2str<size_min>();
		static constexpr auto size_std_s = ver::u16int2str<256>();
	public:
		CreatePage(QWidget* app);
	protected:
		void OnCreateClicked(bool checked);
		void OnCancelClicked(bool checked);
		void OnLink(bool checked);
		void OnFolderChange(bool checked);
		void OnWidthEChanged();
		void OnHeightEChanged();
		void OnWidthChanged(const QString& text);
		void OnHeightChanged(const QString& text);
	private:
		QWidget* parent;
		ApplicationConfig cfg;
		QVBoxLayout lay;

		QLabel name;
		QLabel project_name;
		QLineEdit pname;

		QLabel project_folder;
		QHBoxLayout fl;
		QLineEdit pfolder;
		QPushButton search;

		QIntValidator size_v;
		QIcon locked{ u":/icons8-link.png"_qs };
		QIcon unlocked{ u":/icons8-broken-link.png"_qs };
		QLabel tex_size;
		QHBoxLayout szl;
		QLineEdit xwidth;
		QPushButton lock;
		QLineEdit xheight;

		QHBoxLayout bl;
		QPushButton cancel;
		QPushButton create;
	};
}
