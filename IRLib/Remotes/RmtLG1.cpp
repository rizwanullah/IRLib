#include "../Common.h"
#include "RmtLG1.h"

// Remote Model: LG JetCool

int32_t RmtLG1::remoteId = 1011;

RmtLG1::RmtLG1() {
    headerMark = 8000;
    headerSpace = 4000;
    footerMark = 600;
    zeroMark = 600;
    zeroSpace = 550;
    oneMark = 600;
    oneSpace = 1600;

    // AC Models
    supportedAcModels.push_back("SQLJCI");
    supportedAcModels.push_back("SRLJCI");
    supportedAcModels.push_back("STLJCI");
}

std::string RmtLG1::encodeRemote(REMOTE_COMMON *remoteCommon) {
    int8_t remoteBuffer[PACKET_LEN_LG1] = {0};
    FIELDS_LG1_STRUCT *fieldsLG1Struct = (FIELDS_LG1_STRUCT *) &remoteBuffer[0];

    populateFields(remoteBuffer, remoteCommon);
    fieldsLG1Struct->byte3Crc = calcCRC(remoteBuffer, 3);

    std::string encodedIrData;

    encodedIrData.append(Common::convertShortToHexString(headerMark));
    encodedIrData.append(Common::convertShortToHexString(headerSpace));

    std::string oneMarkStr = Common::convertShortToHexString(oneMark);
    std::string oneSpaceStr = Common::convertShortToHexString(oneSpace);
    std::string zeroMarkStr = Common::convertShortToHexString(zeroMark);
    std::string zeroSpaceStr = Common::convertShortToHexString(zeroSpace);

    int8_t bitsLen = 8;
    for (uint16_t i = 0; i < PACKET_LEN_LG1; i++) {
        int8_t byte = remoteBuffer[i];
        if (i == (PACKET_LEN_LG1 - 1)) bitsLen = 4;
        for (uint8_t j = 0; j < bitsLen; j++) {
            if ((byte & 0x80) == 0x80) {
                encodedIrData.append(oneMarkStr);
                encodedIrData.append(oneSpaceStr);
            } else {
                encodedIrData.append(zeroMarkStr);
                encodedIrData.append(zeroSpaceStr);
            }
            byte <<= 1;
        }
    }

    encodedIrData.append(Common::convertShortToHexString(footerMark));

    return encodedIrData;
}

int32_t RmtLG1::decodeRemote(std::string encodedIrData, REMOTE_COMMON *remoteCommon) {

    return DE_CODE_REMOTE_NOT_FOUND; //this remote decoding is not supported yet.
}

void RmtLG1::populateFields(int8_t *remoteBuffer, REMOTE_COMMON *remoteCommon) {
    FIELDS_LG1_STRUCT *fieldsLG1Struct = (FIELDS_LG1_STRUCT *) &remoteBuffer[0];

    if (remoteCommon->powerOnOff == 0) {
        remoteBuffer[0] = 0x88;
        remoteBuffer[1] = 0xC0;
        remoteBuffer[2] = 0x05;
        return;
    } else {
        fieldsLG1Struct->byte0Header = (int8_t) 0x88;

        fieldsLG1Struct->byte1Mode = 0; // 0 == cool

        fieldsLG1Struct->byte1OnOffState = 0; // 0 == ON

        fieldsLG1Struct->byte2FanSpeed = getFanSpeedValueForFields(remoteCommon->fanSpeed);

        fieldsLG1Struct->byte2TurboOnOff = remoteCommon->turboOnOff;

        fieldsLG1Struct->byte2Temperature = remoteCommon->setTemperature - 15;

        fieldsLG1Struct->byte3Unused = 0;
    }
}

int8_t RmtLG1::getFanSpeedValueForFields(int8_t fanSpeedVal) {
    switch (fanSpeedVal) {
        case FAN_AUTO:
            return 5;
        case FAN_LOW:
            return 0;
        case FAN_MID:
            return 2;
        case FAN_HIGH:
            return 4;
        default:
            return 5;
    }
}

int8_t RmtLG1::calcCRC(int8_t *buff, uint8_t buffLen) {
    int16_t sumVal = 0;
    for (uint16_t i = 0; i < buffLen; i++)
        sumVal += ((buff[i] & 0x0F) + ((buff[i] & 0xF0) >> 4));
    return (int8_t)(sumVal - 32);
}

bool RmtLG1::isAcModelSupported(std::string acModelStr) {
    for (uint32_t i = 0; i < supportedAcModels.size(); i++) {
        std::string supportedAcModel = supportedAcModels.at(i);
        if (supportedAcModel.compare(acModelStr) == 0)
            return true;
    }
    return false;
}
