//
// Created by Mannan on 2/28/2017.
//

#ifndef IRLIB_COMMON_H
#define IRLIB_COMMON_H

#include <stdint.h>
#include <stdarg.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>

#ifdef __ANDROID_API__ // ??
//#include <jni.h>
#include <android/log.h>
#endif

#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE

#endif

#define CM_CODE_JSON_PARSE_ERROR        10
#define CM_CODE_JSON_NO_KEY_ERROR       11

#define RF_REMOTE_ID_NOT_FOUND          100

#define EN_CODE_SUCCESS                 200
#define EN_CODE_REMOTE_NOT_FOUND        201

#define DE_CODE_SUCCESS                 300
#define DE_CODE_REMOTE_NOT_FOUND        301
#define DE_CODE_IR_DATA_LEN_ERROR       302
#define DE_CODE_IR_DATA_NOT_VALID       303

#define APPNAME "PELIRLIB"

typedef enum {
    MODE_AUTO = 0,
    MODE_COOL,
    MODE_DRY,
    MODE_FAN,
    MODE_HEAT
} MODE;

typedef enum {
    FAN_AUTO = 0,
    FAN_HIGH,
    FAN_MID,
    FAN_LOW
} FAN_SPEED;

typedef enum {
    SWING_VER_AUTO = 0,
    SWING_VER_MANUAL,
    SWING_VER_NATURAL,
    SWING_VER_POS_1,
    SWING_VER_POS_2,
    SWING_VER_POS_3,
    SWING_VER_POS_4,
    SWING_VER_POS_5
} SWING_VER;

typedef enum {
    SWING_HOR_AUTO = 0,
    SWING_HOR_MANUAL,
} SWING_HOR;

typedef struct {
    int32_t remoteId;
    std::string remoteModel;
    std::string remoteBrand;

    uint8_t powerOnOff;

    uint8_t operationMode;
    uint8_t setTemperature;
    uint8_t fanSpeed;
    uint8_t swingVertical;
    uint8_t swingHorizontal;

    uint8_t turboOnOff;
    uint8_t muteOnOff;
    uint8_t autoEvapOnOff;
    uint8_t displayOnOff;

    std::string decodedPacket;

} REMOTE_COMMON;

class Common {

public:
    Common();
    static void logIrLib(const char * __restrict fmt, ...);
    static std::string getErrorString(int32_t errorCode);
    static void printBuffer(uint8_t *buff, uint32_t buffLen, bool printByteNum);
    static uint8_t reverseByte(uint8_t b);
    static std::string convertShortToHexString(uint16_t val);
    static int16_t convertHexStringToShort(std::string str);
    static std::string convertCharArrayToHexString(uint8_t *buff, uint32_t buffLen);
    static bool isValueTolerable(int16_t val1, int16_t val2, int16_t margin);
    static uint8_t calcChksum8Mod256(uint8_t *buff, uint8_t buffLen);
};

#endif //IRLIB_COMMON_H
