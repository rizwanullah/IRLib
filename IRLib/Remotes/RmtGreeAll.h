#ifndef IRLIB_RMTGREEALL_H
#define IRLIB_RMTGREEALL_H

#include "../Common.h"

// Remote Model: GREE (Generic, YAN, YAA)

#define PACKET_LEN_GREEALL          8
#define IR_DATA_LEN_GREEALL         272


// start - taken from https://github.com/ToniA/arduino-heatpumpir

#define GREE_AIRCON1_POWER_OFF  0x00
#define GREE_AIRCON1_POWER_ON   0x08

#define GREE_AIRCON1_MODE_AUTO  0x00
#define GREE_AIRCON1_MODE_COOL  0x01
#define GREE_AIRCON1_MODE_DRY   0x02
#define GREE_AIRCON1_MODE_FAN   0x03
#define GREE_AIRCON1_MODE_HEAT  0x04

#define GREE_AIRCON1_FAN_AUTO   0x00 // Fan speed
#define GREE_AIRCON1_FAN1       0x10 // * low
#define GREE_AIRCON1_FAN2       0x20 // * med
#define GREE_AIRCON1_FAN3       0x30 // * high
#define GREE_AIRCON1_TURBO      0x80 // * turbo mode on YAN

// Only available on YAN
// Vertical air directions. Note that these cannot be set on all heat pumps
#define GREE_VDIR_AUTO   0x00
#define GREE_VDIR_MANUAL 0x00
#define GREE_VDIR_SWING  0x01
#define GREE_VDIR_UP     0x02
#define GREE_VDIR_MUP    0x03
#define GREE_VDIR_MIDDLE 0x04
#define GREE_VDIR_MDOWN  0x05
#define GREE_VDIR_DOWN   0x06

// Not available in this model.
// Horizontal air directions. Note that these cannot be set on all heat pumps
#define GREE_HDIR_AUTO   0
#define GREE_HDIR_MANUAL 0
#define GREE_HDIR_SWING  0
#define GREE_HDIR_MIDDLE 0
#define GREE_HDIR_LEFT   0
#define GREE_HDIR_MLEFT  0
#define GREE_HDIR_MRIGHT 0
#define GREE_HDIR_RIGHT  0

#define GREE_GENERIC 0
#define GREE_YAN     1
#define GREE_YAA     2
// end




class RmtGreeAll {

private:
    uint16_t headerMark;
    uint16_t headerSpace;
    uint16_t bitMark;
    uint16_t zeroSpace;
    uint16_t oneSpace;
    uint16_t messageSpace;

    uint8_t greeModel;

    std::vector<std::string> supportedAcModels;

public:
    static int32_t remoteId;

    RmtGreeAll();
    std::string encodeRemote(REMOTE_COMMON *);
    int32_t decodeRemote(std::string, REMOTE_COMMON*);
    bool isAcModelSupported(std::string acModelStr);

};

#endif //IRLIB_RMTGREEALL_H
