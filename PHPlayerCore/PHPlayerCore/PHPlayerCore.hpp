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
class Renderer;
class SubtitleRenderer;
class AudioRenderer;

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
    void setEnableHardwareAcceleration(bool isEnable);
    bool isEnableHardwareAcceleration();
    void setVolume(float volume);
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
    
    Demuxer *getDemuxer();
    Renderer *getRender();
    SubtitleRenderer *getSubRender();
    AudioRenderer *getAudioRender();
    
private:
    std::atomic<PlayerState> state;
    Demuxer *demuxer;
    Renderer *renderer;
    SubtitleRenderer *subRenderer;
    AudioRenderer *audioRenderer;
};

#pragma GCC visibility pop
#endif
