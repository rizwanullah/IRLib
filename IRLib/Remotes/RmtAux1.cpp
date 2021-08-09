#include "RmtAux1.h"
#include "../Common.h"

// Remote Model: YKR-L/102E

int32_t RmtAux1::remoteId = 1001;

RmtAux1::RmtAux1() {
    headerMark = 9000;
    headerSpace = 4500;
    footerMark = 560;
    zeroMark = 560;
    zeroSpace = 560;
    oneMark = 560;
    oneSpace = 1690;

    // Invert o cool Cool only
    supportedAcModels.push_back("SQXIC");
    supportedAcModels.push_back("SRXIC");
    supportedAcModels.push_back("STXIC");
    // AUX Invert o cool H&C
    supportedAcModels.push_back("SQXICH");
    supportedAcModels.push_back("SRXICH");
    supportedAcModels.push_back("STXICH");
    // AUX Invert o ECO Cool only
    supportedAcModels.push_back("SQXIE");
    supportedAcModels.push_back("SRXIE");
    supportedAcModels.push_back("STXIE");
    // AUX Invert o ECO H&C
    supportedAcModels.push_back("SQXIEH");
    supportedAcModels.push_back("SRXIEH");
    supportedAcModels.push_back("STXIEH");
    // AUX Invert o PRO H&C
    supportedAcModels.push_back("SQXIPH");
    supportedAcModels.push_back("SRXIPH");
    supportedAcModels.push_back("STXIPH");
    // AUX Invert o Life H&C
    supportedAcModels.push_back("SQXILH");
    supportedAcModels.push_back("SRXILH");
    supportedAcModels.push_back("STXILH");
}

