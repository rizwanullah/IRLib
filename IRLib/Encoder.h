#ifndef IRLIB_ENCODER_H
#define IRLIB_ENCODER_H

#include <string>
#include "Common.h"

#include "Remotes/RmtChigo1.h"
#include "Remotes/RmtChigo2.h"
#include "Remotes/RmtAux1.h"
#include "Remotes/RmtLG1.h"
#include "Remotes/RmtTcl1.h"
#include "Remotes/RmtMits1.h"
#include "Remotes/RmtGreeAll.h"

class Encoder {

public:
    Encoder();
    std::string encode(std::string remoteJson);
    std::string createResponseJson(int32_t errorCode, std::string errorMessage, std::string encodedIrData);
};

#endif //IRLIB_ENCODER_H
