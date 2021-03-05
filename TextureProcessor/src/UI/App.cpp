#include <UI/App.h>

App::App(int argc, char** argv)
{
	QApplication app(argc, argv);
	QCoreApplication::setApplicationName(AppName.data());
	QCoreApplication::setApplicationVersion(AppVer.data());
	qApp->setStyle(QStyleFactory::create(AppTheme.data()));

	window.emplace();
}

int App::Start()
{
	return qApp->exec();
}