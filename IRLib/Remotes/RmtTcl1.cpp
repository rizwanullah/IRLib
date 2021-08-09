#include "RmtTcl1.h"
#include "../Common.h"

// Remote Model: Unknown TCL

int32_t RmtTcl1::remoteId = 1002;

RmtTcl1::RmtTcl1() {
    headerMark = 2500; //3080;
    headerSpace = 1600;
    footerMark = 500;
    zeroMark = 500;
    zeroSpace = 320;
    oneMark = 500;
    oneSpace = 1080;

    // Miracle
    supportedAcModels.push_back("SQTMI");
    supportedAcModels.push_back("SRTMI");
    // Invert o Mate
    supportedAcModels.push_back("SQTIMH");
    supportedAcModels.push_back("SRTIMH");
}

std::string RmtTcl1::encodeRemote(REMOTE_COMMON *remoteCommon) {

    std::string encodedIrData;

    std::string oneMarkStr = Common::convertShortToHexString(oneMark);
    std::string oneSpaceStr = Common::convertShortToHexString(oneSpace);
    std::string zeroMarkStr = Common::convertShortToHexString(zeroMark);
    std::string zeroSpaceStr = Common::convertShortToHexString(zeroSpace);

    uint8_t fieldsBuffer[PACKET_LEN_TCL1] = {0};
    FIELDS_TCL1_STRUCT *fieldsTcl1Struct = (FIELDS_TCL1_STRUCT *) &fieldsBuffer[0];

    fieldsTcl1Struct->byte0RemoteType = 0x23;
    fieldsTcl1Struct->byte1FixedCode = 0xCB;
    fieldsTcl1Struct->byte2FixedCode = 0x26;
    fieldsTcl1Struct->byte3FuncCode = 1;
    fieldsTcl1Struct->byte3MfgCode = 0; // TCL
    fieldsTcl1Struct->byte4FixedCode = 0;
    fieldsTcl1Struct->byte5BodyCode = 0;
    fieldsTcl1Struct->byte5PowerOnOff = remoteCommon->powerOnOff;
    fieldsTcl1Struct->byte5TimerMode = 0; // timer off
    fieldsTcl1Struct->byte5MuteOnOff = remoteCommon->muteOnOff;
    fieldsTcl1Struct->byte5DisplayOnOff = (uint8_t) (remoteCommon->displayOnOff == 0 ? 1 : 0);
    fieldsTcl1Struct->byte5EcoOnOff = 0;
    fieldsTcl1Struct->byte6OperationMode = getModeValueForFields(remoteCommon->operationMode);
    fieldsTcl1Struct->byte6HealthOnOff = 0;
    fieldsTcl1Struct->byte6MildewOnOff = remoteCommon->autoEvapOnOff;
    fieldsTcl1Struct->byte6TurboOnOff = remoteCommon->turboOnOff;
    fieldsTcl1Struct->byte6CarryOnOff = 0;
    fieldsTcl1Struct->byte7SetTemperature = (uint8_t) ((remoteCommon->setTemperature - 16) ^ 0x0F);
    fieldsTcl1Struct->byte7TemperatureSwitch = 0; // TODO: verify if this is iFeel
    fieldsTcl1Struct->byte8FanSpeed = getFanSpeedValueForFields(remoteCommon->fanSpeed);
    fieldsTcl1Struct->byte8VerticalSwing = getVerSwingValueForFields(remoteCommon->swingVertical);
    fieldsTcl1Struct->byte8MemoryOnOff = 0;
    fieldsTcl1Struct->byte8IfeelOnOff = 0; // TODO: is this related to byte7TemperatureSwitch?
    fieldsTcl1Struct->byte9TimerStopSettings = 0;
    fieldsTcl1Struct->byte10TimerSartSetting = 0;
    fieldsTcl1Struct->byte11Unknown = 0;
    fieldsTcl1Struct->byte12Unknown = 0;
    fieldsTcl1Struct->byte12AutoCleanOnOff = 0;
    fieldsTcl1Struct->byte12ElectHeatingOnOff = 0;
    fieldsTcl1Struct->byte13Chksum8Mod256 = Common::calcChksum8Mod256(fieldsBuffer, PACKET_LEN_TCL1 - 1);

    uint8_t remoteBuffer[PACKET_LEN_TCL1];
    for (uint16_t i = 0; i < PACKET_LEN_TCL1; i++) {
        remoteBuffer[i] = Common::reverseByte(fieldsBuffer[i]);
    }

    encodedIrData.append(Common::convertShortToHexString(headerMark));
    encodedIrData.append(Common::convertShortToHexString(headerSpace));

    for (uint16_t i = 0; i < PACKET_LEN_TCL1; i++) {
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

int32_t RmtTcl1::decodeRemote(std::string encodedIrData, REMOTE_COMMON *remoteCommon) {
    if (encodedIrData.length() != IR_DATA_LEN_TCL1) {
        Common::logIrLib("IR Data length is not correct! size is %d", encodedIrData.length());
        return DE_CODE_IR_DATA_LEN_ERROR;
    }

    int16_t value = 0;
    uint8_t bitIndex = 0;
    uint8_t byteIndex = 0;
    uint8_t remoteBuffer[PACKET_LEN_TCL1];

    value = Common::convertHexStringToShort(encodedIrData.substr(0, 4));
    if (!Common::isValueTolerable(value, headerMark, 400)) return DE_CODE_IR_DATA_NOT_VALID;
    value = Common::convertHexStringToShort(encodedIrData.substr(4, 4));
    if (!Common::isValueTolerable(value, headerSpace, 400)) return DE_CODE_IR_DATA_NOT_VALID;

    for (uint32_t i = 8; i < 904; i += 8) {
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

    //value = Common::convertHexStringToShort(encodedIrData.substr(840, 4));
    //if (!Common::isValueTolerable(value, footerMark, 400)) return DE_CODE_IR_DATA_NOT_VALID;

    for (uint8_t i = 0; i < PACKET_LEN_TCL1; i++) {
        remoteBuffer[i] = Common::reverseByte(remoteBuffer[i]);
    }

    uint8_t chksum8Mod256 = Common::calcChksum8Mod256(remoteBuffer, PACKET_LEN_TCL1 - 1);
    if (chksum8Mod256 != remoteBuffer[PACKET_LEN_TCL1 - 1])
        return DE_CODE_IR_DATA_NOT_VALID;

    FIELDS_TCL1_STRUCT *fieldsTcl1Struct = (FIELDS_TCL1_STRUCT *) &remoteBuffer[0];

    remoteCommon->muteOnOff = fieldsTcl1Struct->byte5MuteOnOff;
    remoteCommon->turboOnOff = fieldsTcl1Struct->byte6TurboOnOff;
    remoteCommon->powerOnOff = fieldsTcl1Struct->byte5PowerOnOff;
    remoteCommon->swingVertical = getVerSwingValueFromFields(fieldsTcl1Struct->byte8VerticalSwing);
    remoteCommon->fanSpeed = getFanSpeedValueFromFields(fieldsTcl1Struct->byte8FanSpeed);
    remoteCommon->setTemperature = (uint8_t) ((fieldsTcl1Struct->byte7SetTemperature ^ 0x0F) + 16);
    remoteCommon->operationMode = getModeValueFromFields(fieldsTcl1Struct->byte6OperationMode);
    remoteCommon->autoEvapOnOff = fieldsTcl1Struct->byte6MildewOnOff;
    remoteCommon->displayOnOff = (uint8_t) (fieldsTcl1Struct->byte5DisplayOnOff == 0 ? 1 : 0);

    // Not supported by this remote
    remoteCommon->swingHorizontal = 0;

    remoteCommon->remoteModel = "Uknown";
    remoteCommon->remoteBrand = "TCL";

    remoteCommon->decodedPacket = Common::convertCharArrayToHexString(remoteBuffer, sizeof(remoteBuffer));

    Common::printBuffer(remoteBuffer, sizeof(remoteBuffer), true);

    return DE_CODE_SUCCESS;
}

uint8_t RmtTcl1::getModeValueForFields(uint8_t modeVal) {
    switch (modeVal) {
        case MODE_AUTO:
            return 0x08;
        case MODE_COOL:
            return 0x03;
        case MODE_DRY:
            return 0x02;
        case MODE_FAN:
            return 0x07;
        case MODE_HEAT:
            return 0x01;
        default:
            return 0x08;
    }
}

uint8_t RmtTcl1::getModeValueFromFields(uint8_t modeVal) {
    switch (modeVal) {
        case 0x08:
            return MODE_AUTO;
        case 0x03:
            return MODE_COOL;
        case 0x02:
            return MODE_DRY;
        case 0x07:
            return MODE_FAN;
        case 0x01:
            return MODE_HEAT;
        default:
            return MODE_AUTO;
    }
}

uint8_t RmtTcl1::getFanSpeedValueForFields(uint8_t fanSpeedVal) {
    // TODO: in table 4, there are more values for fan speed, incorporate them too.
    switch (fanSpeedVal) {
        case FAN_AUTO:
            return 0;
        case FAN_LOW:
            return 2;
        case FAN_MID:
            return 3;
        case FAN_HIGH:
            return 5;
        default:
            return 0;
    }
}

uint8_t RmtTcl1::getFanSpeedValueFromFields(uint8_t fanSpeedVal) {
    // TODO: in table 4, there are more values for fan speed, incorporate them too.
    switch (fanSpeedVal) {
        case 0:
            return FAN_AUTO;
        case 2:
            return FAN_LOW;
        case 3:
            return FAN_MID;
        case 5:
            return FAN_HIGH;
        default:
            return MODE_AUTO;
    }
}

uint8_t RmtTcl1::getVerSwingValueForFields(uint8_t verSwingVal) {
    switch (verSwingVal) {
        case SWING_VER_AUTO:
            return 7;
        case SWING_VER_MANUAL:
            return 0;
        default:
            return 7;
    }
}

uint8_t RmtTcl1::getVerSwingValueFromFields(uint8_t verSwingVal) {
    switch (verSwingVal) {
        case 7:
            return SWING_VER_AUTO;
        case 0:
            return SWING_VER_MANUAL;
        default:
            return SWING_VER_AUTO;
    }
}

uint8_t RmtTcl1::getHorSwingValueForFields(uint8_t horSwingVal) {
    switch (horSwingVal) {
        case SWING_HOR_AUTO:
            return 7;
        case SWING_HOR_MANUAL:
            return 0;
        default:
            return 7;
    }
}

uint8_t RmtTcl1::getHorSwingValueFromFields(uint8_t horSwingVal) {
    switch (horSwingVal) {
        case 7:
            return SWING_HOR_AUTO;
        case 0:
            return SWING_HOR_MANUAL;
        default:
            return SWING_HOR_AUTO;
    }
}

bool RmtTcl1::isAcModelSupported(std::string acModelStr) {
    for (uint32_t i = 0; i < supportedAcModels.size(); i++) {
        std::string supportedAcModel = supportedAcModels.at(i);
        if (supportedAcModel.compare(acModelStr) == 0)
            return true;
    }
    return false;
}