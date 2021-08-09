#include "RmtChigo1.h"
#include "../Common.h"

// Remote Model: ZH/JT-03

int32_t RmtChigo1::remoteId = 1000;

RmtChigo1::RmtChigo1() {
    headerMark = 6100;
    headerSpace = 7400;
    footerMark1 = 540;
    footerSpace = 7400;
    footerMark2 = 540;
    zeroMark = 540;
    zeroSpace = 540;
    oneMark = 540;
    oneSpace = 1620;

    // Chigo Invert o Sense H&C
    supportedAcModels.push_back("SQCISH");
    supportedAcModels.push_back("SRCISH");
    supportedAcModels.push_back("STCISH");
    // Chigo Desire Infinite Invertor H&C
    supportedAcModels.push_back("SQCIIH");
    supportedAcModels.push_back("SRCIIH");
    supportedAcModels.push_back("STCIIH");
    // Chigo Everest Cool only
    supportedAcModels.push_back("SQCES");
    supportedAcModels.push_back("SRCES");
    supportedAcModels.push_back("STCES");
    // Chigo Desire Infinite Cool only
    supportedAcModels.push_back("SQCDI");
    supportedAcModels.push_back("SRCDI");
    supportedAcModels.push_back("STCDI");
}

std::string RmtChigo1::encodeRemote(REMOTE_COMMON *remoteCommon) {

    std::string encodedIrData;

    std::string oneMarkStr = Common::convertShortToHexString(oneMark);
    std::string oneSpaceStr = Common::convertShortToHexString(oneSpace);
    std::string zeroMarkStr = Common::convertShortToHexString(zeroMark);
    std::string zeroSpaceStr = Common::convertShortToHexString(zeroSpace);

    uint8_t fieldsBuffer[6];
    FIELDS_CHIGO1_STRUCT *fieldsChigo1Struct = (FIELDS_CHIGO1_STRUCT *) &fieldsBuffer[0];

    fieldsChigo1Struct->byte0TimerTime = 0;
    fieldsChigo1Struct->byte0TimerEnable = 0;
    fieldsChigo1Struct->byte0TimerFixedOnOff = 0;
    fieldsChigo1Struct->byte0TimerLimitOnOff = 0;
    fieldsChigo1Struct->byte1DisplayOnOff = (uint8_t) (remoteCommon->displayOnOff ? 0 : 1);
    fieldsChigo1Struct->byte1HealthOnOff = 0;
    fieldsChigo1Struct->byte1LockKeyOnOff = 0;
    fieldsChigo1Struct->byte1TurboOnOff = remoteCommon->turboOnOff;
    fieldsChigo1Struct->byte1EconomyOnOff = 0;
    fieldsChigo1Struct->byte1ElectHeatingOnOff = 0;
    fieldsChigo1Struct->byte1HumanDetectOnOff = 0;
    fieldsChigo1Struct->byte1AutoCleanOnOff = remoteCommon->autoEvapOnOff;
    fieldsChigo1Struct->byte2PressedKeyCode = (uint8_t) (remoteCommon->displayOnOff ? 0 : 0b01011);
    fieldsChigo1Struct->byte2IonFunctionOnOff = 0;
    fieldsChigo1Struct->byte2LeavingHomeOnOff = 0;
    fieldsChigo1Struct->byte2VentilationOnOff = 0;
    fieldsChigo1Struct->byte3SleepOnOff = 0;
    fieldsChigo1Struct->byte3PowerOnOff = remoteCommon->powerOnOff;
    fieldsChigo1Struct->byte3OuterFlapControl = getVerSwingValueForFields(remoteCommon->swingVertical);
    fieldsChigo1Struct->byte3InnerFlapControl = getHorSwingValueForFields(remoteCommon->swingHorizontal);
    fieldsChigo1Struct->byte3WindControl = getFanSpeedValueForFields(remoteCommon->fanSpeed);
    fieldsChigo1Struct->byte3TemperatureUnit = 0;
    fieldsChigo1Struct->byte4SetTemperature = (uint8_t) (remoteCommon->setTemperature - 16);
    fieldsChigo1Struct->byte4OperationMode = getModeValueForFields(remoteCommon->operationMode);
    fieldsChigo1Struct->byte5Reserved = 0xD5;

    uint8_t remoteBuffer[PACKET_LEN_CHIGO1];
    remoteBuffer[0] = Common::reverseByte(~fieldsBuffer[0]);
    remoteBuffer[1] = Common::reverseByte(fieldsBuffer[0]);
    remoteBuffer[2] = Common::reverseByte(~fieldsBuffer[1]);
    remoteBuffer[3] = Common::reverseByte(fieldsBuffer[1]);
    remoteBuffer[4] = Common::reverseByte(~fieldsBuffer[2]);
    remoteBuffer[5] = Common::reverseByte(fieldsBuffer[2]);
    remoteBuffer[6] = Common::reverseByte(~fieldsBuffer[3]);
    remoteBuffer[7] = Common::reverseByte(fieldsBuffer[3]);
    remoteBuffer[8] = Common::reverseByte(~fieldsBuffer[4]);
    remoteBuffer[9] = Common::reverseByte(fieldsBuffer[4]);
    remoteBuffer[10] = Common::reverseByte(~fieldsBuffer[5]);
    remoteBuffer[11] = Common::reverseByte(fieldsBuffer[5]);

    encodedIrData.append(Common::convertShortToHexString(headerMark));
    encodedIrData.append(Common::convertShortToHexString(headerSpace));

    for (uint16_t i = 0; i < PACKET_LEN_CHIGO1; i++) {
        uint8_t byte = remoteBuffer[i];
        for (uint8_t j = 0; j < 8; j++) {
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

    encodedIrData.append(Common::convertShortToHexString(footerMark1));
    encodedIrData.append(Common::convertShortToHexString(footerSpace));
    encodedIrData.append(Common::convertShortToHexString(footerMark2));

    return encodedIrData;
}

int32_t RmtChigo1::decodeRemote(std::string encodedIrData, REMOTE_COMMON *remoteCommon) {
    if (encodedIrData.length() != IR_DATA_LEN_CHIGO1) {
        Common::logIrLib("IR Data length is not correct! size is %d", encodedIrData.length());
        return DE_CODE_IR_DATA_LEN_ERROR;
    }

    int16_t value = 0;
    uint8_t bitIndex = 0;
    uint8_t byteIndex = 0;
    uint8_t remoteBuffer[PACKET_LEN_CHIGO1];

    value = Common::convertHexStringToShort(encodedIrData.substr(0, 4));
    if (!Common::isValueTolerable(value, headerMark, 400)) return DE_CODE_IR_DATA_NOT_VALID;
    value = Common::convertHexStringToShort(encodedIrData.substr(4, 4));
    if (!Common::isValueTolerable(value, headerSpace, 400)) return DE_CODE_IR_DATA_NOT_VALID;

    for (uint32_t i = 8; i < 776; i += 8) {
        if (bitIndex >= 8) {
            bitIndex = 0;
            byteIndex++;
        }
        remoteBuffer[byteIndex] <<= 1;

        int16_t mark = Common::convertHexStringToShort(encodedIrData.substr(i, 4));
        int16_t space = Common::convertHexStringToShort(encodedIrData.substr(i + 4, 4));
        if (Common::isValueTolerable(mark, zeroMark, 200) && Common::isValueTolerable(space, zeroSpace, 200)) {
            remoteBuffer[byteIndex] &= 0xFE;
        } else if (Common::isValueTolerable(mark, oneMark, 200) && Common::isValueTolerable(space, oneSpace, 400)) {
            remoteBuffer[byteIndex] |= 0x01;
        } else {
            return DE_CODE_IR_DATA_NOT_VALID;
        }

        bitIndex++;
    }

    value = Common::convertHexStringToShort(encodedIrData.substr(776, 4));
    if (!Common::isValueTolerable(value, footerMark1, 200)) return DE_CODE_IR_DATA_NOT_VALID;
    value = Common::convertHexStringToShort(encodedIrData.substr(780, 4));
    if (!Common::isValueTolerable(value, footerSpace, 400)) return DE_CODE_IR_DATA_NOT_VALID;
    value = Common::convertHexStringToShort(encodedIrData.substr(784, 4));
    if (!Common::isValueTolerable(value, footerMark2, 200)) return DE_CODE_IR_DATA_NOT_VALID;

    remoteBuffer[0] = Common::reverseByte(remoteBuffer[0]);
    remoteBuffer[1] = Common::reverseByte(remoteBuffer[1]);
    remoteBuffer[2] = Common::reverseByte(remoteBuffer[2]);
    remoteBuffer[3] = Common::reverseByte(remoteBuffer[3]);
    remoteBuffer[4] = Common::reverseByte(remoteBuffer[4]);
    remoteBuffer[5] = Common::reverseByte(remoteBuffer[5]);
    remoteBuffer[6] = Common::reverseByte(remoteBuffer[6]);
    remoteBuffer[7] = Common::reverseByte(remoteBuffer[7]);
    remoteBuffer[8] = Common::reverseByte(remoteBuffer[8]);
    remoteBuffer[9] = Common::reverseByte(remoteBuffer[9]);
    remoteBuffer[10] = Common::reverseByte(remoteBuffer[10]);
    remoteBuffer[11] = Common::reverseByte(remoteBuffer[11]);

    if ((uint8_t)~remoteBuffer[0] != (uint8_t)remoteBuffer[1] ||
            (uint8_t)~remoteBuffer[2] != (uint8_t)remoteBuffer[3] ||
            (uint8_t)~remoteBuffer[4] != (uint8_t)remoteBuffer[5] ||
            (uint8_t)~remoteBuffer[6] != (uint8_t)remoteBuffer[7] ||
            (uint8_t)~remoteBuffer[8] != (uint8_t)remoteBuffer[9] ||
            (uint8_t)~remoteBuffer[10] != (uint8_t)remoteBuffer[11])
        return DE_CODE_IR_DATA_NOT_VALID;

    uint8_t remoteFields[6];
    remoteFields[0] = remoteBuffer[1];
    remoteFields[1] = remoteBuffer[3];
    remoteFields[2] = remoteBuffer[5];
    remoteFields[3] = remoteBuffer[7];
    remoteFields[4] = remoteBuffer[9];
    remoteFields[5] = remoteBuffer[11];

    FIELDS_CHIGO1_STRUCT *fieldsChigo1Struct = (FIELDS_CHIGO1_STRUCT *) &remoteFields[0];

    remoteCommon->displayOnOff = (uint8_t) (fieldsChigo1Struct->byte1DisplayOnOff ? 0 : 1);
    remoteCommon->autoEvapOnOff = fieldsChigo1Struct->byte1AutoCleanOnOff;
    remoteCommon->turboOnOff = fieldsChigo1Struct->byte1TurboOnOff;
    remoteCommon->powerOnOff = fieldsChigo1Struct->byte3PowerOnOff;
    remoteCommon->swingVertical = getVerSwingValueFromFields(fieldsChigo1Struct->byte3OuterFlapControl);
    remoteCommon->swingHorizontal = getHorSwingValueFromFields(fieldsChigo1Struct->byte3InnerFlapControl);
    remoteCommon->fanSpeed = getFanSpeedValueFromFields(fieldsChigo1Struct->byte3WindControl);
    remoteCommon->setTemperature = (uint8_t) (fieldsChigo1Struct->byte4SetTemperature + 16);
    remoteCommon->operationMode = getModeValueFromFields(fieldsChigo1Struct->byte4OperationMode);

    // Not supported by this remote
    remoteCommon->muteOnOff = 0;

    remoteCommon->remoteModel = "ZH/JT-03";
    remoteCommon->remoteBrand = "Chigo";

    remoteCommon->decodedPacket = Common::convertCharArrayToHexString(remoteBuffer, sizeof(remoteBuffer));

    Common::printBuffer(remoteBuffer, sizeof(remoteBuffer), true);

    return DE_CODE_SUCCESS;
}

uint8_t RmtChigo1::getModeValueForFields(uint8_t modeVal) {
    switch (modeVal) {
        case MODE_AUTO:
            return 0;
        case MODE_COOL:
            return 1;
        case MODE_DRY:
            return 2;
        case MODE_FAN:
            return 3;
        case MODE_HEAT:
            return 4;
        default:
            return 0;
    }
}

uint8_t RmtChigo1::getModeValueFromFields(uint8_t modeVal) {
    switch (modeVal) {
        case 0:
            return MODE_AUTO;
        case 1:
            return MODE_COOL;
        case 2:
            return MODE_DRY;
        case 3:
            return MODE_FAN;
        case 4:
            return MODE_HEAT;
        default:
            return MODE_AUTO;
    }
}

uint8_t RmtChigo1::getFanSpeedValueForFields(uint8_t fanSpeedVal) {
    switch (fanSpeedVal) {
        case FAN_AUTO:
            return 0;
        case FAN_LOW:
            return 3;
        case FAN_MID:
            return 2;
        case FAN_HIGH:
            return 1;
        default:
            return 0;
    }
}

uint8_t RmtChigo1::getFanSpeedValueFromFields(uint8_t fanSpeedVal) {
    switch (fanSpeedVal) {
        case 0:
            return FAN_AUTO;
        case 3:
            return FAN_LOW;
        case 2:
            return FAN_MID;
        case 1:
            return FAN_HIGH;
        default:
            return MODE_AUTO;
    }
}

uint8_t RmtChigo1::getVerSwingValueForFields(uint8_t verSwingVal) {
    switch (verSwingVal) {
        case SWING_VER_AUTO:
            return 1;
        case SWING_VER_MANUAL:
            return 2;
        case SWING_VER_NATURAL:
            return 0;
        default:
            return 1;
    }
}

uint8_t RmtChigo1::getVerSwingValueFromFields(uint8_t verSwingVal) {
    switch (verSwingVal) {
        case 0:
            return SWING_VER_NATURAL;
        case 1:
            return SWING_VER_AUTO;
        case 2:
            return SWING_VER_MANUAL;
        default:
            return SWING_VER_AUTO;
    }
}

uint8_t RmtChigo1::getHorSwingValueForFields(uint8_t horSwingVal) {
    switch (horSwingVal) {
        case SWING_HOR_MANUAL:
            return 1;
        case SWING_HOR_AUTO:
            return 0;
        default:
            return 0;
    }
}

uint8_t RmtChigo1::getHorSwingValueFromFields(uint8_t horSwingVal) {
    switch (horSwingVal) {
        case 0:
            return SWING_HOR_AUTO;
        case 1:
            return SWING_HOR_MANUAL;
        default:
            return SWING_HOR_AUTO;
    }
}

bool RmtChigo1::isAcModelSupported(std::string acModelStr) {
    for (uint32_t i = 0; i < supportedAcModels.size(); i++) {
        std::string supportedAcModel = supportedAcModels.at(i);
        if (supportedAcModel.compare(acModelStr) == 0)
            return true;
    }
    return false;
}
