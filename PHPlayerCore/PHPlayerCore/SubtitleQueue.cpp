//
//  SubtitleQueue.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/3/14.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "SubtitleQueue.hpp"
SubtitleQueue::SubtitleQueue(int capacity)
{
    this->capacity = capacity;
}

bool SubtitleQueue::push(AVSubtitle* sub)
{
    std::unique_lock<std::mutex> lock(mutex);
    conditionFull.wait(lock, [this] {return queue.size() < capacity;});
    queue.push(sub);
    conditionEmpty.notify_one();
    return true;
    
}

AVSubtitle* SubtitleQueue::front()
{
    std::unique_lock<std::mutex> lock(mutex);
    conditionEmpty.wait(lock, [this]{return !queue.empty();});
    AVSubtitle *sub = queue.front();
    queue.pop();
    conditionFull.notify_one();
    return sub;
}

void SubtitleQueue::clear()
{
    std::unique_lock<std::mutex> lock(mutex);
    while (queue.size() > 0) {
        AVSubtitle *sub = queue.front();
        queue.pop();
        avsubtitle_free(sub);
        av_free(sub);
    }
    conditionFull.notify_one();
}
