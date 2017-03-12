//
//  FrameQueue.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/1/20.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "FrameQueue.hpp"

FrameQueue::FrameQueue(int capacity)
{
    this->capacity = capacity;
}

bool FrameQueue::push(AVFrame *frame)
{
//    AVFrame *newFrame = av_frame_alloc();
//    int ret = av_frame_ref(newFrame, frame);
//    if (ret < 0) {
//        av_free(newFrame);
//        return false;
//    }
    
    std::unique_lock<std::mutex> lock(mutex);
    conditionFull.wait(lock, [this] {return queue.size() < capacity;});
    queue.push(frame);
    conditionEmpty.notify_one();
    return true;

}

AVFrame * FrameQueue::front()
{
    std::unique_lock<std::mutex> lock(mutex);
    conditionEmpty.wait(lock, [this]{return !queue.empty();});
    AVFrame*frame = queue.front();
    queue.pop();
    conditionFull.notify_one();
    return frame;
}

void FrameQueue::clear()
{
    std::unique_lock<std::mutex> lock(mutex);
    while (queue.size() > 0) {
        AVFrame *temp = queue.front();
        queue.pop();
        av_frame_free(&temp);
    }
    conditionFull.notify_one();
}
