//
//  WrapperIOS.h
//  CppTest
//
//  Created by Curiologix on 15/02/2017.
//  Copyright © 2017 Seagate. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface WrapperIOS : NSObject

- (NSInteger)findRemoteIdForAcModel:(NSString *)acModelStr;
- (NSString *)encodeRemoteIrData:(NSString *)remoteJsonStr;
- (NSString *)decodeRemoteIrData:(NSString *)encodedIrJsonStr;

@end
