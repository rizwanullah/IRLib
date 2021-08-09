#include "Encoder.h"
#include "json.hpp"
#include "Common.h"

using json = nlohmann::json;

Encoder::Encoder() { }

std::string Encoder::encode(std::string remoteJsonStr) {

    REMOTE_COMMON remoteCommon;
    json remoteJson;
    json encodedRemoteJson;

    //memset(&remoteCommon, 0, sizeof(remoteCommon));

    try {
        remoteJson = json::parse(remoteJsonStr);
    } catch (std::exception &e) {
        Common::logIrLib("Exception: %s", e.what());
        return createResponseJson(CM_CODE_JSON_PARSE_ERROR, e.what(), "");
    }

    try {
        remoteCommon.remoteId = remoteJson["remoteId"];
    } catch (std::exception &e) {
        Common::logIrLib("Exception: %s", e.what());
        return createResponseJson(CM_CODE_JSON_NO_KEY_ERROR, e.what(), "");
    }

    try {
        remoteCommon.remoteModel = remoteJson["remoteModel"];
    } catch (std::exception &e) {
        remoteCommon.remoteModel = "";
    }

    try {
        remoteCommon.remoteBrand = remoteJson["remoteBrand"];
    } catch (std::exception &e) {
        remoteCommon.remoteBrand = "";
    }

    try {
        remoteCommon.powerOnOff = remoteJson["powerOnOff"];
    } catch (std::exception &e) {
        remoteCommon.powerOnOff = 1; // ON
    }

    try {
        remoteCommon.operationMode = remoteJson["operationMode"];
    } catch (std::exception &e) {
        remoteCommon.operationMode = 0; // Automatic
    }

    try {
        remoteCommon.setTemperature = remoteJson["setTemperature"];
    } catch (std::exception &e) {
        remoteCommon.setTemperature = 10; // 10 + 16 = 26
    }

    try {
        remoteCommon.fanSpeed = remoteJson["fanSpeed"];
    } catch (std::exception &e) {
        remoteCommon.fanSpeed = 0; // Automatic
    }

    try {
        remoteCommon.swingVertical = remoteJson["swingVertical"];
    } catch (std::exception &e) {
        remoteCommon.swingVertical = 0;
    }

    try {
        remoteCommon.swingHorizontal = remoteJson["swingHorizontal"];
    } catch (std::exception &e) {
        remoteCommon.swingHorizontal = 0;
    }

    try {
        remoteCommon.turboOnOff = remoteJson["turboOnOff"];
    } catch (std::exception &e) {
        remoteCommon.turboOnOff = 0;
    }

    try {
        remoteCommon.muteOnOff = remoteJson["muteOnOff"];
    } catch (std::exception &e) {
        remoteCommon.muteOnOff = 0;
    }

    try {
        remoteCommon.autoEvapOnOff = remoteJson["autoEvapOnOff"];
    } catch (std::exception &e) {
        remoteCommon.autoEvapOnOff = 0;
    }

    try {
        remoteCommon.displayOnOff = remoteJson["displayOnOff"];
    } catch (std::exception &e) {
        remoteCommon.displayOnOff = 0;
    }

    std::string encodedIrData;

    if (remoteCommon.remoteId == RmtChigo1::remoteId) {
        RmtChigo1 *rmtChigo1 = new RmtChigo1();
        encodedIrData = rmtChigo1->encodeRemote(&remoteCommon);
    } else if (remoteCommon.remoteId == RmtAux1::remoteId) {
        RmtAux1 *rmtAux1 = new RmtAux1();
        encodedIrData = rmtAux1->encodeRemote(&remoteCommon);
    } else if (remoteCommon.remoteId == RmtTcl1::remoteId) {
        RmtTcl1 *rmtTcl1 = new RmtTcl1();
        encodedIrData = rmtTcl1->encodeRemote(&remoteCommon);
    } else if (remoteCommon.remoteId == RmtChigo2::remoteId) {
        RmtChigo2 *rmtChigo2 = new RmtChigo2();
        encodedIrData = rmtChigo2->encodeRemote(&remoteCommon);
    } else if (remoteCommon.remoteId == RmtLG1::remoteId) {
        RmtLG1 *rmtLG1 = new RmtLG1();
        encodedIrData = rmtLG1->encodeRemote(&remoteCommon);
    } else if (remoteCommon.remoteId == RmtMits1::remoteId) {
        RmtMits1 *rmtMits1 = new RmtMits1();
        encodedIrData = rmtMits1->encodeRemote(&remoteCommon);
    } else if (remoteCommon.remoteId == RmtGreeAll::remoteId) {
        RmtGreeAll *rmtGreeAll = new RmtGreeAll();
        encodedIrData = rmtGreeAll->encodeRemote(&remoteCommon);
    } else {
        return createResponseJson(EN_CODE_REMOTE_NOT_FOUND, Common::getErrorString(EN_CODE_REMOTE_NOT_FOUND), "");
    }

    return createResponseJson(EN_CODE_SUCCESS, "", encodedIrData);
}

std::string  Encoder::createResponseJson(int32_t errorCode, std::string errorMessage, std::string encodedIrData) {
    json responseJson;
    responseJson["errorCode"] = errorCode;
    responseJson["errorMessage"] = errorMessage;
    responseJson["encodedIrData"] = encodedIrData;
    return responseJson.dump();
}