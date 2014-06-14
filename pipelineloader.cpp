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

static inline void setStageStatus(const QString& status,Instruction* ins)
{
    QVariantList list = ins->status();
    /*Q_FOREACH(QVariant i,list) {
        qDebug() << i;
    }*/
    QMetaObject::invokeMethod(root,"writeContainer",Q_ARG(QVariant,status),Q_ARG(QVariant,QVariant(list)));
}

static inline void removeStageLabel(int idx)
{
    QMetaObject::invokeMethod(root,"removeStageLabel",Qt::QueuedConnection,Q_ARG(QVariant,idx));
}

static inline void clearInsTable()
{
    QMetaObject::invokeMethod(root,"clearInsTable",Qt::QueuedConnection);
}

static inline void setStageLabel(int idx,const QString& label)
{
    QMetaObject::invokeMethod(root,"setStageLabel",Qt::QueuedConnection,Q_ARG(QVariant,idx),Q_ARG(QVariant,label));
}

static inline void addElement(const std::string& addr,const std::string& data,const std::string& stage,const std::string& code)
{
    QMetaObject::invokeMethod(root,"addElement",
                              Qt::QueuedConnection,
                              Q_ARG(QVariant,addr.c_str()),
                              Q_ARG(QVariant,data.c_str()),
                              Q_ARG(QVariant,stage.c_str()),
                              Q_ARG(QVariant,code.c_str()));
}

static inline void addAllElement()
{
     for (int i=0;i<prog.size();i++){
        addElement(int2Hex(prog[i].addr()),
                   hex2Data(prog[i].instructionCode()),
                   "",
                   prog[i].comment());
    }
}

void PipelineLoader::readAllStage()
{
    if (m_pipeline==NULL) return ;
    for (int i=0;i<prog.size();i++){
        if (prog[i].addr()==m_pipeline->fetchI->addr()) setStageLabel(i,"F");
        if (prog[i].addr()==m_pipeline->decodeI->addr()) setStageLabel(i,"D");
        if (prog[i].addr()==m_pipeline->executeI->addr()) setStageLabel(i,"E");
        if (prog[i].addr()==m_pipeline->memoryI->addr()) setStageLabel(i,"M");
        if (prog[i].addr()==m_pipeline->writeBackI->addr()) setStageLabel(i,"W");
    }
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
    clearInsTable();
    m_pipeline = new Y86Pipeline(m_filename.section('/',2).toStdString());
    qDebug() << "loaded";
    addAllElement();
    readAllStage();
    refreshDisplay();
    qDebug() << "written";
}

void PipelineLoader::load()
{
    qDebug() << "reset";
    loadFile(m_filename);
}

PipelineLoader::~PipelineLoader()
{
    delete m_pipeline;
}

void PipelineLoader::refreshDisplay()
{
    if (m_pipeline==NULL || !m_pipeline->loaded())
        return ;
    setStageStatus("decode",m_pipeline->decodeI);
    setStageStatus("execute",m_pipeline->executeI);
    setStageStatus("memory",m_pipeline->memoryI);
    setStageStatus("writeback",m_pipeline->writeBackI);
}

void PipelineLoader::step()
{
    if (m_pipeline==NULL || !m_pipeline->loaded())
        return ;
    m_pipeline->setProgToThis();
    m_pipeline->execute();
    clearInsTable();
    addAllElement();
    readAllStage();
    refreshDisplay();
}

