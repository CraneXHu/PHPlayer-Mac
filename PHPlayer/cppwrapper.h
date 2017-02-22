//
//  cppwrapper.h
//  PHPlayer
//
//  Created by huhexiang on 17/1/16.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PlayerWrapper : NSObject
{
    void* _player;
}

-(bool) open: (char*) file;

-(void) start;

typedef void (*VideoCallback)(void *userData, unsigned char *data, int width, int height, int *linesize);

-(void) videoCallback:(VideoCallback) callback userData: (void *) data;

-(void) getAudioData: (unsigned char *) outData size: (int) size;

-(int) getVideoWidth;
-(int) getVideoHeight;

-(int) getAudioSampleRate;
-(int) getAudioChannels;

@end
