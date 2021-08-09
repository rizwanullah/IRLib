//
//  ViewController.m
//  PELIRLib
//
//  Created by Curiologix on 27/02/2017.
//  Copyright © 2017 PEL. All rights reserved.
//

#import "ViewController.h"
#import "WrapperIOS.h"

@interface ViewController () {
    WrapperIOS *wrapper;
    
}

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    wrapper = [[WrapperIOS alloc] init];
    
    NSString *encodedIrJsonStr = @"";
    
    NSString *barcode = @"SRXARI";
    [self findRemoteTest:barcode];
    NSLog(@" ");
    
    encodedIrJsonStr = [self encodeTest:1000];
    [self decodeTest:1000 encodedIrJsonStr:encodedIrJsonStr];
    NSLog(@" ");
    
    encodedIrJsonStr = [self encodeTest:1001];
    [self decodeTest:1001 encodedIrJsonStr:encodedIrJsonStr];
    NSLog(@" ");
    
    encodedIrJsonStr = [self encodeTest:1002];
    [self decodeTest:1002 encodedIrJsonStr:encodedIrJsonStr];
    NSLog(@" ");
    
    encodedIrJsonStr = [self encodeTest:1003];
    [self decodeTest:1003 encodedIrJsonStr:encodedIrJsonStr];
    NSLog(@" ");
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}


- (void)findRemoteTest:(NSString *)barcode {
    NSInteger foundRemoteId = [wrapper findRemoteIdForAcModel:barcode];
    NSLog(@"remoteId: %ld", (long)foundRemoteId);
}

- (NSString *)encodeTest:(NSInteger)rmtId {
    
    NSDictionary *remoteIrDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                  [NSNumber numberWithInteger:rmtId], @"remoteId",
                                  @"ZH/JT-03", @"remoteModel",
                                  @"Chigo", @"remoteBrand",
                                  [NSNumber numberWithInt:1], @"powerOnOff",
                                  [NSNumber numberWithInt:1], @"operationMode",
                                  [NSNumber numberWithInt:24], @"setTemperature",
                                  [NSNumber numberWithInt:1], @"fanSpeed",
                                  [NSNumber numberWithInt:2], @"swingVertical",
                                  [NSNumber numberWithInt:0], @"swingHorizontal",
                                  [NSNumber numberWithInt:0], @"turboOnOff",
                                  [NSNumber numberWithInt:0], @"muteOnOff",
                                  [NSNumber numberWithInt:0], @"autoEvapOnOff",
                                  [NSNumber numberWithInt:1], @"displayOnOff",
                                  nil];
    
    NSError *error;
    NSData *remoteIrData = [NSJSONSerialization dataWithJSONObject:remoteIrDict options:NSJSONWritingPrettyPrinted error:&error];
    if (error) {
        NSLog(@"enc -> json error: %@", error.localizedDescription);
        return @"";
    }
    
    NSString *remoteIrJson = [[NSString alloc] initWithData:remoteIrData encoding:NSUTF8StringEncoding];
    NSString *encodedIrJsonStr = [wrapper encodeRemoteIrData:remoteIrJson];
    NSLog(@"encoded(%ld): %@", (long)rmtId, encodedIrJsonStr);
    return encodedIrJsonStr;
}

- (void)decodeTest:(NSInteger)rmtId encodedIrJsonStr:(NSString *)encodedIrJsonStr {
    
    NSError *error;
    NSData *encodedIrJsonData = [encodedIrJsonStr dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *encodedIrJson = [NSJSONSerialization JSONObjectWithData:encodedIrJsonData options:NSJSONReadingMutableContainers error:&error];
    if (error) {
        NSLog(@"dec -> json error: %@", error.localizedDescription);
        return;
    }
    
    NSString *encodedIrStr = [encodedIrJson objectForKey:@"encodedIrData"];
    
    NSDictionary *remoteIrDict = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInteger:rmtId], @"remoteId", encodedIrStr, @"encodedIrData", nil];
    
    NSData *remoteIrData = [NSJSONSerialization dataWithJSONObject:remoteIrDict options:NSJSONWritingPrettyPrinted error:&error];
    if (error) {
        NSLog(@"dec -> json error: %@", error.localizedDescription);
        return;
    }
    
    NSString *remoteIrJson = [[NSString alloc] initWithData:remoteIrData encoding:NSUTF8StringEncoding];
    NSString *decodedIrJsonStr = [wrapper decodeRemoteIrData:remoteIrJson];
    NSLog(@"decoded(%ld): %@", (long)rmtId, decodedIrJsonStr);
}

@end
