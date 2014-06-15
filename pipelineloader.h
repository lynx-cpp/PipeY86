#ifndef PIPELINELOADER_H
#define PIPELINELOADER_H
#include <QObject>
#include <QStack>
#include <QTimer>
#include <QTime>

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
    void pause();
    void setLatency(int latency);

private:
    void setRegisterStatus();
    void setMemoryStatus();
    void showStopDialog();
    int interval;
    int cycle;
    QTime* time;
    QTimer* m_timer;
    Y86Pipeline* m_pipeline;
    QVector < Y86Pipeline > history;
    QString m_filename;
};

#endif // PIPELINELOADER_H
