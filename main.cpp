#include <QApplication>
#include <QQmlApplicationEngine>
#include "ShapefileView.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	qmlRegisterType<ShapefileView>("ShapefileView", 1, 0, "ShapefileView");

    QQmlApplicationEngine engine("mainwindow.qml");
    return app.exec();
}