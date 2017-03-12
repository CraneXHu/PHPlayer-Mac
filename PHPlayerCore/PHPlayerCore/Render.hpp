//
//  Render.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef Render_hpp
#define Render_hpp

#include <stdio.h>
#include "globaldef.h"
#include <thread>
#include <atomic>

class PHPlayerCore;

class Render{
public:
    Render(PHPlayerCore *player);
    ~Render();
    
    void start();
    void play();
    void setVideoCallback(void *userData, VideoCallback callback);
    void renderVideo();
    void renderAudio(unsigned char* outData, int *size);
    double getAudioClock();
    void setAudioClock(double clock);
    
private:
    PHPlayerCore *player;
    void *userData;
    VideoCallback videoCallback;
    std::atomic<double> audioClock;
    std::mutex mutex;
    std::condition_variable cv;
};
#endif /* Render_hpp */
