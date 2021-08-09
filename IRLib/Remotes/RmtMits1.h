#ifndef IRLIB_RMTMITS1_H
#define IRLIB_RMTMITS1_H

#include "../Common.h"

// Remote Model: YKR-L/102E

#define PACKET_LEN_MITS1             11
#define IR_DATA_LEN_MITS1            716

// Mitsubishi Heavy codes
#define MITSUBISHI_HEAVY_MODE_AUTO         0x07 // Operating mode
#define MITSUBISHI_HEAVY_MODE_HEAT         0x03
#define MITSUBISHI_HEAVY_MODE_COOL         0x06
#define MITSUBISHI_HEAVY_MODE_DRY          0x05
#define MITSUBISHI_HEAVY_MODE_FAN          0x04

#define MITSUBISHI_HEAVY_MODE_OFF          0x08 // Power OFF
#define MITSUBISHI_HEAVY_MODE_ON           0x00 // Power ON

#define MITSUBISHI_HEAVY_ZJ_FAN_AUTO       0xE0 // Fan speed
#define MITSUBISHI_HEAVY_ZJ_FAN1           0xA0
#define MITSUBISHI_HEAVY_ZJ_FAN2           0x80
#define MITSUBISHI_HEAVY_ZJ_FAN3           0x60
#define MITSUBISHI_HEAVY_ZJ_HIPOWER        0x40
#define MITSUBISHI_HEAVY_ZJ_ECONO          0x00

#define MITSUBISHI_HEAVY_CLEAN_ON          0x00
#define MITSUBISHI_HEAVY_ZJ_CLEAN_OFF      0x20

#define MITSUBISHI_HEAVY_ZJ_SILENT_ON      0x00

#define MITSUBISHI_HEAVY_ZJ_VS_SWING       0x0A // Vertical swing
#define MITSUBISHI_HEAVY_ZJ_VS_UP          0x02
#define MITSUBISHI_HEAVY_ZJ_VS_MUP         0x18
#define MITSUBISHI_HEAVY_ZJ_VS_MIDDLE      0x10
#define MITSUBISHI_HEAVY_ZJ_VS_MDOWN       0x08
#define MITSUBISHI_HEAVY_ZJ_VS_DOWN        0x00
#define MITSUBISHI_HEAVY_ZJ_VS_STOP        0x1A

#define MITSUBISHI_HEAVY_ZJ_HS_SWING       0x4C // Horizontal swing - 3D AUTO
#define MITSUBISHI_HEAVY_ZJ_HS_MIDDLE      0x48
#define MITSUBISHI_HEAVY_ZJ_HS_LEFT        0xC8
#define MITSUBISHI_HEAVY_ZJ_HS_MLEFT       0x88
#define MITSUBISHI_HEAVY_ZJ_HS_MRIGHT      0x08
#define MITSUBISHI_HEAVY_ZJ_HS_RIGHT       0xC4
#define MITSUBISHI_HEAVY_ZJ_HS_STOP        0xCC
#define MITSUBISHI_HEAVY_ZJ_HS_LEFTRIGHT   0x84
#define MITSUBISHI_HEAVY_ZJ_HS_RIGHTLEFT   0x44
#define MITSUBISHI_HEAVY_ZJ_HS_3DAUTO      0x04

class RmtMits1 {

private:
    uint16_t headerMark;
    uint16_t headerSpace;
    uint16_t footerMark;
    uint16_t zeroMark;
    uint16_t zeroSpace;
    uint16_t oneMark;
    uint16_t oneSpace;

    std::vector<std::string> supportedAcModels;
    uint8_t getModeValueForFields(uint8_t modeVal);
    uint8_t getModeValueFromFields(uint8_t modeVal);
    uint8_t getFanSpeedValueForFields(uint8_t fanSpeedVal);
    uint8_t getFanSpeedValueFromFields(uint8_t fanSpeedVal);
    uint8_t getVerSwingValueForFields(uint8_t verSwingVal);
    uint8_t getVerSwingValueFromFields(uint8_t verSwingVal);
    uint8_t getHorSwingValueForFields(uint8_t horSwingVal);
    uint8_t getHorSwingValueFromFields(uint8_t horSwingVal);

public:
    static int32_t remoteId;

    RmtMits1();
    std::string encodeRemote(REMOTE_COMMON *);
    int32_t decodeRemote(std::string, REMOTE_COMMON*);
    bool isAcModelSupported(std::string acModelStr);

};

#endif //IRLIB_RMTMITS1_H
