#pragma once
#include <QBoxLayout>
#include <QToolButton>
#include <utils/string_literal.h>

namespace UI::Internal
{
	/**
	  * @brief represents non-client area of a custom window
	 */
	class FrameControl : public QWidget
	{
	public:
		FrameControl(QWidget* parent);
	public:
		void Restore() { OnRestoreClicked(false); }
		QVBoxLayout& Layout()noexcept { return lay; }
	protected:
		void OnCloseClicked(bool checked)
		{
			parentWidget()->close();
		}
		void OnMaximizedClicked(bool checked)
		{
			maximize.setIcon(QIcon{ u":/icon_window_restore.png"_qs });
			connect(&maximize, &QToolButton::clicked, this, &FrameControl::OnRestoreClicked);
			parentWidget()->showMaximized();
		}
		void OnRestoreClicked(bool checked)
		{
			maximize.setIcon(QIcon{ u":/icon_window_maximize.png"_qs });
			connect(&maximize, &QToolButton::clicked, this, &FrameControl::OnMaximizedClicked);
			parentWidget()->showNormal();
		}
		void OnMinimizedClicked(bool checked)
		{
			parentWidget()->showMinimized();
		}
	private:
		QVBoxLayout lay;
		QHBoxLayout buttons;
		QToolButton close;
		QToolButton minimize;
		QToolButton maximize;

		static constexpr ver::u16string_literal style = uR"(QToolButton{
			background-color:none;
			border:none;
			width:25px;
			height:16px;
			padding:4px;
		}
		QToolButton:hover{
			background-color:gray;
		}
		QToolButton:pressed{
			background-color:palette(highlight);
		})";
		static constexpr ver::u16string_literal closestyle = uR"(QToolButton{
			background-color:none;
			border:none;
			width:25px;
			height:16px;
			padding:4px;
		}
		QToolButton:hover{
			background-color:red;
		}
		QToolButton:pressed{
			background-color:palette(highlight);
		})";
	};
}
