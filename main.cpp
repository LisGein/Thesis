#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include "mainWindow/mainWindow.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("LisGein");
	QCoreApplication::setOrganizationDomain("lisgein.com");
	QCoreApplication::setApplicationName(QObject::tr("Regression thesis"));

	QTranslator myTranslator;
	myTranslator.load("./Diploma_ru.qm");
	app.installTranslator(&myTranslator);

	MainWindow mainWindow;
	mainWindow.show();
	return app.exec();
}
