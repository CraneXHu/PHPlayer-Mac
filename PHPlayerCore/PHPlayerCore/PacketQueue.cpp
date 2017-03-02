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
    abort = false;
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
//            printf("Packet full %d\n", maxSize);
            conditionFull.wait(lock);
        }
    }
    
}

//need to optimize，not use av_packet_ref, use move.
bool PacketQueue::front(AVPacket *packet)
{
    std::unique_lock<std::mutex> lock(mutex);
    for (; ; ) {
        if (abort) {
            return false;
        }
        if (!queue.empty()) {
            int ret = av_packet_ref(packet, queue.front());
            if (ret < 0) {
                return false;
            }
            AVPacket *pkt = queue.front();
            queue.pop();
            av_packet_free(&pkt);
            conditionFull.notify_one();
            return true;
        } else{
//            conditionEmpty.wait(lock, [&]() -> bool {return queue.size() != 0;});
//            printf("Packet empty %d\n", maxSize);
            conditionEmpty.wait(lock);
        }
    }
}

void PacketQueue::setAbort(bool abort)
{
    this->abort = abort;
}

void PacketQueue::clear()
{
    std::unique_lock<std::mutex> lock(mutex);
    while (queue.size() > 0) {
        AVPacket *pkt = queue.front();
        queue.pop();
        av_packet_free(&pkt);
    }
    conditionFull.notify_all();
}

int PacketQueue::size()
{
    std::unique_lock<std::mutex> lock(mutex);
    return queue.size();
}
