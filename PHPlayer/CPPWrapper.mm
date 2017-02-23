//
//  CppWrapper.m
//  PHPlayer
//
//  Created by huhexiang on 17/1/16.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#import "cppwrapper.h"
#include "PHPlayerCore.hpp"

@implementation PlayerWrapper

-(id) init
{
    self = [super init];
    if (self) {
        _player = new PHPlayerCore();
        static_cast<PHPlayerCore*>(_player)->init();
    }
    return self;
};

-(bool) open:(char *) file
{
    bool ret = static_cast<PHPlayerCore*>(_player)->open(file);
    return ret;
}

-(void) start
{
    static_cast<PHPlayerCore*>(_player)->start();
}

-(int) getVideoWidth
{
    return static_cast<PHPlayerCore*>(_player)->getVideoWidth();
}

-(int) getVideoHeight
{
    return static_cast<PHPlayerCore*>(_player)->getVideoHeight();
}

-(int) getAudioSampleRate
{
    return static_cast<PHPlayerCore*>(_player)->getAudioSampleRate();
}

-(int) getAudioChannels
{
    return static_cast<PHPlayerCore*>(_player)->getAudioChannels();
}

-(void) videoCallback:(VideoCallback)callback userData: (void*) data
{
    static_cast<PHPlayerCore*>(_player)->setVideoCallback(data, callback);
}

-(void) getAudioData:(unsigned char *)outData size:(int *)size
{
    static_cast<PHPlayerCore*>(_player)->getAudioData(outData, size);
}

-(int64_t) getDuration
{
    return static_cast<PHPlayerCore*>(_player)->getDuration();
}

-(char*) getFileName
{
    return static_cast<PHPlayerCore*>(_player)->getFileName();
}

-(void) dealloc
{
    delete static_cast<PHPlayerCore*>(_player);
}

@end
