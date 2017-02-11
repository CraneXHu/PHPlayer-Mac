//
//  PacketQueue.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/1/20.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef PacketQueue_hpp
#define PacketQueue_hpp

#include <stdio.h>
#include <queue>
#include <thread>
extern "C" {
#include "avformat.h"
#include "avcodec.h"
}

class PacketQueue
{
public:
    bool push(const AVPacket *packet);
    bool front(AVPacket *packet);
    int size();
    
private:
    std::queue<AVPacket> queue;
    std::mutex mutex;
};
#endif /* PacketQueue_hpp */
