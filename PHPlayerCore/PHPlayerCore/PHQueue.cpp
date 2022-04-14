//
//  PHQueue.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 2022/4/14.
//  Copyright © 2022 huhexiang. All rights reserved.
//

#include "PHQueue.hpp"
PHQueue::PHQueue(int capacity)
{
    this->capacity = capacity;
}

bool PHQueue::push(T* data)
{
    std::unique_lock<std::mutex> lock(mutex);
    conditionFull.wait(lock, [this] {return queue.size() < capacity;});
    queue.push(data);
    conditionEmpty.notify_one();
    return true;
    
}

T* PHQueue::front()
{
    std::unique_lock<std::mutex> lock(mutex);
    conditionEmpty.wait(lock, [this]{return !queue.empty();});
    T *data = queue.front();
    queue.pop();
    conditionFull.notify_one();
    return data;
}

void PHQueue::clear()
{
    std::unique_lock<std::mutex> lock(mutex);
    while (queue.size() > 0) {
        T *data = queue.front();
        queue.pop();
        if(typeid(T) == typeid(PacketQueue)){
            av_packet_free(&data);
        } else if(typeid(T) == typeid(FrameQueue)){
            av_frame_free(&data);
        } else {
            avsubtitle_free(data);
            av_free(data);
        }
    }
    conditionFull.notify_one();
}
