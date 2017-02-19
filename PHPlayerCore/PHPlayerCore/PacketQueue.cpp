//
//  PacketQueue.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/1/20.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "PacketQueue.hpp"

PacketQueue::PacketQueue(int maxSize)
{
    this->maxSize = maxSize;
}

bool PacketQueue::push(const AVPacket *packet)
{
	AVPacket *pkt = av_packet_alloc();
    int ret = av_packet_ref(pkt, packet);
    if (ret < 0) {
//        av_packet_free(&pkt);
        return false;
    }
    
    std::unique_lock<std::mutex> lock(mutex);
    for (; ; ) {
        if (queue.size() <= maxSize) {
            queue.push(pkt);
            conditionEmpty.notify_one();
            return true;
        } else {
//            conditionFull.wait(lock, [&]() -> bool {return queue.size() < MAX_SIZE;});
            conditionFull.wait(lock);
        }
    }
    
}

bool PacketQueue::front(AVPacket *packet)
{
    std::unique_lock<std::mutex> lock(mutex);
    for (; ; ) {
        if (!queue.empty()) {
            int ret = av_packet_ref(packet, queue.front());
            if (ret < 0) {
                return false;
            }
            AVPacket *pkt = queue.front();
            queue.pop();
            conditionFull.notify_one();
            av_packet_free(&pkt);
            return true;
        } else{
//            conditionEmpty.wait(lock, [&]() -> bool {return queue.size() != 0;});
            conditionEmpty.wait(lock);
        }
    }
}

int PacketQueue::size()
{
    std::unique_lock<std::mutex> lock(mutex);
    return queue.size();
}
