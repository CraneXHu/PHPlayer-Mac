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
#include "globaldef.h"
#include <atomic>

class Demuxer;
class Decoder;
class Render;

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
    void pause();
    void play();
    void stop();
    void seek(double postion, int flag);
    void forward(double duration);
    void backward(double duration);
//    void clear();
    
    void setVideoCallback(void *userData, VideoCallback callback);
    void getAudioData(unsigned char* outData, int* size);
    
    int getVideoWidth();
    int getVideoHeight();
    
    int getAudioSampleRate();
    int getAudioChannels();
    
    char * getFileName();
    double getDuration();
    double getCurrentTime();
    
public:
    Demuxer *getDemuxer();
    Render *getRender();
    
private:
    std::atomic<PlayerState> state;
    Demuxer *demuxer;
    Render *render;
};

#pragma GCC visibility pop
#endif
