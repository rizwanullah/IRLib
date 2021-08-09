#ifndef IRLIB_CHIGO1_H
#define IRLIB_CHIGO1_H

#include "../Common.h"

// Remote Model: ZH/JT-03

#define PACKET_LEN_CHIGO1           12
#define IR_DATA_LEN_CHIGO1          788

// For temperature table, see "IR CODE130130 - Chigo (Translated by PEL).pdf"

typedef struct {
    uint8_t byte0TimerTime:5;
    uint8_t byte0TimerEnable:1;
    uint8_t byte0TimerFixedOnOff:1;
    uint8_t byte0TimerLimitOnOff:1;
    uint8_t byte1DisplayOnOff:1;
    uint8_t byte1HealthOnOff:1;
    uint8_t byte1LockKeyOnOff:1;
    uint8_t byte1TurboOnOff:1;
    uint8_t byte1EconomyOnOff:1;
    uint8_t byte1ElectHeatingOnOff:1;
    uint8_t byte1HumanDetectOnOff:1;
    uint8_t byte1AutoCleanOnOff:1;
    uint8_t byte2PressedKeyCode:5;
    uint8_t byte2IonFunctionOnOff:1;
    uint8_t byte2LeavingHomeOnOff:1;
    uint8_t byte2VentilationOnOff:1;
    uint8_t byte3SleepOnOff:1;
    uint8_t byte3PowerOnOff:1;
    uint8_t byte3OuterFlapControl:2;    // 00: natural wind 01: sweeping 10: Fixed wind
    uint8_t byte3InnerFlapControl:1;    // 0: sweep Wind 1: Fixed wind
    uint8_t byte3WindControl:2;         // 00: Auto Wind 01: High wind 10: Stroke 11: Low wind
    uint8_t byte3TemperatureUnit:1;     // 0: Celsius 1: Fahrenheit
    uint8_t byte4SetTemperature:5;      // See 'Temperature Table' above
    uint8_t byte4OperationMode:3;       // 000: Automatic, 001: Cooling, 010: Dry,  011: Fan, 100: Heating
    uint8_t byte5Reserved:8;
} FIELDS_CHIGO1_STRUCT;

class RmtChigo1 {

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

    RmtChigo1();
    std::string encodeRemote(REMOTE_COMMON *);
    int32_t decodeRemote(std::string, REMOTE_COMMON*);
    bool isAcModelSupported(std::string acModelStr);
};

#endif //IRLIB_CHIGO1_H
