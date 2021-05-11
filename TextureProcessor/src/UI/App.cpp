/**
 * @file App.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Class for loading project view initialization
 */
#include <UI/App.h>
#include <UI/ProjectEvent.h>

/**
 * @brief Construct a new App:: App object
 *
 * @param argc argument count
 * @param argv argument list
 */
App::App(int argc, char** argv)
	:app(argc, argv)
{
    // Basic setup of the application
	QCoreApplication::setApplicationName(Window::AppName.data());
	QCoreApplication::setApplicationVersion(AppVer.data());
	app.setStyle(QStyleFactory::create(AppTheme.data()));
    app.setWindowIcon(QIcon{ ":/tlr.ico" });

    // Style definition (darkmode)
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

    // Event type for opening projects
    QEvent::registerEventType(QEvent::User+1);

    // Set window size
    projects.emplace(1280, 720, *this);
    projects->show();
}

/**
 * @brief Starts the application
 *
 * @return int return code of the application
 */
int App::Start()
{
	return app.exec();
}

/**
 * @brief QEvent handler, mainly used for handling 'open project' events
 *
 * @param e Captured event
 */
bool App::event(QEvent* e)
{
    // Handling of opening projects
    if (e->type() == QEvent::User+1) {
        ProjectEvent& proj = static_cast<ProjectEvent&>(*e);
        projects.reset();  // Destroys project selection window
        window.emplace(1280, 720, std::move(proj.projPath));
        window->ShowMaximized();
    }
    return true;
}
