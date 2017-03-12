//
//  PacketQueue.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/1/20.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "PacketQueue.hpp"

PacketQueue::PacketQueue(int capacity)
{
    this->capacity = capacity;
}

bool PacketQueue::push(AVPacket *packet)
{
//	AVPacket *newPacket = av_packet_alloc();
//    int ret = av_packet_ref(newPacket, packet);
//    if (ret < 0) {
//        av_packet_free(&newPacket);
//        return false;
//    }
    
    std::unique_lock<std::mutex> lock(mutex);
    conditionFull.wait(lock, [this] {return queue.size() < capacity;});
    queue.push(packet);
    conditionEmpty.notify_one();
    return true;
}

//need to optimize，not use av_packet_ref, use move.
AVPacket * PacketQueue::front()
{
    std::unique_lock<std::mutex> lock(mutex);
    conditionEmpty.wait(lock, [this]{return !queue.empty();});
    AVPacket *packet = queue.front();
    queue.pop();
    conditionFull.notify_one();
    return packet;
}

void PacketQueue::clear()
{
    std::unique_lock<std::mutex> lock(mutex);
    while (queue.size() > 0) {
        AVPacket *packet = queue.front();
        queue.pop();
        av_packet_free(&packet);
    }
    conditionFull.notify_one();
}
