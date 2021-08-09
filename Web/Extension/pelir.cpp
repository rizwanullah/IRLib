#include <iostream>
#include <phpcpp.h>

#include "../../IRLib/RemoteFinder.h"
#include "../../IRLib/Encoder.h"
#include "../../IRLib/Decoder.h"

using namespace std;

Php::Value testMethod(Php::Parameters &params) {
    cout << "testmethod() being called!" << endl;
    cout << "Param 1 -> " << params[0] << endl;

    RemoteFinder *remoteFinder = new RemoteFinder();
    int rmtId = remoteFinder->findRemoteId("SQTIMH");
    std::cout << "rmtId: " << rmtId << endl;

    Php::Value ret;
    ret = "ReturnValue";
    return ret;
}

Php::Value encoderTest(Php::Parameters &params) {
    cout << "encoderTest() being called!" << endl;
    
    Encoder *encoder = new Encoder();
    std::string encIrDataStr = encoder->encode(params[0]);
    //std::cout << "encIrDataStr: " << encIrDataStr << endl;

    Php::Value ret;
    ret = encIrDataStr;
    return ret;
}

Php::Value decoderTest(Php::Parameters &params) {
    cout << "decoderTest() being called!" << endl;

    Decoder *decoder = new Decoder();
    std::string decJsonStr = decoder->decode(params[0]);
    //std::cout << "decJsonStr: " << decJsonStr << endl;

    Php::Value ret;
    ret = decJsonStr;
    return ret;
}

extern "C" 
{
    PHPCPP_EXPORT void *get_module()
    {
        static Php::Extension extension("pelir","1.0");

        extension.add<testMethod>("testMethod", {Php::ByVal("string", Php::Type::String)});
        extension.add<encoderTest>("encoderTest", {Php::ByVal("string", Php::Type::String)});
        extension.add<decoderTest>("decoderTest", {Php::ByVal("string", Php::Type::String)});
        
        // return the extension module
        return extension.module();
    }
}
