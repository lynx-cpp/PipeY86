#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlComponent>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickView>
#include <QtCore/QUrl>

#include "y86pipeline.h"
#include "instruction.h"
#include "utility.h"

Program prog;
Y86Pipeline* pipeline;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QQuickWindow::setDefaultAlphaBuffer(true);
    QQuickView view(QUrl("qrc:///qml/test.qml"));
    view.show();
    return app.exec();
}