#ifndef IRLIB_RMTTCL1_H
#define IRLIB_RMTTCL1_H

#include "../Common.h"

// Remote Model: Unknown TCL

#define PACKET_LEN_TCL1             14
#define IR_DATA_LEN_TCL1            908

typedef struct {
    uint8_t byte0RemoteType:8;
    uint8_t byte1FixedCode:8;
    uint8_t byte2FixedCode:8;
    uint8_t byte3FuncCode:4;
    uint8_t byte3MfgCode:4;
    uint8_t byte4FixedCode:8;
    uint8_t byte5BodyCode:2;
    uint8_t byte5PowerOnOff:1;
    uint8_t byte5TimerMode:2;
    uint8_t byte5MuteOnOff:1;
    uint8_t byte5DisplayOnOff:1;
    uint8_t byte5EcoOnOff:1;
    uint8_t byte6OperationMode:4; // refer to table 1
    uint8_t byte6HealthOnOff:1;
    uint8_t byte6MildewOnOff:1;
    uint8_t byte6TurboOnOff:1;
    uint8_t byte6CarryOnOff:1;
    uint8_t byte7SetTemperature:4; // refer to table 2
    uint8_t byte7TemperatureSwitch:4; // refer to table 3
    uint8_t byte8FanSpeed:3; // refer to table 4
    uint8_t byte8VerticalSwing:3; // refer to table 5
    uint8_t byte8MemoryOnOff:1;
    uint8_t byte8IfeelOnOff:1;
    uint8_t byte9TimerStopSettings:8; // refer to table 6
    uint8_t byte10TimerSartSetting:8; // refer to table 6
    uint8_t byte11Unknown:8; // refer to table 7. TODO: translate this field from doc.
    uint8_t byte12Unknown:6; // TODO: translate these fields from doc.
    uint8_t byte12AutoCleanOnOff:1; // not using it as no button available in remote. using mildew instead.
    uint8_t byte12ElectHeatingOnOff:1;
    uint8_t byte13Chksum8Mod256:8;
} FIELDS_TCL1_STRUCT;

class RmtTcl1 {

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

    RmtTcl1();
    std::string encodeRemote(REMOTE_COMMON *);
    int32_t decodeRemote(std::string, REMOTE_COMMON*);
    bool isAcModelSupported(std::string acModelStr);

};

#endif //IRLIB_RMTTCL1_H
