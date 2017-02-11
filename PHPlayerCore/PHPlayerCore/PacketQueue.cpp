//
//  PacketQueue.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/1/20.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "PacketQueue.hpp"

bool PacketQueue::push(const AVPacket *packet)
{
	AVPacket *pkt = av_packet_alloc();
    int ret = av_packet_ref(pkt, packet);
    if (ret < 0) {
        return false;
    }
    
    mutex.lock();
    queue.push(*pkt);
    mutex.unlock();
    
    return true;
}

bool PacketQueue::front(AVPacket *packet)
{
    bool ret = true;
    mutex.lock();
    if (!queue.empty()) {
        int res = av_packet_ref(packet, &queue.front());
        if (res < 0) {
            return false;
        }
        
        AVPacket pkt = queue.front();
        queue.pop();
        av_packet_unref(&pkt);
        
    } else{
        ret = false;
    }
    mutex.unlock();
    return ret;
}

int PacketQueue::size()
{
    int size = queue.size();
    return size;
}
