#include <QThread>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QStandardPaths>
#include <QObject>
#include "record_process.h"

RecordProcess::RecordProcess(QObject *parent) : QThread(parent)
{
}

void RecordProcess::setRecordInfo(int rx, int ry, int rw, int rh)
{
    recordX = rx;
    recordY = ry;
    recordWidth = rw;
    recordHeight = rh;
}

void RecordProcess::setRecordType(int type) 
{
    recordType = type;
}

void RecordProcess::run()
{
    if (recordType == RECORD_TYPE_GIF) {
        recordGIF();
    } else if (recordType == RECORD_TYPE_VIDEO) {
        recordVideo();
    }
}

void RecordProcess::recordGIF() 
{
    process = new QProcess();
    connect(process, SIGNAL(finished(int)), process, SLOT(deleteLater()));

    QString path = "%1/Desktop/%2";
    QString savepath = path.arg(QDir::homePath()).arg("deepin-record.gif");

    QFile file(savepath);
    file.remove();

    QStringList arguments;
    arguments << QString("--duration=%1").arg(864000);
    arguments << QString("--x=%1").arg(recordX) << QString("--y=%1").arg(recordY);
    arguments << QString("--width=%1").arg(recordWidth) << QString("--height=%1").arg(recordHeight);
    arguments << savepath;

    process->start("byzanz-record", arguments);

    process->waitForFinished(-1);
    if (process->exitCode() !=0) {
        qDebug() << "error" << process->readAllStandardError();
    } else{
        qDebug() << "ok" << process->readAllStandardOutput() << process->readAllStandardError();
    }
}

void RecordProcess::recordVideo()
{
    process = new QProcess();
    connect(process, SIGNAL(finished(int)), process, SLOT(deleteLater()));

    QString path = "%1/Desktop/%2";
    QString savepath = path.arg(QDir::homePath()).arg("deepin-record.mp4");

    QFile file(savepath);
    file.remove();

    // FFmpeg need pass arugment split two part: -option value,
    // otherwise, it will report 'Unrecognized option' error.
    QStringList arguments;
    arguments << QString("-video_size");
    arguments << QString("%1x%2").arg(recordWidth).arg(recordHeight);
    arguments << QString("-framerate");
    arguments << QString("25");
    arguments << QString("-f");
    arguments << QString("x11grab");
    arguments << QString("-i");
    arguments << QString(":0.0+%1,%2").arg(recordX).arg(recordY);
    arguments << savepath;
    
    process->start("ffmpeg", arguments);

    process->waitForFinished(-1);
    if (process->exitCode() !=0) {
        qDebug() << "Error";
        foreach (auto line, (process->readAllStandardError().split('\n'))) {
            qDebug() << line;
        }
    } else{
        qDebug() << "OK" << process->readAllStandardOutput() << process->readAllStandardError();
    }
}    

void RecordProcess::stopRecord()
{
    process->terminate();
}