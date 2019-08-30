/*********************************************************************************
*Copyright(C):
*FileName:  dvrserial.h
*Author:    WuXiao
*Version:   1.0
*Date:      2019.08.30
*Description:  serial port communicate with dvr
*Others:
*Function List:
**********************************************************************************/


#ifndef DVRSERIAL_H
#define DVRSERIAL_H

#include <QObject>
#include <memory>

#include "serialmodule.h"

#pragma pack(push)
#pragma pack(1)
typedef union {
    struct cons{
        unsigned char year;
        unsigned char month;
        unsigned char day;
        unsigned char hour;
        unsigned char minute;
        unsigned char second;
        int latitude;
        int longitude;
        unsigned char latitudeLongitude;
        short direction;       //0-north  90-east  180-south 270-west
        short speed;           //km/h
        short reserve;
    }   construct;
    char data[sizeof(struct cons)];
} GPS_INFO;
#pragma pack(pop)


class DvrSerial :  public SerialModule
{
    Q_OBJECT
public:



    DvrSerial(QThread* thd, const QString& portName, const int& baud);

    void packageGpsInfo(const short& year,
                        const short& month,
                        const short& day,
                        const short& hour,
                        const short& minute,
                        const short& second,
                        const double& latitude,
                        const double& longitude,
                        const bool& northLatitude,
                        const bool& eastLongitude,
                        const short& direction,
                        const short& speed);
    void packageGpsInfo();




signals:

private:
    char checkSum(quint8 type, quint8 command, quint8 dataLength, const char *data);
    void parseDataTask() override;
    void packageMessageFrame(const int& type, const int& command, char* data, const int& dataLength);
    void parseMessage(const int& type, const int& command, const char* data, const int& frameLength, const int& check);

public:
    short year_;
    short month_;
    short day_;
    short hour_;
    short minute_;
    short second_;
    double latitude_;
    double longitude_;
    bool northLatitude_;    //true = north latitude
    bool eastLongitude_;    //true = east longitude
    short direction_;       //0-north  90-east  180-south 270-west
    short speed_;           //km/h
private slots:
    void testWriteData();

private:
    GPS_INFO gpsInfo_;
    static const char StartByte;
    static const char EndByte;


};

#endif // DVRSERIAL_H
