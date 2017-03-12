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
    FrameQueue(int capacity);
    bool push(AVFrame *frame);
    AVFrame* front();
    void clear();
    
private:
    std::queue<AVFrame*> queue;
    std::mutex mutex;
    std::condition_variable conditionEmpty;
    std::condition_variable conditionFull;
    int capacity;
};
#endif /* FrameQueue_hpp */
