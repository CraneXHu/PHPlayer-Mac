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

class PHPlayerCore;

class Render{
public:
    Render(PHPlayerCore *player);
    ~Render();
    
    void start();
    void pause();
    void stop();
    void setVideoCallback(void *userData, VideoCallback callback);
    void renderVideo();
    void renderAudio(unsigned char* outData, int *size);
    
private:
    PHPlayerCore *player;
    void *userData;
    VideoCallback videoCallback;
    double audioClock;
};
#endif /* Render_hpp */
