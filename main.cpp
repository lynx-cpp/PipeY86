/*
 *   Copyright (C) 2014 by Yuquan Fang<lynx.cpp@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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
#include <QIcon>
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
    //app.setWindowIcon(QIcon("qrc:///icon.png"));
    QQuickWindow::setDefaultAlphaBuffer(true);
    QQuickView view(QUrl("qrc:///qml/test.qml"));
    view.setIcon(QIcon(":/icon.png"));
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
    QObject::connect(root,SIGNAL(setBreakPoint(int)),pipeline,SLOT(setBreakPoint(int)),Qt::QueuedConnection);
    QObject::connect(root,SIGNAL(unsetBreakPoint(int)),pipeline,SLOT(unsetBreakPoint(int)),Qt::QueuedConnection);
    QObject::connect(root,SIGNAL(fastStart()),pipeline,SLOT(fastStart()),Qt::QueuedConnection);
    pipelineThread->start();
    return app.exec();
}
