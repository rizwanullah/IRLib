#ifndef IRLIB_RMTAUX1_H
#define IRLIB_RMTAUX1_H

#include "../Common.h"

// Remote Model: YKR-L/102E

#define PACKET_LEN_AUX1             13
#define IR_DATA_LEN_AUX1            844

typedef struct {
    uint8_t byte0Unknown:8;
    uint8_t byte1VerticalSwing:3;
    uint8_t byte1Temperature:5;
    uint8_t byte2Unknown1:5;
    uint8_t byte2HorizontalSwing:3;
    uint8_t byte3Unknown1:7;
    uint8_t byte3TemperatureFraction:1;
    uint8_t byte4TimerTimeValue:5;
    uint8_t byte4FanSpeed:3;
    uint8_t byte5Unknown2:6;
    uint8_t byte5TurboOnOff:1;
    uint8_t byte5SilentOnOff:1;
    uint8_t byte6Unknown2:2;
    uint8_t byte6SleepOnOff:1;
    uint8_t byte6IfeelOnOff:1; // It affects all bits of byte 7.
    uint8_t byte6HealthOnOff:1;
    uint8_t byte6OperationMode:3;
    uint8_t byte7Unknown:8;
    uint8_t byte8Unknown:8;
    uint8_t byte9Unknown2:2;
    uint8_t byte9AutoEvapOnOff:1;
    uint8_t byte9EcoOnOff:1;
    uint8_t byte9ElectHeatingOnOff:1;
    uint8_t byte9PowerOnOff:1;
    uint8_t byte9TimerOnOff:1;
    uint8_t byte9Unknown1:1;
    uint8_t byte10Unknown:8;
    uint8_t byte11KeyCode:5;
    uint8_t byte11Unknown1:3;
    uint8_t byte12Chksum8Mod256:8;
} FIELDS_AUX1_STRUCT;

class RmtAux1 {

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

    RmtAux1();
    std::string encodeRemote(REMOTE_COMMON *);
    int32_t decodeRemote(std::string, REMOTE_COMMON*);
    bool isAcModelSupported(std::string acModelStr);

};

#endif //IRLIB_RMTAUX1_H
