//
//  FrameQueue.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/1/20.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef FrameQueue_hpp
#define FrameQueue_hpp

#include <stdio.h>
#include <queue>
#include <thread>
extern "C" {
#include "avformat.h"
#include "avcodec.h"
}

class FrameQueue
{
public:
    bool push(const AVFrame *pFrame);
    bool front(AVFrame **pFrame);
    int size();
    
private:
    std::queue<AVFrame*> queue;
    std::mutex mutex;
    std::condition_variable conditionEmpty;
    std::condition_variable conditionFull;
    const int MAX_SIZE = 8;
};
#endif /* FrameQueue_hpp */
