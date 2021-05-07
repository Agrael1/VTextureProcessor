#include <UI/App.h>

App::App(int argc, char** argv)
	:app(argc, argv)
{
    // TODO: Calling AppName and AppVer causes a segfault on Linux
	QCoreApplication::setApplicationName(AppName.data());
	QCoreApplication::setApplicationVersion(AppVer.data());
	app.setStyle(QStyleFactory::create(AppTheme.data()));
    app.setWindowIcon(QIcon{ ":/tlr.ico" });

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    app.setPalette(darkPalette);

	window.emplace(1280, 720);
	window->ShowMaximized();
}

int App::Start()
{
	return app.exec();
}