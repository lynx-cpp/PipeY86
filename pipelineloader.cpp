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

void PipelineLoader::setRegisterStatus()
{
    QVariantList list; list.clear();
    for (int i=0;i<8;i++)
        list.append(QString::fromStdString(int2Hex(m_pipeline->m_register[i],8)));
    QMetaObject::invokeMethod(root,"writeContainer",Q_ARG(QVariant,"register"),Q_ARG(QVariant,QVariant(list)));
}

void PipelineLoader::setMemoryStatus()
{
    QMetaObject::invokeMethod(root,"clearMemTable",Qt::QueuedConnection);
    MemorySeq seq;
    printMemory(m_pipeline->m_memory,seq);
    for (int i=0;i<seq.size();i++){
        std::string addr = int2Hex(seq[i].first,8);
        std::string data = int2Hex(seq[i].second,8);
        //qDebug() << "Memory : " << addr.c_str() << " " << data.c_str();
        QMetaObject::invokeMethod(root,"addMemoryElement",
                                  Qt::QueuedConnection,
                                  Q_ARG(QVariant,addr.c_str()),
                                  Q_ARG(QVariant,data.c_str()));
    }
}

void PipelineLoader::showStopDialog()
{
    QMetaObject::invokeMethod(root,"showStopDialog",Qt::QueuedConnection);
}

void PipelineLoader::readAllStage()
{
    if (m_pipeline==NULL) return ;
    for (int i=0;i<prog.size();i++){
        QString label = "";
        if (prog[i].addr()==m_pipeline->fetchI->addr()) label = "F";
        if (prog[i].addr()==m_pipeline->decodeI->addr()) label = "D";
        if (prog[i].addr()==m_pipeline->executeI->addr()) label = "E";
        if (prog[i].addr()==m_pipeline->memoryI->addr()) label = "M";
        if (prog[i].addr()==m_pipeline->writeBackI->addr()) label = "W";
        setStageLabel(i,label);
    }
}

PipelineLoader::PipelineLoader(QObject* parent): QObject(parent)
{
    m_pipeline = NULL;
    m_timer = new QTimer(this);
    interval = 200;
    m_timer->setSingleShot(false);
    cycle = 0;
    time = new QTime();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(step()));
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
    //disconnect(m_timer);
    cycle = 0;
    time->start();
    qDebug() << "written";
}

void PipelineLoader::load()
{
    qDebug() << "reset";
    pause();
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
    setStageStatus("fetch",m_pipeline->fetchI);
    setStageStatus("decode",m_pipeline->decodeI);
    setStageStatus("execute",m_pipeline->executeI);
    setStageStatus("memory",m_pipeline->memoryI);
    setStageStatus("writeback",m_pipeline->writeBackI);
    setRegisterStatus();
    setMemoryStatus();
}

void PipelineLoader::step()
{
    int cur = time->elapsed();
    static int last = time->elapsed();
    cycle ++;
    if (m_pipeline==NULL || !m_pipeline->loaded())
        return ;
    m_pipeline->setProgToThis();
   m_pipeline->execute();
   if (!m_pipeline->running()){
        showStopDialog();
        m_timer->stop();
        return ;
    }
    //clearInsTable();
    //addAllElement();
    readAllStage();
    refreshDisplay();
    qDebug() << "Cycle " << cycle << "elapsed time: "<< cur;
    last = cur;
}

void PipelineLoader::pause()
{
    m_timer->stop();
    //m_timer->setInterval(interval);
}

void PipelineLoader::start(int latency)
{
    interval = latency;
    m_timer->setInterval(latency);
    if (!m_timer->isActive()){
        m_timer->start();
    }
    qDebug() << "start with latency" << m_timer->interval();
}

void PipelineLoader::setLatency(int latency)
{
    interval = latency;
    m_timer->setInterval(latency);
    m_timer->setSingleShot(false);
    qDebug() << "set latency to " << m_timer->interval();
}

