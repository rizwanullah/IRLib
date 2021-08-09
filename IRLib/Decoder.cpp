#include "Decoder.h"
#include "json.hpp"
#include "Common.h"

using json = nlohmann::json;

Decoder::Decoder() {}

std::string Decoder::decode(std::string encodedIrJsonStr) {

    json encodedIrJson;
    REMOTE_COMMON remoteCommon;
    std::string encodedIrData;

    //memset(&remoteCommon, 0, sizeof(remoteCommon));

    try {
        encodedIrJson = json::parse(encodedIrJsonStr);
    } catch (std::exception &e) {
        Common::logIrLib("Exception: %s", e.what());
        return createResponseJson(CM_CODE_JSON_PARSE_ERROR, e.what(), "", "");
    }

    try {
        remoteCommon.remoteId = encodedIrJson["remoteId"];
    } catch (std::exception &e) {
        Common::logIrLib("Exception: %s", e.what());
        return createResponseJson(CM_CODE_JSON_NO_KEY_ERROR, e.what(), "", "");
    }

    try {
        encodedIrData = encodedIrJson["encodedIrData"];
    } catch (std::exception &e) {
        Common::logIrLib("Exception: %s", e.what());
        return createResponseJson(CM_CODE_JSON_NO_KEY_ERROR, e.what(), "", "");
    }

    int32_t decodeErrorCode = DE_CODE_REMOTE_NOT_FOUND;

    if (remoteCommon.remoteId == 0) { // find remote
        if (decodeErrorCode != DE_CODE_SUCCESS) {
            RmtChigo1 *rmtChigo1 = new RmtChigo1();
            decodeErrorCode = rmtChigo1->decodeRemote(encodedIrData, &remoteCommon);
            remoteCommon.remoteId = RmtChigo1::remoteId;
        }
        if (decodeErrorCode != DE_CODE_SUCCESS) {
            RmtAux1 *rmtAux1 = new RmtAux1();
            decodeErrorCode = rmtAux1->decodeRemote(encodedIrData, &remoteCommon);
            remoteCommon.remoteId = RmtAux1::remoteId;
        }
        if (decodeErrorCode != DE_CODE_SUCCESS) {
            RmtTcl1 *rmtTcl1 = new RmtTcl1();
            decodeErrorCode = rmtTcl1->decodeRemote(encodedIrData, &remoteCommon);
            remoteCommon.remoteId = RmtTcl1::remoteId;
        }
        if (decodeErrorCode != DE_CODE_SUCCESS) {
            RmtChigo2 *rmtChigo2 = new RmtChigo2();
            decodeErrorCode = rmtChigo2->decodeRemote(encodedIrData, &remoteCommon);
            remoteCommon.remoteId = RmtChigo2::remoteId;
        }
        if (decodeErrorCode != DE_CODE_SUCCESS) {
            RmtLG1 *rmtLG1 = new RmtLG1();
            decodeErrorCode = rmtLG1->decodeRemote(encodedIrData, &remoteCommon);
            remoteCommon.remoteId = RmtLG1::remoteId;
        }
        if (decodeErrorCode != DE_CODE_SUCCESS) {
            RmtMits1 *rmtMits1 = new RmtMits1();
            decodeErrorCode = rmtMits1->decodeRemote(encodedIrData, &remoteCommon);
            remoteCommon.remoteId = RmtMits1::remoteId;
        }
        if (decodeErrorCode != DE_CODE_SUCCESS) {
            RmtGreeAll *rmtGreeAll = new RmtGreeAll();
            decodeErrorCode = rmtGreeAll->decodeRemote(encodedIrData, &remoteCommon);
            remoteCommon.remoteId = RmtGreeAll::remoteId;
        }
    } else if (remoteCommon.remoteId == RmtChigo1::remoteId) {
        RmtChigo1 *rmtChigo1 = new RmtChigo1();
        decodeErrorCode = rmtChigo1->decodeRemote(encodedIrData, &remoteCommon);
    } else if (remoteCommon.remoteId == RmtAux1::remoteId) {
        RmtAux1 *rmtAux1 = new RmtAux1();
        decodeErrorCode = rmtAux1->decodeRemote(encodedIrData, &remoteCommon);
    } else if (remoteCommon.remoteId == RmtTcl1::remoteId) {
        RmtTcl1 *rmtTcl1 = new RmtTcl1();
        decodeErrorCode = rmtTcl1->decodeRemote(encodedIrData, &remoteCommon);
    } else if (remoteCommon.remoteId == RmtChigo2::remoteId) {
        RmtChigo2 *rmtChigo2 = new RmtChigo2();
        decodeErrorCode = rmtChigo2->decodeRemote(encodedIrData, &remoteCommon);
    } else if (remoteCommon.remoteId == RmtLG1::remoteId) {
        RmtLG1 *rmtLG1 = new RmtLG1();
        decodeErrorCode = rmtLG1->decodeRemote(encodedIrData, &remoteCommon);
    } else if (remoteCommon.remoteId == RmtMits1::remoteId) {
        RmtMits1 *rmtMits1 = new RmtMits1();
        decodeErrorCode = rmtMits1->decodeRemote(encodedIrData, &remoteCommon);
    } else if (remoteCommon.remoteId == RmtGreeAll::remoteId) {
        RmtGreeAll *rmtGreeAll = new RmtGreeAll();
        decodeErrorCode = rmtGreeAll->decodeRemote(encodedIrData, &remoteCommon);
    } else {
        return createResponseJson(DE_CODE_REMOTE_NOT_FOUND, Common::getErrorString(DE_CODE_REMOTE_NOT_FOUND), "", "");
    }

    json remoteJson;
    remoteJson["remoteId"] = remoteCommon.remoteId;
    remoteJson["remoteModel"] = remoteCommon.remoteModel;
    remoteJson["remoteBrand"] = remoteCommon.remoteBrand;
    remoteJson["powerOnOff"] = remoteCommon.powerOnOff;
    remoteJson["operationMode"] = remoteCommon.operationMode;
    remoteJson["setTemperature"] = remoteCommon.setTemperature;
    remoteJson["fanSpeed"] = remoteCommon.fanSpeed;
    remoteJson["swingVertical"] = remoteCommon.swingVertical;
    remoteJson["swingHorizontal"] = remoteCommon.swingHorizontal;
    remoteJson["turboOnOff"] = remoteCommon.turboOnOff;
    remoteJson["muteOnOff"] = remoteCommon.muteOnOff;
    remoteJson["autoEvapOnOff"] = remoteCommon.autoEvapOnOff;
    remoteJson["displayOnOff"] = remoteCommon.displayOnOff;

    if (decodeErrorCode == DE_CODE_SUCCESS) {
        json responseJson;
        responseJson["errorCode"] = DE_CODE_SUCCESS;
        responseJson["errorMessage"] = "";
        responseJson["decodedIrPacket"] = remoteCommon.decodedPacket;
        responseJson["decodedIrJson"] = remoteJson;
        return responseJson.dump();
    } else
        return createResponseJson(decodeErrorCode, Common::getErrorString(decodeErrorCode), "", "");
}

std::string Decoder::createResponseJson(int32_t errorCode, std::string errorMessage, std::string decodedIrPacket, std::string decodedIrJsonStr) {
    json responseJson;
    responseJson["errorCode"] = errorCode;
    responseJson["errorMessage"] = errorMessage;
    responseJson["decodedIrPacket"] = decodedIrPacket;
    responseJson["decodedIrJson"] = decodedIrJsonStr;
    return responseJson.dump();
}