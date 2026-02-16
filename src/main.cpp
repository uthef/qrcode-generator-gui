#include <QApplication>
#include <QIcon>
#include <QDir>
#include "widgets/mainwindow.hpp"

using namespace QrCodeGeneratorGui;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    auto appIcon = QIcon(":/images/qr-icon.png");
    app.setWindowIcon(appIcon);

    QString defaultImagePath = app.applicationDirPath();

    QDir homePath = QDir::homePath();
    QString picturesDir = homePath.absoluteFilePath("Pictures");

    if (QDir(picturesDir).exists())
        defaultImagePath = picturesDir;

    MainWindow mainWindow(defaultImagePath);
    mainWindow.show();

    return app.exec();
}