//
//  WrapperIOS.m
//  CppTest
//
//  Created by Curiologix on 15/02/2017.
//  Copyright © 2017 Seagate. All rights reserved.
//

#import "WrapperIOS.h"
#include "RemoteFinder.h"
#include "Encoder.h"
#include "Decoder.h"

@implementation WrapperIOS

- (NSInteger)findRemoteIdForAcModel:(NSString *)acModelStr {
    RemoteFinder *remoteFinder = new RemoteFinder();
    int32_t remoteId = remoteFinder->findRemoteId((std::string([acModelStr UTF8String])));
    return remoteId;
}

- (NSString *)encodeRemoteIrData:(NSString *)remoteJsonStr {
    Encoder *encoder = new Encoder();
    std::string encodedIrData = encoder->encode(std::string([remoteJsonStr UTF8String]));
    return [NSString stringWithCString:encodedIrData.c_str() encoding:NSUTF8StringEncoding];
}

- (NSString *)decodeRemoteIrData:(NSString *)encodedIrJsonStr {
    Decoder *decoder = new Decoder();
    std::string decodedIrJson = decoder->decode(std::string([encodedIrJsonStr UTF8String]));
    return [NSString stringWithCString:decodedIrJson.c_str() encoding:NSUTF8StringEncoding];
}

@end
