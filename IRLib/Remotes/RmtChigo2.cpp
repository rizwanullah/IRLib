#include "RmtChigo2.h"
#include "../Common.h"

// Remote Model: ZH/ZH-03

int32_t RmtChigo2::remoteId = 1003;

RmtChigo2::RmtChigo2() {
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

std::string RmtChigo2::encodeRemote(REMOTE_COMMON *remoteCommon) {

    std::string encodedIrData;

    std::string oneMarkStr = Common::convertShortToHexString(oneMark);
    std::string oneSpaceStr = Common::convertShortToHexString(oneSpace);
    std::string zeroMarkStr = Common::convertShortToHexString(zeroMark);
    std::string zeroSpaceStr = Common::convertShortToHexString(zeroSpace);

    uint8_t fieldsBuffer[PACKET_LEN_CHIGO2];
    FIELDS_CHIGO2_STRUCT *fieldsChigo2Struct = (FIELDS_CHIGO2_STRUCT *) &fieldsBuffer[0];

    fieldsChigo2Struct->byte0AcOnTime = 0;
    fieldsChigo2Struct->byte0Reserved = 0;
    fieldsChigo2Struct->byte1Reserved1 = 0;
    fieldsChigo2Struct->byte1Reserved2 = 0;
    fieldsChigo2Struct->byte1IonizerOnOff = 0;
    fieldsChigo2Struct->byte1Reserved3 = 0;
    fieldsChigo2Struct->byte1Reserved4 = 0;
    fieldsChigo2Struct->byte1TimerChangeOnOff = 0;
    fieldsChigo2Struct->byte1OnTimerOnOff = 0;
    fieldsChigo2Struct->byte1SwitchOnOff = 0;
    fieldsChigo2Struct->byte2Reserved = 0;
    fieldsChigo2Struct->byte3DisplayOnOff = remoteCommon->displayOnOff;
    fieldsChigo2Struct->byte3HealthOnOff = 0;
    fieldsChigo2Struct->byte3LockOnOff = 0;
    fieldsChigo2Struct->byte3TurboOnOff = remoteCommon->turboOnOff;
    fieldsChigo2Struct->byte3EcoOnOff = 0;
    fieldsChigo2Struct->byte3ElectHeatingOnOff = 0;
    fieldsChigo2Struct->byte3HumanSenseOnOff = 0;
    fieldsChigo2Struct->byte3AutoCleanOnOff = remoteCommon->autoEvapOnOff; // TODO: verify this
    fieldsChigo2Struct->byte4AcOffTime = 0;
    fieldsChigo2Struct->byte4Reserved = 0;
    fieldsChigo2Struct->byte5KeyCode = 0;
    fieldsChigo2Struct->byte5ResvButtonCode = 0;
    fieldsChigo2Struct->byte5ResvFloorStanding = 0;
    fieldsChigo2Struct->byte5VentilationOnOff = 0;
    fieldsChigo2Struct->byte6Reserved = 0;
    fieldsChigo2Struct->byte7SleepOnOff = 0;
    fieldsChigo2Struct->byte7PowerOnOff = remoteCommon->powerOnOff;
    fieldsChigo2Struct->byte7VerticalSwing = getVerSwingValueForFields(remoteCommon->swingVertical);
    fieldsChigo2Struct->byte7HorizontalSwing = getHorSwingValueForFields(remoteCommon->swingHorizontal);
    fieldsChigo2Struct->byte7FanSpeed = getFanSpeedValueForFields(remoteCommon->fanSpeed);
    fieldsChigo2Struct->byte7TemperatureUnit = 0; // 0 = C, 1 = F
    fieldsChigo2Struct->byte8BodySenseTemperature = 0;
    fieldsChigo2Struct->byte8BodySenseOnOff = 0;
    fieldsChigo2Struct->byte8ElectHeatingOnOff = 0;
    fieldsChigo2Struct->byte9SetTemperature = (uint8_t) (remoteCommon->setTemperature - 16);
    fieldsChigo2Struct->byte9OperationMode = getModeValueForFields(remoteCommon->operationMode);
    fieldsChigo2Struct->byte10Reserved = 0;
    fieldsChigo2Struct->byte11Chksum8Mod256 = Common::calcChksum8Mod256(fieldsBuffer, PACKET_LEN_CHIGO2 - 1);

    uint8_t remoteBuffer[PACKET_LEN_CHIGO2];
    for (uint16_t i = 0; i < PACKET_LEN_CHIGO2; i++) {
        //Common::logIrLib("enc byte %02d: %02X", i, fieldsBuffer[i]);
        remoteBuffer[i] = Common::reverseByte(fieldsBuffer[i]);
    }

    encodedIrData.append(Common::convertShortToHexString(headerMark));
    encodedIrData.append(Common::convertShortToHexString(headerSpace));

    for (uint16_t i = 0; i < PACKET_LEN_CHIGO2; i++) {
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

int32_t RmtChigo2::decodeRemote(std::string encodedIrData, REMOTE_COMMON *remoteCommon) {
    if (encodedIrData.length() != IR_DATA_LEN_CHIGO2) {
        Common::logIrLib("IR Data length is not correct! size is %d", encodedIrData.length());
        return DE_CODE_IR_DATA_LEN_ERROR;
    }

    int16_t value = 0;
    uint8_t bitIndex = 0;
    uint8_t byteIndex = 0;
    uint8_t remoteBuffer[PACKET_LEN_CHIGO2];

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

    for (uint8_t i = 0; i < PACKET_LEN_CHIGO2; i++) {
        remoteBuffer[i] = Common::reverseByte(remoteBuffer[i]);
        //Common::logIrLib("dec byte %02d: %02X", i, remoteBuffer[i]);
    }

    uint8_t chksum8Mod256 = Common::calcChksum8Mod256(remoteBuffer, PACKET_LEN_CHIGO2 - 1);
    if (chksum8Mod256 != remoteBuffer[PACKET_LEN_CHIGO2 - 1])
        return DE_CODE_IR_DATA_NOT_VALID;

    FIELDS_CHIGO2_STRUCT *fieldsChigo2Struct = (FIELDS_CHIGO2_STRUCT *) &remoteBuffer[0];

    remoteCommon->displayOnOff = fieldsChigo2Struct->byte3DisplayOnOff;
    remoteCommon->turboOnOff = fieldsChigo2Struct->byte3TurboOnOff;
    remoteCommon->powerOnOff = fieldsChigo2Struct->byte7PowerOnOff;
    remoteCommon->autoEvapOnOff = fieldsChigo2Struct->byte3AutoCleanOnOff;
    remoteCommon->swingVertical = getVerSwingValueFromFields(fieldsChigo2Struct->byte7VerticalSwing);
    remoteCommon->swingHorizontal = getHorSwingValueFromFields(fieldsChigo2Struct->byte7HorizontalSwing);
    remoteCommon->fanSpeed = getFanSpeedValueFromFields(fieldsChigo2Struct->byte7FanSpeed);
    remoteCommon->setTemperature = (uint8_t) (fieldsChigo2Struct->byte9SetTemperature + 16);
    remoteCommon->operationMode = getModeValueFromFields(fieldsChigo2Struct->byte9OperationMode);

    // Not supported by this remote
    remoteCommon->muteOnOff = 0;

    remoteCommon->remoteModel = "ZH/ZH-03";
    remoteCommon->remoteBrand = "Chigo";

    remoteCommon->decodedPacket = Common::convertCharArrayToHexString(remoteBuffer, sizeof(remoteBuffer));

    Common::printBuffer(remoteBuffer, sizeof(remoteBuffer), true);

    return DE_CODE_SUCCESS;
}

uint8_t RmtChigo2::getModeValueForFields(uint8_t modeVal) {
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

uint8_t RmtChigo2::getModeValueFromFields(uint8_t modeVal) {
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

uint8_t RmtChigo2::getFanSpeedValueForFields(uint8_t fanSpeedVal) {
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

uint8_t RmtChigo2::getFanSpeedValueFromFields(uint8_t fanSpeedVal) {
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

uint8_t RmtChigo2::getVerSwingValueForFields(uint8_t verSwingVal) {
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

uint8_t RmtChigo2::getVerSwingValueFromFields(uint8_t verSwingVal) {
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

uint8_t RmtChigo2::getHorSwingValueForFields(uint8_t horSwingVal) {
    switch (horSwingVal) {
        case SWING_HOR_MANUAL:
            return 1;
        case SWING_HOR_AUTO:
            return 0;
        default:
            return 0;
    }
}

uint8_t RmtChigo2::getHorSwingValueFromFields(uint8_t horSwingVal) {
    switch (horSwingVal) {
        case 0:
            return SWING_HOR_AUTO;
        case 1:
            return SWING_HOR_MANUAL;
        default:
            return SWING_HOR_AUTO;
    }
}

bool RmtChigo2::isAcModelSupported(std::string acModelStr) {
    for (uint32_t i = 0; i < supportedAcModels.size(); i++) {
        std::string supportedAcModel = supportedAcModels.at(i);
        if (supportedAcModel.compare(acModelStr) == 0)
            return true;
    }
    return false;
}
