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
public slots:
    void loadFile(const std::string& filename);
    void load();

private:
    Y86Pipeline* m_pipeline;
    std::string m_filename;
};

#endif // PIPELINELOADER_H
