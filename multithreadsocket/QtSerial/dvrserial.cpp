/*********************************************************************************
*Copyright(C):
*FileName:  dvrserial.cpp
*Author:    WuXiao
*Version:   1.0
*Date:      2019.08.30
*Description:  serial port communicate with dvr
*Others:
*Function List:
**********************************************************************************/


#include <arpa/inet.h>

#include "dvrserial.h"


//DvrSerial::DvrSerial(QObject *parent) : QObject(parent)
//   /*serialPtr_(std::make_shared<SerialModule>(&thd_, QString("dvrport"), QSerialPort::Baud19200))*/
//{

//}

DvrSerial::DvrSerial(QThread *thd, const QString &portName, const int &baud) :
    SerialModule(thd, portName, baud)
{

}

void DvrSerial::packageGpsInfo(const short &year, const short &month, const short &day, const short &hour, const short &minute, const short &second, const double &latitude, const double &longitude, const bool &northLatitude, const bool &eastLongitude, const short &direction, const short &speed)
{
    qDebug() << "DvrSerial::packageGpsInfo--- " ;
    year_ = year;
    month_ = month;
    day_ = day;
    hour_ = hour;
    minute_ = minute;
    second_ = second;
    latitude_ = latitude;
    longitude_ = longitude;
    northLatitude_ = northLatitude;
    eastLongitude_ = eastLongitude;
    direction_ = direction;
    speed_ = speed;

    int type = 0;
    int command = 0;
    gpsInfo_.construct.year = year;
    gpsInfo_.construct.month = month;
    gpsInfo_.construct.day = day;
    gpsInfo_.construct.hour = hour;
    gpsInfo_.construct.minute = minute;
    gpsInfo_.construct.second = second;
    gpsInfo_.construct.latitude = htonl((int)(latitude * 60 * 60 * 256));
    gpsInfo_.construct.longitude = htonl((int)(longitude * 60 * 60 * 256));
    gpsInfo_.construct.latitudeLongitude = (northLatitude == true ? 0x01 : 0);
    gpsInfo_.construct.latitudeLongitude |= (eastLongitude == true ? 0x02 : 0);
    gpsInfo_.construct.direction = htons(direction);
    gpsInfo_.construct.speed = htons(speed);
    gpsInfo_.construct.reserve = 0;

    qDebug() << "DvrSerial::packageGpsInfo--- year:" << gpsInfo_.construct.year <<
                " month:" << gpsInfo_.construct.month << " day" << gpsInfo_.construct.day <<
                " hour:" << gpsInfo_.construct.hour <<
                " minute" << gpsInfo_.construct.minute << " second:" << gpsInfo_.construct.second <<
                " latitude" << gpsInfo_.construct.latitude << " longitude:" << gpsInfo_.construct.longitude <<
                " latitudeLongitude:" << gpsInfo_.construct.latitudeLongitude <<
                " direction:" << gpsInfo_.construct.direction << " speed" << gpsInfo_.construct.speed <<
                " sizeof(gpsInfo_):" << sizeof(gpsInfo_);


    packageMessageFrame(type, command, &gpsInfo_.data[0], sizeof(gpsInfo_));
}

