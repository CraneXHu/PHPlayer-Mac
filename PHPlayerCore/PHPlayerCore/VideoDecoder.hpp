//
//  VideoDecoder.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/3/14.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef VideoDecoder_hpp
#define VideoDecoder_hpp

#include <stdio.h>
#include "Decoder.hpp"

class PHPlayerCore;

class VideoDecoder: public Decoder
{
public:
    VideoDecoder(PHPlayerCore *player);
    bool open();
    void start();
    void decode();
    void setEnableHardwareAcceleration(bool isEnable);
    bool isEnableHardwareAcceleration();
    
private:
    std::atomic<bool> hardwareAcceleration;
};
#endif /* VideoDecoder_hpp */
