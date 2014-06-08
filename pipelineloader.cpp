#include <QQuickItem>
#include <QMetaObject>
#include <QDebug>
#include <string>
#include <cstdlib>
#include "pipelineloader.h"
#include "utility.h"
#include "instruction.h"

extern QQuickItem* root;
QVariant returnValue;
static inline void addElement(const std::string& addr,const std::string& data,const std::string& stage,const std::string& code)
{
    QMetaObject::invokeMethod(root,"addElement",
                              Qt::QueuedConnection,
                              Q_ARG(QVariant,addr.c_str()),
                              Q_ARG(QVariant,data.c_str()),
                              Q_ARG(QVariant,stage.c_str()),
                              Q_ARG(QVariant,code.c_str()));
}

PipelineLoader::PipelineLoader(QObject* parent): QObject(parent)
{
    m_pipeline = NULL;
}

void PipelineLoader::loadFile(const QString& filename)
{
    m_filename = filename;
    if (m_pipeline!=NULL) 
        delete m_pipeline;
    qDebug() << "entered" << " " << filename;
    m_pipeline = new Y86Pipeline(m_filename.section('/',2).toStdString());
    qDebug() << "loaded";
    for (int i=0;i<prog.size();i++){
        addElement(int2Hex(prog[i].addr()),
                   hex2Data(prog[i].instructionCode()),
                   "",
                   prog[i].comment());
    }
    qDebug() << "writed";
}

void PipelineLoader::load()
{
    qDebug() << "hehe";
    loadFile("./1.yo");
}

PipelineLoader::~PipelineLoader()
{
    delete m_pipeline;
}

