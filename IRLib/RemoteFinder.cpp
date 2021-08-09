//
// Created by Win7 on 12/17/2016.
//

#include "RemoteFinder.h"

#include "Remotes/RmtLG1.h"
#include "Remotes/RmtMits1.h"
#include "Remotes/RmtGreeAll.h"
#include "Remotes/RmtChigo1.h"
#include "Remotes/RmtChigo2.h"
#include "Remotes/RmtAux1.h"
#include "Remotes/RmtTcl1.h"

int32_t RemoteFinder::findRemoteId(std::string acModelStr) {

    RmtGreeAll *rmtGreeAll = new RmtGreeAll();
    if (rmtGreeAll->isAcModelSupported(acModelStr))
        return RmtGreeAll::remoteId;

    RmtMits1 *rmtMits1 = new RmtMits1();
    if (rmtMits1->isAcModelSupported(acModelStr))
        return RmtMits1::remoteId;

    RmtChigo1 *rmtChigo1 = new RmtChigo1();
    if (rmtChigo1->isAcModelSupported(acModelStr))
        return RmtChigo1::remoteId;

    RmtAux1 *rmtAux1 = new RmtAux1();
    if (rmtAux1->isAcModelSupported(acModelStr))
        return RmtAux1::remoteId;

    RmtTcl1 *rmtTcl1 = new RmtTcl1();
    if (rmtTcl1->isAcModelSupported(acModelStr))
        return RmtTcl1::remoteId;

    RmtChigo2 *rmtChigo2 = new RmtChigo2();
    if (rmtChigo2->isAcModelSupported(acModelStr))
        return RmtChigo2::remoteId;

    return RF_REMOTE_ID_NOT_FOUND;
}
