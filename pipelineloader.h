#ifndef PIPELINELOADER_H
#define PIPELINELOADER_H
#include <QObject>

#include "y86pipeline.h"

class PipelineLoader : public QObject
{
    Q_OBJECT

public:
    explicit PipelineLoader(QObject* parent = 0);
    ~PipelineLoader();
    void refreshDisplay();
public slots:
    void loadFile(const QString& filename);
    void load();
    void step();

private:
    Y86Pipeline* m_pipeline;
    QString m_filename;
};

#endif // PIPELINELOADER_H
