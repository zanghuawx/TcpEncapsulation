/*********************************************************************************
*Copyright(C):
*FileName:  serialmodule.h
*Author:    WuXiao
*Version:   1.0
*Date:      2019.08.30
*Description:  base class of serial port communicate
*Others:
*Function List:
**********************************************************************************/


#ifndef SERIALMODULE_H
#define SERIALMODULE_H

#include <QObject>
#include <QQueue>
#include <QByteArray>
#include <mutex>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QThreadPool>
#include <QDebug>

#include "qringbuffer.h"



class SerialModule : public QObject
{
    Q_OBJECT
public:
    SerialModule(QThread* thd, const QString& portName, const int& baud, QObject *parent = 0);
    ~SerialModule();

    void openPort();
    void closePort();

    int extractData(char* data, const int& length);
    virtual void packageMessageFrame(const int& type, const int& command, char* data, const int& dataLength);
    char checkSum(quint8 dataType, quint8 len, const char *data);

    void setParsePeriod(const int& per);
protected:
    virtual void parseDataTask();
private slots:
    void popQueue();
protected:
    void init();
protected slots:
    void readData();
    //-------------------------test-------------------//
    void testReadData();
    virtual void testWriteData();
private slots:
    int writeData(const char* data, const int& length);

signals:
    void writeSignal(const char* data, const int& length);

protected:
    QThread* thd_;
    QString portName_;
    int baud_;
    QSerialPort serial_;
    int parsePeriod_;
    std::mutex mtx_;
    QRingBuffer readBufQueue_;
    QTimer parseTimer_;

    QTimer testProduceTimer;
    QTimer testConsumeTimer;

    static const int MaxFrameBytes;
    static const char Head;
    static const char Rear;


};

#endif // SERIALMODULE_H
