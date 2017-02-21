/*
 *  PHPlayerCore.hpp
 *  PHPlayerCore
 *
 *  Created by huhexiang on 17/1/16.
 *  Copyright © 2017年 huhexiang. All rights reserved.
 *
 */

#ifndef PHPlayerCore_
#define PHPlayerCore_

/* The classes below are exported */
#pragma GCC visibility push(default)

#include "globalenum.h"

class Source;
class Demuxer;
class Decoder;

class PHPlayerCore
{
public:
    PHPlayerCore();
    ~PHPlayerCore();
    void init();
    void setState(PlayerState state);
    PlayerState getState();
    bool open(char* file);
    void start();
    void seek(int64_t postion);
    
public:
    Source *getSource();
    Demuxer *getDemuxer();
    Decoder *getVideoDecoder();
    Decoder *getAudioDecoder();
    Decoder *getSubtitleDecoder();
    
private:
    PlayerState state;
    Source *source;
    Demuxer *demuxer;
    Decoder *videoDecoder;
    Decoder *audioDecoder;
    Decoder *subtitleDecoder;
};

#pragma GCC visibility pop
#endif
