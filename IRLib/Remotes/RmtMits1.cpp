#include "RmtMits1.h"
#include "../Common.h"

// Remote Model: TODO

int32_t RmtMits1::remoteId = 1012;

RmtMits1::RmtMits1() {
    headerMark = 3200;
    headerSpace = 1600;
    footerMark = 400;
    zeroMark = 400;
    zeroSpace = 400;
    oneMark = 400;
    oneSpace = 1200;

    // Mitsubishi
    supportedAcModels.push_back("SQSJCI");
    supportedAcModels.push_back("SRSJCI");
    supportedAcModels.push_back("STSJCI");
}

std::string RmtMits1::encodeRemote(REMOTE_COMMON *remoteCommon) {

    std::string encodedIrData;

    std::string oneMarkStr = Common::convertShortToHexString(oneMark);
    std::string oneSpaceStr = Common::convertShortToHexString(oneSpace);
    std::string zeroMarkStr = Common::convertShortToHexString(zeroMark);
    std::string zeroSpaceStr = Common::convertShortToHexString(zeroSpace);

    uint8_t powerMode = remoteCommon->powerOnOff ? MITSUBISHI_HEAVY_MODE_ON : MITSUBISHI_HEAVY_MODE_OFF;
    uint8_t operatingMode = getModeValueForFields(remoteCommon->operationMode);
    uint8_t fanSpeed = getFanSpeedValueForFields(remoteCommon->fanSpeed);
    uint8_t temperature = (~((remoteCommon->setTemperature - 17) << 4)) & 0xF0;
    uint8_t swingV = getVerSwingValueForFields(remoteCommon->swingVertical);
    uint8_t swingH = getHorSwingValueForFields(remoteCommon->swingHorizontal);
    uint8_t cleanMode = MITSUBISHI_HEAVY_ZJ_CLEAN_OFF;

    uint8_t fieldsBuffer[PACKET_LEN_MITS1] = { 0x52, 0xAE, 0xC3, 0x26, 0xD9, 0x11, 0x00, 0x07, 0x00, 0x00, 0x00 };

    fieldsBuffer[5] |= swingH | (swingV & 0b00000010) | cleanMode;
    fieldsBuffer[7] |= fanSpeed | (swingV & 0b00011000);
    fieldsBuffer[9] |= operatingMode | powerMode | temperature;

    fieldsBuffer[6] = ~fieldsBuffer[5];
    fieldsBuffer[8] = ~fieldsBuffer[7];
    fieldsBuffer[10] = ~fieldsBuffer[9];

    uint8_t remoteBuffer[PACKET_LEN_MITS1];
    for (uint16_t i = 0; i < PACKET_LEN_MITS1; i++) {
        //Common::logIrLib("enc byte %02d: %02X", i, fieldsBuffer[i]);
        remoteBuffer[i] = Common::reverseByte(fieldsBuffer[i]);
    }

    encodedIrData.append(Common::convertShortToHexString(headerMark));
    encodedIrData.append(Common::convertShortToHexString(headerSpace));

    for (uint16_t i = 0; i < PACKET_LEN_MITS1; i++) {
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

int32_t RmtMits1::decodeRemote(std::string encodedIrData, REMOTE_COMMON *remoteCommon) {

    /*
    if (encodedIrData.length() != IR_DATA_LEN_AUX1) {
        Common::logIrLib("IR Data length is not correct! size is %d", encodedIrData.length());
        return DE_CODE_IR_DATA_LEN_ERROR;
    }

    int16_t value = 0;
    uint8_t bitIndex = 0;
    uint8_t byteIndex = 0;
    uint8_t remoteBuffer[PACKET_LEN_MITS1];

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

    for (uint8_t i = 0; i < PACKET_LEN_MITS1; i++) {
        remoteBuffer[i] = Common::reverseByte(remoteBuffer[i]);
        //Common::logIrLib("dec byte %02d: %02X", i, remoteBuffer[i]);
    }

    uint8_t chksum8Mod256 = Common::calcChksum8Mod256(remoteBuffer, PACKET_LEN_MITS1 - 1);
    if (chksum8Mod256 != remoteBuffer[PACKET_LEN_MITS1 - 1])
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
    */

    remoteCommon->muteOnOff = 0;
    remoteCommon->turboOnOff = 0;
    remoteCommon->powerOnOff = 1;
    remoteCommon->autoEvapOnOff = 0;
    remoteCommon->swingVertical = SWING_VER_MANUAL;
    remoteCommon->swingHorizontal = SWING_HOR_MANUAL;
    remoteCommon->fanSpeed = FAN_HIGH;
    remoteCommon->setTemperature = 24;
    remoteCommon->operationMode = MODE_COOL;
    remoteCommon->displayOnOff = 1;

    remoteCommon->remoteModel = "";
    remoteCommon->remoteBrand = "MITSUBISHI";

    remoteCommon->decodedPacket = "";

    return DE_CODE_SUCCESS;
}

uint8_t RmtMits1::getModeValueForFields(uint8_t modeVal) {
    switch (modeVal) {
        case MODE_AUTO:
            return MITSUBISHI_HEAVY_MODE_AUTO;
        case MODE_HEAT:
            return MITSUBISHI_HEAVY_MODE_HEAT;
        case MODE_COOL:
            return MITSUBISHI_HEAVY_MODE_COOL;
        case MODE_DRY:
            return MITSUBISHI_HEAVY_MODE_DRY;
        case MODE_FAN:
            return MITSUBISHI_HEAVY_MODE_FAN;
        default:
            return MITSUBISHI_HEAVY_MODE_AUTO;
    }
}

uint8_t RmtMits1::getModeValueFromFields(uint8_t modeVal) {
    switch (modeVal) {
        case MITSUBISHI_HEAVY_MODE_AUTO:
            return MODE_AUTO;
        case MITSUBISHI_HEAVY_MODE_COOL:
            return MODE_COOL;
        case MITSUBISHI_HEAVY_MODE_DRY:
            return MODE_DRY;
        case MITSUBISHI_HEAVY_MODE_FAN:
            return MODE_FAN;
        case MITSUBISHI_HEAVY_MODE_HEAT:
            return MODE_HEAT;
        default:
            return MODE_AUTO;
    }
}

uint8_t RmtMits1::getFanSpeedValueForFields(uint8_t fanSpeedVal) {
    switch (fanSpeedVal) {
        case FAN_AUTO:
            return MITSUBISHI_HEAVY_ZJ_FAN_AUTO;
        case FAN_LOW:
            return MITSUBISHI_HEAVY_ZJ_FAN1;
        case FAN_MID:
            return MITSUBISHI_HEAVY_ZJ_FAN2;
        case FAN_HIGH:
            return MITSUBISHI_HEAVY_ZJ_FAN3;
        default:
            return MITSUBISHI_HEAVY_ZJ_FAN_AUTO;
    }
}

uint8_t RmtMits1::getFanSpeedValueFromFields(uint8_t fanSpeedVal) {
    switch (fanSpeedVal) {
        case MITSUBISHI_HEAVY_ZJ_FAN_AUTO:
            return FAN_AUTO;
        case MITSUBISHI_HEAVY_ZJ_FAN1:
            return FAN_LOW;
        case MITSUBISHI_HEAVY_ZJ_FAN2:
            return FAN_MID;
        case MITSUBISHI_HEAVY_ZJ_FAN3:
            return FAN_HIGH;
        default:
            return MODE_AUTO;
    }
}

uint8_t RmtMits1::getVerSwingValueForFields(uint8_t verSwingVal) {
    switch (verSwingVal) {
        case SWING_VER_MANUAL:
            return MITSUBISHI_HEAVY_ZJ_VS_STOP;
        case SWING_VER_AUTO:
            return MITSUBISHI_HEAVY_ZJ_VS_SWING;
        case SWING_VER_POS_1:
            return MITSUBISHI_HEAVY_ZJ_VS_UP;
        case SWING_VER_POS_2:
            return MITSUBISHI_HEAVY_ZJ_VS_MUP;
        case SWING_VER_POS_3:
            return MITSUBISHI_HEAVY_ZJ_VS_MIDDLE;
        case SWING_VER_POS_4:
            return MITSUBISHI_HEAVY_ZJ_VS_MDOWN;
        case SWING_VER_POS_5:
            return MITSUBISHI_HEAVY_ZJ_VS_DOWN;
        default:
            return MITSUBISHI_HEAVY_ZJ_VS_SWING;
    }
}

uint8_t RmtMits1::getVerSwingValueFromFields(uint8_t verSwingVal) {
    switch (verSwingVal) {
        case MITSUBISHI_HEAVY_ZJ_VS_SWING:
            return SWING_VER_AUTO;
        case MITSUBISHI_HEAVY_ZJ_VS_STOP:
            return SWING_VER_MANUAL;
        case MITSUBISHI_HEAVY_ZJ_VS_UP:
            return SWING_VER_POS_1;
        case MITSUBISHI_HEAVY_ZJ_VS_MUP:
            return SWING_VER_POS_2;
        case MITSUBISHI_HEAVY_ZJ_VS_MIDDLE:
            return SWING_VER_POS_3;
        case MITSUBISHI_HEAVY_ZJ_VS_MDOWN:
            return SWING_VER_POS_4;
        case MITSUBISHI_HEAVY_ZJ_VS_DOWN:
            return SWING_VER_POS_5;
        default:
            return SWING_VER_AUTO;
    }
}

uint8_t RmtMits1::getHorSwingValueForFields(uint8_t horSwingVal) {
    switch (horSwingVal) {
        case SWING_HOR_MANUAL:
            return MITSUBISHI_HEAVY_ZJ_HS_STOP;
            break;
        case SWING_HOR_AUTO:
            return MITSUBISHI_HEAVY_ZJ_HS_SWING;
        default:
            return MITSUBISHI_HEAVY_ZJ_HS_STOP;
    }
}

uint8_t RmtMits1::getHorSwingValueFromFields(uint8_t horSwingVal) {
    switch (horSwingVal) {
        case MITSUBISHI_HEAVY_ZJ_HS_SWING:
            return SWING_HOR_AUTO;
        case MITSUBISHI_HEAVY_ZJ_HS_STOP:
            return SWING_HOR_MANUAL;
        default:
            return SWING_HOR_AUTO;
    }
}

bool RmtMits1::isAcModelSupported(std::string acModelStr) {
    for (uint32_t i = 0; i < supportedAcModels.size(); i++) {
        std::string supportedAcModel = supportedAcModels.at(i);
        if (supportedAcModel.compare(acModelStr) == 0)
            return true;
    }
    return false;
}