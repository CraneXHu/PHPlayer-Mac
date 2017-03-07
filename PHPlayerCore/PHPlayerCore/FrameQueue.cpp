//
//  FrameQueue.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/1/20.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "FrameQueue.hpp"

FrameQueue::FrameQueue(int maxSize)
{
    this->maxSize = maxSize;
    this->abort = false;
}

void FrameQueue::setAbort(bool abort)
{
    this->abort = abort;
}

bool FrameQueue::push(const AVFrame *pFrame)
{
    AVFrame *pDistFrame = av_frame_alloc();
    int ret = av_frame_ref(pDistFrame, pFrame);
    if (ret < 0) {
        av_free(pDistFrame);
        return false;
    }
    
    std::unique_lock<std::mutex> lock(mutex);
    for (; ; ) {
        if (queue.size() <= maxSize) {
            queue.push(pDistFrame);
            conditionEmpty.notify_one();
            return true;
        } else {
//            conditionFull.wait(lock, [&]() -> bool {return queue.size() < MAX_SIZE;});
            conditionFull.wait(lock);
        }
    }

}

bool FrameQueue::front(AVFrame **pFrame)
{
    std::unique_lock<std::mutex> lock(mutex);
    for (; ; ) {
        if (abort) {
            return false;
        }
        if (!queue.empty()) {
            int ret = av_frame_ref(*pFrame, queue.front());
            if (ret < 0) {
                return false;
            }
            
            AVFrame *temp = queue.front();
            queue.pop();
            av_frame_free(&temp);
            conditionFull.notify_one();
            return true;
        } else{
            //        conditionEmpty.wait(lock, [&]() -> bool {return queue.size() != 0;});
            conditionEmpty.wait(lock);
        }
    }
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

int FrameQueue::size()
{
    std::unique_lock<std::mutex> lock(mutex);
    return queue.size();
}
