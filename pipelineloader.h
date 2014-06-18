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

#ifndef PIPELINELOADER_H
#define PIPELINELOADER_H
#include <QObject>
#include <QStack>
#include <QTimer>
#include <QTime>
#include <QSet>
#include <QTemporaryFile>
#include <fstream>

#include "y86pipeline.h"
#include "s2o/assembler.h"

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
    void printStageStatus();

    int interval;
    int cycle;
    QTime* time;
    QTimer* m_timer;
    QTimer* fastTimer;
    
    AssemblerY86* assembler;
    QTemporaryFile* tmpFile;

    QSet < int > breakPoints;

    Y86Pipeline* m_pipeline;
    QStack < Y86Pipeline > history;
    QString m_filename;
    
    
    std::fstream stream;
};

#endif // PIPELINELOADER_H
