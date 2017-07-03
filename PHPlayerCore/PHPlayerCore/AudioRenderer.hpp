//
//  AudioRenderer.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/3/16.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef AudioRenderer_hpp
#define AudioRenderer_hpp

#include <stdio.h>
#include <AudioToolbox/AudioQueue.h>

class PHPlayerCore;
class AudioRenderer{
public:
    AudioRenderer(PHPlayerCore*player);
    ~AudioRenderer();
    void init();
    void play();
    void pause();
    void flush();
    void stop();
    
    void setPlaybackRate(float rate);
    void setPlaybackVolume(float volume);
    
private:
    PHPlayerCore *player;
    AudioQueueRef audioQueueRef;
    AudioQueueBufferRef *audioQueueBufferRefs;
    int bufferSize;
};
#endif /* AudioRenderer_hpp */