void DvrSerial::packageGpsInfo()
{
    int type = 0;
    int command = 0;
    gpsInfo_.construct.year = year_;
    gpsInfo_.construct.month = month_;
    gpsInfo_.construct.day = day_;
    gpsInfo_.construct.hour = hour_;
    gpsInfo_.construct.minute = minute_;
    gpsInfo_.construct.second = second_;
    gpsInfo_.construct.latitude = htonl((int)(latitude_ * 60 * 60 * 256));
    gpsInfo_.construct.longitude = htonl((int)(longitude_ * 60 * 60 * 256));
    gpsInfo_.construct.latitudeLongitude = (northLatitude_ == true ? 0x01 : 0);
    gpsInfo_.construct.latitudeLongitude |= (eastLongitude_ == true ? 0x02 : 0);
    gpsInfo_.construct.direction = htons(direction_);
    gpsInfo_.construct.speed = htons(speed_);
    gpsInfo_.construct.reserve = 0;
    qDebug() << "DvrSerial::packageGpsInfo--- year:" << gpsInfo_.construct.year <<
                " month:" << gpsInfo_.construct.month << " day" << gpsInfo_.construct.day <<
                " hour:" << gpsInfo_.construct.hour <<
                " minute" << gpsInfo_.construct.minute << " second:" << gpsInfo_.construct.second <<
                " latitude" << gpsInfo_.construct.latitude << " longitude:" << gpsInfo_.construct.longitude <<
                " latitudeLongitude:" << gpsInfo_.construct.latitudeLongitude <<
                " direction:" << gpsInfo_.construct.direction << " speed" << gpsInfo_.construct.speed <<
                " sizeof(gpsInfo_):" << sizeof(gpsInfo_);
    packageMessageFrame(type, command, &gpsInfo_.data[0], sizeof(gpsInfo_));
}







void DvrSerial::packageMessageFrame(const int &type, const int &command, char *data, const int &dataLength)
{
    qDebug() << " DvrSerial::packageMessageFrame---";
    char buf[MaxFrameBytes];
    buf[0] = Head;
    buf[1] = (char)dataLength;
    buf[2] = (char)type;
    buf[3] = (char)command;
    memcpy(&buf[4], data, dataLength);
    buf[dataLength + 4] = checkSum(type, command, dataLength, data);
    buf[dataLength + 5] = Rear;
    int frameLength = dataLength + 6;

//    std::string str(buf, frameLength);
//    std::cout << "\n\r SerialModule::packageMessageFrame---" << frameLength << " bytes " << str;
    emit writeSignal(buf, frameLength);
}

char DvrSerial::checkSum(quint8 type, quint8 command, quint8 dataLength, const char *data)
{
    char check = 0;
    check = type + command + dataLength;

    for (quint8 i = 0; i < dataLength; i++) {
        check += data[i];
    }

    check ^= 0xff;
    return check;
}


void DvrSerial::parseDataTask()
{
    qDebug() << "DvrSerial::parseDataTask()--- "  <<  " thread id: " << QThread::currentThreadId();
    int state = 0;
    std::lock_guard<std::mutex> lock(mtx_);
    while (!readBufQueue_.isEmpty()) {

        switch (state) {
        case 0: {
            char head;
            readBufQueue_.read(&head, 1);
            qDebug() << "DvrSerial::parseMessage--- head: " << head;
            if (StartByte == head) {
                state = 1;
            } else {
                return;
            }
            break;
        }
        case 1: {
            char frameSize;
            readBufQueue_.read(&frameSize, 1);
            if (frameSize < 6) {
                return;
            }
            char type;
            readBufQueue_.read(&type, 1);
            char command;
            readBufQueue_.read(&command, 1);
            char data[200];
            memset(data, 0, sizeof(data));
            char dataSize = frameSize - 6;

            readBufQueue_.read(data, (qint64)dataSize);
            char check;
            readBufQueue_.read(&check, 1);
            char endByte;
            readBufQueue_.read(&endByte, 1);

            parseMessage(type, command, data, frameSize, check);
            state = 0;
            break;
        }

        default:
            break;
        }
    }
}

void DvrSerial::parseMessage(const int &type, const int &command, const char *data, const int &frameLength, const int &check)
{
    qDebug() << "DvrSerial::parseMessage--- type: " << type << " command: " << command << " frameLength: " << frameLength << " check: " << check;
    switch (type) {
    case 1:
        qDebug() << "DvrSerial::parseMessage--- data content: " << data;
        break;
    case 0x0c:
        for (int i = 0; i < frameLength - 6; ++i) {
            printf("%x", data[i]);
        }
        break;
    default:
        break;
    }
}

void DvrSerial::testWriteData()
{
    qDebug() << " DvrSerial::testWriteData---";
    packageGpsInfo(19, 8, 30, 16, 30, 0, 1, 1, true, true, 1, 1);
}

const char DvrSerial::StartByte = 0x0A;
const char DvrSerial::EndByte = 0xBB;

