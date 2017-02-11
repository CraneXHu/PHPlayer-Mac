//
//  FrameQueue.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/1/20.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "FrameQueue.hpp"

bool FrameQueue::push(const AVFrame *pFrame)
{
    AVFrame *pDistFrame = av_frame_alloc();
    int ret = av_frame_ref(pDistFrame, pFrame);
    if (ret < 0) {
        return false;
    }
    
    mutex.lock();
    queue.push(pDistFrame);
    mutex.unlock();
    
    return true;
}

bool FrameQueue::front(AVFrame **pFrame)
{
    bool ret = true;
    mutex.lock();
    if (!queue.empty()) {
        int res = av_frame_ref(*pFrame, queue.front());
        if (res < 0) {
            return false;
        }
        
        AVFrame *temp = queue.front();
        queue.pop();
        av_frame_unref(temp);
        
    } else{
        ret = false;
    }
    mutex.unlock();
    return ret;
}

int FrameQueue::size()
{
    int size = queue.size();
    return size;
}