std::string RmtAux1::encodeRemote(REMOTE_COMMON *remoteCommon) {

    std::string encodedIrData;

    std::string oneMarkStr = Common::convertShortToHexString(oneMark);
    std::string oneSpaceStr = Common::convertShortToHexString(oneSpace);
    std::string zeroMarkStr = Common::convertShortToHexString(zeroMark);
    std::string zeroSpaceStr = Common::convertShortToHexString(zeroSpace);

    uint8_t fieldsBuffer[PACKET_LEN_AUX1] = {0};
    FIELDS_AUX1_STRUCT *fieldsAux1Struct = (FIELDS_AUX1_STRUCT *) &fieldsBuffer[0];

    fieldsAux1Struct->byte0Unknown = (uint8_t) 0b11000011;
    fieldsAux1Struct->byte1Temperature = (uint8_t) (remoteCommon->setTemperature - 8);
    fieldsAux1Struct->byte1VerticalSwing = getVerSwingValueForFields(remoteCommon->swingVertical);
    fieldsAux1Struct->byte2HorizontalSwing = getHorSwingValueForFields(remoteCommon->swingHorizontal);
    fieldsAux1Struct->byte2Unknown1 = 0;
    fieldsAux1Struct->byte3TemperatureFraction = 0; // this means that fraction part is never transmitted
    fieldsAux1Struct->byte3Unknown1 = 0;
    fieldsAux1Struct->byte4FanSpeed = getFanSpeedValueForFields(remoteCommon->fanSpeed);
    fieldsAux1Struct->byte4TimerTimeValue = 0;
    fieldsAux1Struct->byte5SilentOnOff = remoteCommon->muteOnOff;
    fieldsAux1Struct->byte5TurboOnOff = remoteCommon->turboOnOff;
    fieldsAux1Struct->byte5Unknown2 = 0;
    fieldsAux1Struct->byte6OperationMode = getModeValueForFields(remoteCommon->operationMode);
    fieldsAux1Struct->byte6HealthOnOff = 0;
    fieldsAux1Struct->byte6IfeelOnOff = 0; //TODO: be added in app?
    fieldsAux1Struct->byte6SleepOnOff = 0; //TODO: be added in app?
    fieldsAux1Struct->byte6Unknown2 = 0;
    fieldsAux1Struct->byte7Unknown = 0;
    fieldsAux1Struct->byte8Unknown = 0;
    fieldsAux1Struct->byte9Unknown1 = 0;
    fieldsAux1Struct->byte9TimerOnOff = 0;
    fieldsAux1Struct->byte9PowerOnOff = remoteCommon->powerOnOff;
    fieldsAux1Struct->byte9ElectHeatingOnOff = 0;
    fieldsAux1Struct->byte9EcoOnOff = 0;
    fieldsAux1Struct->byte9AutoEvapOnOff = remoteCommon->autoEvapOnOff;
    fieldsAux1Struct->byte10Unknown = 0;
    fieldsAux1Struct->byte11KeyCode = (uint8_t) (remoteCommon->displayOnOff ? 0b10101 : 0);
    fieldsAux1Struct->byte11Unknown1 = 0;
    fieldsAux1Struct->byte12Chksum8Mod256 = Common::calcChksum8Mod256(fieldsBuffer, PACKET_LEN_AUX1 - 1);

    uint8_t remoteBuffer[PACKET_LEN_AUX1];
    for (uint16_t i = 0; i < PACKET_LEN_AUX1; i++) {
        //Common::logIrLib("enc byte %02d: %02X", i, fieldsBuffer[i]);
        remoteBuffer[i] = Common::reverseByte(fieldsBuffer[i]);
    }

    encodedIrData.append(Common::convertShortToHexString(headerMark));
    encodedIrData.append(Common::convertShortToHexString(headerSpace));

    for (uint16_t i = 0; i < PACKET_LEN_AUX1; i++) {
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

    encodedIrData.append(Common::convertShortToHexString(footerMark));

    return encodedIrData;
}

int32_t RmtAux1::decodeRemote(std::string encodedIrData, REMOTE_COMMON *remoteCommon) {
    if (encodedIrData.length() != IR_DATA_LEN_AUX1) {
        Common::logIrLib("IR Data length is not correct! size is %d", encodedIrData.length());
        return DE_CODE_IR_DATA_LEN_ERROR;
    }

    int16_t value = 0;
    uint8_t bitIndex = 0;
    uint8_t byteIndex = 0;
    uint8_t remoteBuffer[PACKET_LEN_AUX1];

    value = Common::convertHexStringToShort(encodedIrData.substr(0, 4));
    if (!Common::isValueTolerable(value, headerMark, 400)) return DE_CODE_IR_DATA_NOT_VALID;
    value = Common::convertHexStringToShort(encodedIrData.substr(4, 4));
    if (!Common::isValueTolerable(value, headerSpace, 400)) return DE_CODE_IR_DATA_NOT_VALID;

    for (uint32_t i = 8; i < 840; i += 8) {
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

    value = Common::convertHexStringToShort(encodedIrData.substr(840, 4));
    if (!Common::isValueTolerable(value, footerMark, 400)) return DE_CODE_IR_DATA_NOT_VALID;

    for (uint8_t i = 0; i < PACKET_LEN_AUX1; i++) {
        remoteBuffer[i] = Common::reverseByte(remoteBuffer[i]);
        //Common::logIrLib("dec byte %02d: %02X", i, remoteBuffer[i]);
    }

    uint8_t chksum8Mod256 = Common::calcChksum8Mod256(remoteBuffer, PACKET_LEN_AUX1 - 1);
    if (chksum8Mod256 != remoteBuffer[PACKET_LEN_AUX1 - 1])
        return DE_CODE_IR_DATA_NOT_VALID;

    FIELDS_AUX1_STRUCT *fieldsAux1Struct = (FIELDS_AUX1_STRUCT *) &remoteBuffer[0];

    remoteCommon->muteOnOff = fieldsAux1Struct->byte5SilentOnOff;
    remoteCommon->turboOnOff = fieldsAux1Struct->byte5TurboOnOff;
    remoteCommon->powerOnOff = fieldsAux1Struct->byte9PowerOnOff;
    remoteCommon->autoEvapOnOff = fieldsAux1Struct->byte9AutoEvapOnOff;
    remoteCommon->swingVertical = getVerSwingValueFromFields(fieldsAux1Struct->byte1VerticalSwing);
    remoteCommon->swingHorizontal = getHorSwingValueFromFields(fieldsAux1Struct->byte2HorizontalSwing);
    remoteCommon->fanSpeed = getFanSpeedValueFromFields(fieldsAux1Struct->byte4FanSpeed);
    remoteCommon->setTemperature = (uint8_t) (fieldsAux1Struct->byte1Temperature + 8);
    remoteCommon->operationMode = getModeValueFromFields(fieldsAux1Struct->byte6OperationMode);

    // Not supported by this remote
    remoteCommon->displayOnOff = 1;

    remoteCommon->remoteModel = "YKR-L/102E";
    remoteCommon->remoteBrand = "AUX";

    remoteCommon->decodedPacket = Common::convertCharArrayToHexString(remoteBuffer, sizeof(remoteBuffer));

    Common::printBuffer(remoteBuffer, sizeof(remoteBuffer), true);

    return DE_CODE_SUCCESS;
}

uint8_t RmtAux1::getModeValueForFields(uint8_t modeVal) {
    switch (modeVal) {
        case MODE_AUTO:
            return 0;
        case MODE_COOL:
            return 1;
        case MODE_DRY:
            return 2;
        case MODE_FAN:
            return 6;
        case MODE_HEAT:
            return 4;
        default:
            return 0;
    }
}

uint8_t RmtAux1::getModeValueFromFields(uint8_t modeVal) {
    switch (modeVal) {
        case 0:
            return MODE_AUTO;
        case 1:
            return MODE_COOL;
        case 2:
            return MODE_DRY;
        case 6:
            return MODE_FAN;
        case 4:
            return MODE_HEAT;
        default:
            return MODE_AUTO;
    }
}

uint8_t RmtAux1::getFanSpeedValueForFields(uint8_t fanSpeedVal) {
    switch (fanSpeedVal) {
        case FAN_AUTO:
            return 5;
        case FAN_LOW:
            return 3;
        case FAN_MID:
            return 2;
        case FAN_HIGH:
            return 1;
        default:
            return 5;
    }
}

uint8_t RmtAux1::getFanSpeedValueFromFields(uint8_t fanSpeedVal) {
    switch (fanSpeedVal) {
        case 5:
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

uint8_t RmtAux1::getVerSwingValueForFields(uint8_t verSwingVal) {
    switch (verSwingVal) {
        case SWING_VER_AUTO:
            return 0;
        case SWING_VER_MANUAL:
            return 7;
        case SWING_VER_POS_1:
            return 1;
        case SWING_VER_POS_2:
            return 2;
        case SWING_VER_POS_3:
            return 3;
        case SWING_VER_POS_4:
            return 4;
        case SWING_VER_POS_5:
            return 5;
        default:
            return 0;
    }
}

uint8_t RmtAux1::getVerSwingValueFromFields(uint8_t verSwingVal) {
    switch (verSwingVal) {
        case 0:
            return SWING_VER_AUTO;
        case 7:
            return SWING_VER_MANUAL;
        case 1:
            return SWING_VER_POS_1;
        case 2:
            return SWING_VER_POS_2;
        case 3:
            return SWING_VER_POS_3;
        case 4:
            return SWING_VER_POS_4;
        case 5:
            return SWING_VER_POS_5;
        default:
            return SWING_VER_AUTO;
    }
}

uint8_t RmtAux1::getHorSwingValueForFields(uint8_t horSwingVal) {
    switch (horSwingVal) {
        case SWING_HOR_AUTO:
            return 0;
        case SWING_HOR_MANUAL:
            return 7;
        default:
            return 0;
    }
}

uint8_t RmtAux1::getHorSwingValueFromFields(uint8_t horSwingVal) {
    switch (horSwingVal) {
        case 0:
            return SWING_HOR_AUTO;
        case 7:
            return SWING_HOR_MANUAL;
        default:
            return SWING_HOR_AUTO;
    }
}

bool RmtAux1::isAcModelSupported(std::string acModelStr) {
    for (uint32_t i = 0; i < supportedAcModels.size(); i++) {
        std::string supportedAcModel = supportedAcModels.at(i);
        if (supportedAcModel.compare(acModelStr) == 0)
            return true;
    }
    return false;
}