#ifndef IRLIB_RMTLG1_H
#define IRLIB_RMTLG1_H

#include "../Common.h"

// Remote Model: LG JetCool

#define PACKET_LEN_LG1           4
//#define IR_DATA_LEN_LG1          0

typedef struct {
    int8_t byte0Header:8;
    int8_t byte1Mode:3;
    int8_t byte1OnOffState:5;
    int8_t byte2FanSpeed:3;
    int8_t byte2TurboOnOff:1;
    int8_t byte2Temperature:4;
    int8_t byte3Unused:4;
    int8_t byte3Crc:4;
} FIELDS_LG1_STRUCT;

class RmtLG1 {

private:
    uint16_t headerMark;
    uint16_t headerSpace;
    uint16_t footerMark;
    uint16_t zeroMark;
    uint16_t zeroSpace;
    uint16_t oneMark;
    uint16_t oneSpace;

    std::vector<std::string> supportedAcModels;

public:
    static int32_t remoteId;

    RmtLG1();
    std::string encodeRemote(REMOTE_COMMON *);
    int32_t decodeRemote(std::string, REMOTE_COMMON *);
    void populateFields(int8_t *remoteBuffer, REMOTE_COMMON *remoteCommon);
    int8_t getFanSpeedValueForFields(int8_t fanSpeedVal);
    int8_t calcCRC(int8_t *buff, uint8_t buffLen);
    bool isAcModelSupported(std::string acModelStr);
};

#endif //IRLIB_RMTLG1_H
