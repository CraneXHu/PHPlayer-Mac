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
//        static_cast<PHPlayerCore*>(_player)->init();
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

-(void) pause
{
    static_cast<PHPlayerCore*>(_player)->pause();
}

-(void) play
{
    static_cast<PHPlayerCore*>(_player)->play();
}

-(void) stop
{
    static_cast<PHPlayerCore*>(_player)->stop();
}

-(void) seek:(double) postion
{
    static_cast<PHPlayerCore*>(_player)->seek(postion, 0);
}

-(void) forward:(double)postion
{
    static_cast<PHPlayerCore*>(_player)->forward(postion);
}

-(void) backward:(double)postion
{
    static_cast<PHPlayerCore*>(_player)->backward(postion);
}

-(int) getState
{
    return static_cast<PHPlayerCore*>(_player)->getState();
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

-(double) getDuration
{
    return static_cast<PHPlayerCore*>(_player)->getDuration();
}

-(double) getCurrentTime
{
    return static_cast<PHPlayerCore*>(_player)->getCurrentTime();
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
