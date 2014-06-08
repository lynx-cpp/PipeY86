#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlComponent>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickView>
#include <QtCore/QUrl>
#include <QVariant>
#include <QQuickItem>
#include <QMetaObject>
#include <QThread>
#include <QObject>

#include "y86pipeline.h"
#include "instruction.h"
#include "utility.h"
#include "pipelineloader.h"

Program prog;
Y86Pipeline* pipeline;
QQuickItem* root;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QQuickWindow::setDefaultAlphaBuffer(true);
    QQuickView view(QUrl("qrc:///qml/test.qml"));
    view.show();
    root = view.rootObject();
    QVariant returnValue;
    //QMetaObject::invokeMethod(root,"removeStageLabel",Q_RETURN_ARG(QVariant,returnValue),Q_ARG(QVariant,0));
    //QMetaObject::invokeMethod(root,"setStageLabel",Q_RETURN_ARG(QVariant,returnValue),Q_ARG(QVariant,1),Q_ARG(QVariant,"F"));
    QMetaObject::invokeMethod(root,"addElement",
                              Q_RETURN_ARG(QVariant,returnValue),
                              Q_ARG(QVariant,"0x02"),
                              Q_ARG(QVariant,"60 20"),
                              Q_ARG(QVariant,"M"),
                              Q_ARG(QVariant,"addl %edx,%eax"));
    PipelineLoader* pipeline = new PipelineLoader;
    QThread* pipelineThread = new QThread;
    QObject::connect(pipelineThread,SIGNAL(finished()),pipeline,SLOT(deleteLater()));
    QObject::connect(root,SIGNAL(test()),pipeline,SLOT(load()));
    QObject::connect(root,SIGNAL(load(QString)),pipeline,SLOT(loadFile(QString)));
    pipeline->moveToThread(pipelineThread);
    pipelineThread->start();
    return app.exec();
}