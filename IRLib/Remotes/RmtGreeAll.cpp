#include "RmtGreeAll.h"
#include "../Common.h"

// Remote Model: GREE (Generic, YAN, YAA)

int32_t RmtGreeAll::remoteId = 1013;

RmtGreeAll::RmtGreeAll() {
    headerMark = 8800; // 9000
    headerSpace = 4500;
    bitMark = 620;
    zeroSpace = 540;
    oneSpace = 1600;
    messageSpace = 19000; // 19000

    greeModel = GREE_YAA; // default

    // Update these to support PEL App
    supportedAcModels.push_back("SQGJCI");
    supportedAcModels.push_back("SRGJCI");
    supportedAcModels.push_back("STGJCI");
}

std::string RmtGreeAll::encodeRemote(REMOTE_COMMON *remoteCommon) {

    // start - taken from https://github.com/ToniA/arduino-heatpumpir

    uint8_t powerMode = GREE_AIRCON1_POWER_ON;
    uint8_t operatingMode = GREE_AIRCON1_MODE_COOL;
    uint8_t fanSpeed = GREE_AIRCON1_FAN_AUTO;
    uint8_t temperature = 21;
    uint8_t swingV = GREE_VDIR_AUTO;
    uint8_t swingH = GREE_HDIR_AUTO;

    if (remoteCommon->remoteModel.compare("Generic") == 0)
        greeModel = GREE_GENERIC;
    else if (remoteCommon->remoteModel.compare("Yan") == 0)
        greeModel = GREE_YAN;
    else if (remoteCommon->remoteModel.compare("Yaa") == 0)
        greeModel = GREE_YAA;

    if (remoteCommon->powerOnOff == 0) {
        powerMode = GREE_AIRCON1_POWER_OFF;
    } else {
        switch (remoteCommon->operationMode) {
            case MODE_AUTO:
                operatingMode = GREE_AIRCON1_MODE_AUTO;
                remoteCommon->setTemperature = 25;
                break;
            case MODE_HEAT:
                operatingMode = GREE_AIRCON1_MODE_HEAT;
                break;
            case MODE_COOL:
                operatingMode = GREE_AIRCON1_MODE_COOL;
                break;
            case MODE_DRY:
                operatingMode = GREE_AIRCON1_MODE_DRY;
                remoteCommon->fanSpeed = FAN_LOW;
                break;
            case MODE_FAN:
                operatingMode = GREE_AIRCON1_MODE_FAN;
                break;
            default:
                operatingMode = GREE_AIRCON1_MODE_AUTO;
                remoteCommon->setTemperature = 25;
                break;
        }
    }

    switch (remoteCommon->fanSpeed) {
        case FAN_AUTO:
            fanSpeed = GREE_AIRCON1_FAN_AUTO;
            break;
        case FAN_LOW:
            fanSpeed = GREE_AIRCON1_FAN1;
            break;
        case FAN_MID:
            fanSpeed = GREE_AIRCON1_FAN2;
            break;
        case FAN_HIGH:
            fanSpeed = GREE_AIRCON1_FAN3;
            break;
        default:
            fanSpeed = GREE_AIRCON1_FAN_AUTO;
            break;
    }


    if (greeModel == GREE_YAN || greeModel == GREE_YAA) {
        switch (remoteCommon->swingHorizontal) {
            case SWING_VER_AUTO:
                swingV = GREE_VDIR_AUTO;
            case SWING_VER_MANUAL:
                swingV = GREE_VDIR_MANUAL;
            case SWING_VER_POS_1:
                swingV = GREE_VDIR_UP;
            case SWING_VER_POS_2:
                swingV = GREE_VDIR_MUP;
            case SWING_VER_POS_3:
                swingV = GREE_VDIR_MIDDLE;
            case SWING_VER_POS_4:
                swingV = GREE_VDIR_MDOWN;
            case SWING_VER_POS_5:
                swingV = GREE_VDIR_DOWN;
            default:
                swingV = GREE_VDIR_AUTO;
        }
    }

    if (remoteCommon->setTemperature > 15 && remoteCommon->setTemperature < 31)
        temperature = remoteCommon->setTemperature - (uint8_t)16;


    uint8_t GreeTemplate[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00 };

    uint8_t i;

    GreeTemplate[0] = fanSpeed | operatingMode | powerMode;
    GreeTemplate[1] = temperature;

    if (greeModel == GREE_YAN) {
        GreeTemplate[2] = 0x60;
        GreeTemplate[2] = 0x50;

        if (remoteCommon->turboOnOff)
            GreeTemplate[2]=0x70;

        GreeTemplate[4] = swingV;
    }
    if (greeModel == GREE_YAA) {
        //GreeTemplate[2] = 0xE0; // bits 0..3 always 0000, bits 4..7 TURBO,LIGHT,HEALTH,X-FAN
        GreeTemplate[2] = 0x20; // bits 0..3 always 0000, bits 4..7 TURBO,LIGHT,HEALTH,X-FAN
        GreeTemplate[3] = 0x50; // bits 4..7 always 0101
        GreeTemplate[6] = 0x20; // YAA1FB, FAA1FB1, YB1F2 bits 4..7 always 0010

        if (remoteCommon->turboOnOff) {
            GreeTemplate[2] |= (1 << 4); // Set bit 4 (TURBO)
        }
        if (swingV == GREE_VDIR_SWING) {
            GreeTemplate[0] |= (1 << 6); // Enable swing by setting bit 6
        } else if (swingV != GREE_VDIR_AUTO) {
            GreeTemplate[5] = swingV;
        }
    }

    // Calculate the checksum
    if (greeModel == GREE_YAN)
    {
        GreeTemplate[7] = (uint8_t) (
                (GreeTemplate[0] << 4) +
                (GreeTemplate[1] << 4) +
                0xC0);
    }
    else
    {
        GreeTemplate[7] = (uint8_t) (((
        (GreeTemplate[0] & 0x0F) +
        (GreeTemplate[1] & 0x0F) +
        (GreeTemplate[2] & 0x0F) +
        (GreeTemplate[3] & 0x0F) +
        ((GreeTemplate[5] & 0xF0) >> 4) +
        ((GreeTemplate[6] & 0xF0) >> 4) +
        ((GreeTemplate[7] & 0xF0) >> 4) +
        0x0A) & 0x0F) << 4) | (GreeTemplate[7] & (uint8_t)0x0F);
    }

    // end

    std::string encodedIrData;

    std::string bitMarkStr = Common::convertShortToHexString(bitMark);
    std::string oneSpaceStr = Common::convertShortToHexString(oneSpace);
    std::string zeroSpaceStr = Common::convertShortToHexString(zeroSpace);
    std::string messageSpaceStr = Common::convertShortToHexString(messageSpace);

    uint8_t remoteBuffer[PACKET_LEN_GREEALL];
    for (uint16_t i = 0; i < PACKET_LEN_GREEALL; i++) {
        //Common::logIrLib("enc byte %02d: %02X", i, GreeTemplate[i]);
        remoteBuffer[i] = Common::reverseByte(GreeTemplate[i]);
    }

    encodedIrData.append(Common::convertShortToHexString(headerMark));
    encodedIrData.append(Common::convertShortToHexString(headerSpace));

    for (uint16_t i = 0; i < PACKET_LEN_GREEALL; i++) {
        uint8_t byte = remoteBuffer[i];
        for (uint8_t j = 0; j < 8; j++) {
            if ((byte & 0x80) == 0x80) {
                encodedIrData.append(bitMarkStr);
                encodedIrData.append(oneSpaceStr);
            } else {
                encodedIrData.append(bitMarkStr);
                encodedIrData.append(zeroSpaceStr);
            }
            byte <<= 1;
        }

        if (i == 3) {
            // encode '010'
            encodedIrData.append(bitMarkStr); encodedIrData.append(zeroSpaceStr);
            encodedIrData.append(bitMarkStr); encodedIrData.append(oneSpaceStr);
            encodedIrData.append(bitMarkStr); encodedIrData.append(zeroSpaceStr);

            // encode message gap (19000)
            encodedIrData.append(bitMarkStr); encodedIrData.append(messageSpaceStr);
        }
    }

    encodedIrData.append(bitMarkStr); encodedIrData.append(zeroSpaceStr);

    return encodedIrData;
}

int32_t RmtGreeAll::decodeRemote(std::string encodedIrData, REMOTE_COMMON *remoteCommon) {
    return DE_CODE_REMOTE_NOT_FOUND;
}

bool RmtGreeAll::isAcModelSupported(std::string acModelStr) {
    for (uint32_t i = 0; i < supportedAcModels.size(); i++) {
        std::string supportedAcModel = supportedAcModels.at(i);
        if (supportedAcModel.compare(acModelStr) == 0)
            return true;
    }
    return false;
}

