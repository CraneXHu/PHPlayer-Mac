//
//  SubtitleQueue.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/3/14.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef SubtitleQueue_hpp
#define SubtitleQueue_hpp

#include <stdio.h>
#include <stdio.h>
#include <queue>
#include <thread>
extern "C" {
#include "avformat.h"
#include "avcodec.h"
}

class SubtitleQueue
{
public:
    SubtitleQueue(int capacity);
    bool push(AVSubtitle* sub);
    AVSubtitle* front();
    void clear();
    
private:
    std::queue<AVSubtitle*> queue;
    std::mutex mutex;
    std::condition_variable conditionEmpty;
    std::condition_variable conditionFull;
    int capacity;
};
#endif /* SubtitleQueue_hpp */
