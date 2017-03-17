//
//  Renderer.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdio.h>
#include "globaldef.h"
#include <thread>
#include <atomic>

struct AVSubtitle;
struct AVFrame;
class PHPlayerCore;

class Renderer{
public:
    Renderer(PHPlayerCore *player);
    ~Renderer();
    
    void start();
    void play();
    void setVideoCallback(void *userData, VideoCallback callback);
    void renderVideo();
    void renderAudio(unsigned char* outData, int *size);
    double getAudioClock();
    void setAudioClock(double clock);
    void subOverlay(AVSubtitle *sub, AVFrame *frame);
    
private:
    PHPlayerCore *player;
    void *userData;
    VideoCallback videoCallback;
    std::atomic<double> audioClock;
    std::mutex mutex;
    std::condition_variable cv;
};
#endif /* Renderer_hpp */
