#include "Common.h"

Common::Common() { }

void Common::logIrLib(const char * __restrict fmt, ...) {

    va_list va;
    va_start (va, fmt);

#ifdef __ANDROID_API__
    __android_log_vprint(ANDROID_LOG_VERBOSE, APPNAME, fmt, va);
#endif

#ifdef __APPLE__
    fprintf(stderr, fmt, va);
    fprintf(stderr, "\n");
#endif

    va_end (va);
}

std::string Common::getErrorString(int32_t errorCode) {

    switch (errorCode) {
        case CM_CODE_JSON_PARSE_ERROR:
            return "JSON Parsing Error!";
        case CM_CODE_JSON_NO_KEY_ERROR:
            return "JSON does not have a required key/value pair!";
        case RF_REMOTE_ID_NOT_FOUND:
            return "This remote id not found in the library!";
        case EN_CODE_REMOTE_NOT_FOUND:
            return "This remote is not supported by the library!";
        case DE_CODE_REMOTE_NOT_FOUND:
            return "This remote is not supported by the library!";
        case DE_CODE_IR_DATA_LEN_ERROR:
            return "IR Data length is not correct!";
        case DE_CODE_IR_DATA_NOT_VALID:
            return "IR Data is not valid!";
        default:
            return "No error string for this error code!";
    }
}

void Common::printBuffer(uint8_t *buff, uint32_t buffLen, bool printByteNum) {
    std::stringstream sstream;
    for (uint32_t i = 0; i < buffLen; i++) {
        sstream << " ";
        if (printByteNum) {
            sstream << i;
            sstream << "-";
        }
        uint8_t b = buff[i];
        for (uint8_t j = 0; j < 8; j++) {
            if ((b & 0x80) == 0x80) // 1
                sstream << "1";
            else // 0
                sstream << "0";
            b <<= 1;
        }
    }
    Common::logIrLib("decoded: %s", sstream.str().c_str());
}

uint8_t Common::reverseByte(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

std::string Common::convertShortToHexString(uint16_t val) {
    std::stringstream sstream;
    sstream << std::setfill ('0') << std::setw(sizeof(uint16_t)*2) << std::uppercase << std::hex << val;
    return sstream.str();
}

int16_t Common::convertHexStringToShort(std::string str) {
    std::stringstream sstream(str);
    int16_t val;
    sstream >> std::hex >> val;
    return val;
}

std::string Common::convertCharArrayToHexString(uint8_t *buff, uint32_t buffLen) {
    std::stringstream sstream;
    for (uint32_t i = 0; i < buffLen; i++)
        sstream << std::setfill ('0') << std::setw(sizeof(uint8_t)*2) << std::uppercase << std::hex << static_cast<unsigned>(buff[i]);
    return sstream.str();
}

bool Common::isValueTolerable(int16_t val1, int16_t val2, int16_t margin) {
    return (abs(val1 - val2) < margin);
}

uint8_t Common::calcChksum8Mod256(uint8_t *buff, uint8_t buffLen) {
    uint16_t sumVal = 0;
    for (uint16_t i = 0; i < buffLen; i++)
        sumVal += buff[i];
    return (uint8_t)(sumVal/* % 256*/);
}