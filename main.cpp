#include <QApplication>
#include "mainWindow/mainWindow.h"

int main(int argc, char *argv[])
{
	 QApplication app(argc, argv);

	 QCoreApplication::setOrganizationName("LisGein");
	 QCoreApplication::setOrganizationDomain("lisgein.com");
	 QCoreApplication::setApplicationName("Regression thesis");

	 MainWindow mainWindow;
	 mainWindow.show();
	 return app.exec();
}
