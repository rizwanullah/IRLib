#include <iostream>
#include <string>

#include <string.h>
#include "utils/base64.h"
#include "../../IRLib/RemoteFinder.h"
#include "../../IRLib/Encoder.h"
#include "../../IRLib/Decoder.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3) {
        std::cout << "Please pass at least 2 arguments." << endl;
        return 0;
    }

    std::string optValB64(argv[2]);
    std::string optVal = base64_decode(optValB64);

    if (strcmp(argv[1], "-fri") == 0) { // Find Remote Id
        RemoteFinder *remoteFinder = new RemoteFinder();
        int rmtId = remoteFinder->findRemoteId(optVal);
        std::string rmtIdStrB64 = base64_encode(std::to_string(rmtId));
        std::cout << rmtIdStrB64 << endl;
    } else if (strcmp(argv[1], "-enc") == 0) { // Encode
        Encoder *encoder = new Encoder();
        std::string encIrDataStr = encoder->encode(optVal);
        std::string encIrDataStrB64 = base64_encode(encIrDataStr);
        std::cout << encIrDataStrB64 << endl;
    } else if (strcmp(argv[1], "-dec") == 0) { // Decode
        Decoder *decoder = new Decoder();
        std::string decJsonStr = decoder->decode(optVal);
        std::string decJsonStrB64 = base64_encode(decJsonStr);
        std::cout << decJsonStrB64 << endl;
    } else { // unknown
        std::cout << "Please pass correct argument values." << endl;
        return 0;
    }

    return 0;
}
