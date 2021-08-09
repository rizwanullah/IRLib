#ifndef IRLIB_DECODER_H
#define IRLIB_DECODER_H

#include <string>
#include "Common.h"

#include "Remotes/RmtChigo1.h"
#include "Remotes/RmtChigo2.h"
#include "Remotes/RmtAux1.h"
#include "Remotes/RmtLG1.h"
#include "Remotes/RmtTcl1.h"
#include "Remotes/RmtMits1.h"
#include "Remotes/RmtGreeAll.h"

class Decoder {

public:
    Decoder();
    std::string decode(std::string encodedIrJsonStr);
    std::string createResponseJson(int32_t errorCode, std::string errorMessage, std::string decodedIrPacket, std::string decodedIrJsonStr);

};


#endif //IRLIB_DECODER_H
