//
//  FrameQueue.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/1/20.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "FrameQueue.hpp"

bool FrameQueue::push(const AVFrame &frame)
{
    AVFrame distFrame;
    av_frame_ref(&distFrame, &frame);
    
    mutex.lock();
    queue.push(distFrame);
    mutex.unlock();
    
    return true;
}

bool FrameQueue::front(AVFrame &frame)
{
    bool ret = true;
    mutex.lock();
    if (!queue.empty()) {
        int res = av_frame_ref(&frame, &queue.front());
        if (res < 0) {
            ret = false;
        }
        
        AVFrame temp = queue.front();
        queue.pop();
        av_frame_unref(&temp);
        
    } else{
        ret = false;
    }
    mutex.unlock();
    return ret;
}
