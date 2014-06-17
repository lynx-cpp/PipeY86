#ifndef PIPELINELOADER_H
#define PIPELINELOADER_H
#include <QObject>
#include <QStack>
#include <QTimer>
#include <QTime>
#include <QSet>

#include "y86pipeline.h"

class PipelineLoader : public QObject
{
    Q_OBJECT

public:
    explicit PipelineLoader(QObject* parent = 0);
    ~PipelineLoader();
    void refreshDisplay();
    void readAllStage();
public slots:
    void loadFile(const QString& filename);
    void load();
    void step();
    void back();
    void start(int latency);
    void fastStart();
    void pause();
    void setLatency(int latency);
    void setBreakPoint(int row);
    void unsetBreakPoint(int row);

private slots:
    void fastStep();
    
private:
    void setRegisterStatus();
    void setMemoryStatus();
    void showStopDialog();
    
    int interval;
    int cycle;
    QTime* time;
    QTimer* m_timer;
    QTimer* fastTimer;
    
    QSet < int > breakPoints;
    
    Y86Pipeline* m_pipeline;
    QStack < Y86Pipeline > history;
    QString m_filename;
};

#endif // PIPELINELOADER_H
