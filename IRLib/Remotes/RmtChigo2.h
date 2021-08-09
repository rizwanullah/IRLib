#ifndef IRLIB_CHIGO2_H
#define IRLIB_CHIGO2_H

#include "../Common.h"

// Remote Model: ZH/ZH-03

#define PACKET_LEN_CHIGO2           12
#define IR_DATA_LEN_CHIGO2          788

typedef struct {
    uint8_t byte0AcOnTime:5;
    uint8_t byte0Reserved:3;
    uint8_t byte1Reserved1:1;
    uint8_t byte1Reserved2:1;
    uint8_t byte1IonizerOnOff:1;
    uint8_t byte1Reserved3:1;
    uint8_t byte1Reserved4:1;
    uint8_t byte1TimerChangeOnOff:1;
    uint8_t byte1OnTimerOnOff:1;
    uint8_t byte1SwitchOnOff:1;
    uint8_t byte2Reserved:8;
    uint8_t byte3DisplayOnOff:1;
    uint8_t byte3HealthOnOff:1;
    uint8_t byte3LockOnOff:1;
    uint8_t byte3TurboOnOff:1;
    uint8_t byte3EcoOnOff:1;
    uint8_t byte3ElectHeatingOnOff:1; //  this is linked with T8.7
    uint8_t byte3HumanSenseOnOff:1;
    uint8_t byte3AutoCleanOnOff:1;
    uint8_t byte4AcOffTime:5;
    uint8_t byte4Reserved:3;
    uint8_t byte5KeyCode:5;
    uint8_t byte5ResvButtonCode:1;
    uint8_t byte5ResvFloorStanding:1;
    uint8_t byte5VentilationOnOff:1;
    uint8_t byte6Reserved:8;
    uint8_t byte7SleepOnOff:1;
    uint8_t byte7PowerOnOff:1;
    uint8_t byte7VerticalSwing:2;
    uint8_t byte7HorizontalSwing:1;
    uint8_t byte7FanSpeed:2;
    uint8_t byte7TemperatureUnit:1;
    uint8_t byte8BodySenseTemperature:6;
    uint8_t byte8BodySenseOnOff:1;
    uint8_t byte8ElectHeatingOnOff:1; //  this is linked with T3.5
    uint8_t byte9SetTemperature:5;
    uint8_t byte9OperationMode:3;
    uint8_t byte10Reserved:8;
    uint8_t byte11Chksum8Mod256:8;
} FIELDS_CHIGO2_STRUCT;

class RmtChigo2 {

private:
    uint16_t headerMark;
    uint16_t headerSpace;
    uint16_t footerMark1;
    uint16_t footerSpace;
    uint16_t footerMark2;
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

    RmtChigo2();
    std::string encodeRemote(REMOTE_COMMON *);
    int32_t decodeRemote(std::string, REMOTE_COMMON*);
    bool isAcModelSupported(std::string acModelStr);
};

#endif //IRLIB_CHIGO2_H
