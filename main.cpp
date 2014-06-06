#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlComponent>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickView>
#include <QtCore/QUrl>
#include <QVariant>
#include <QQuickItem>
#include <QMetaObject>

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
    QQuickItem* root = view.rootObject();
    QVariant returnValue;
    //QMetaObject::invokeMethod(root,"removeStageLabel",Q_RETURN_ARG(QVariant,returnValue),Q_ARG(QVariant,0));
    //QMetaObject::invokeMethod(root,"setStageLabel",Q_RETURN_ARG(QVariant,returnValue),Q_ARG(QVariant,1),Q_ARG(QVariant,"F"));
    QMetaObject::invokeMethod(root,"addElement",
                              Q_RETURN_ARG(QVariant,returnValue),
                              Q_ARG(QVariant,"0x02"),
                              Q_ARG(QVariant,"60 20"),
                              Q_ARG(QVariant,"M"),
                              Q_ARG(QVariant,"addl %edx,%eax"));
    return app.exec();
}