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
#include <QList>
#include <QVariant>

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
                              Q_ARG(QVariant,"E"),
                              Q_ARG(QVariant,"addl %edx,%eax"));
    PipelineLoader* pipeline = new PipelineLoader;
    QThread* pipelineThread = new QThread;
    pipeline->moveToThread(pipelineThread);
    QObject::connect(pipelineThread,SIGNAL(finished()),pipeline,SLOT(deleteLater()),Qt::QueuedConnection);
    QObject::connect(root,SIGNAL(reset()),pipeline,SLOT(load()),Qt::QueuedConnection);
    QObject::connect(root,SIGNAL(load(QString)),pipeline,SLOT(loadFile(QString)),Qt::QueuedConnection);
    QObject::connect(root,SIGNAL(step()),pipeline,SLOT(step()),Qt::QueuedConnection);
    QObject::connect(root,SIGNAL(start(int)),pipeline,SLOT(start(int)),Qt::QueuedConnection);
    QObject::connect(root,SIGNAL(pause()),pipeline,SLOT(pause()),Qt::QueuedConnection);
    QObject::connect(root,SIGNAL(setLatency(int)),pipeline,SLOT(setLatency(int)),Qt::QueuedConnection);
    QObject::connect(root,SIGNAL(back()),pipeline,SLOT(back()),Qt::QueuedConnection);
    pipelineThread->start();
    return app.exec();
}