#include <QApplication>
#include <QQmlApplicationEngine>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine("mainwindow.qml");
    return app.exec();
}