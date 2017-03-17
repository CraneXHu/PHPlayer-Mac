//
//  Decoder.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "spdlog/spdlog.h"
#include "Decoder.hpp"
#include "PHPlayerCore.hpp"
#include "PacketQueue.hpp"
#include "FrameQueue.hpp"
#include "Demuxer.hpp"

extern "C" {
#include "libavutil/pixdesc.h"
#include "VideoToolbox.h"
}

Decoder::Decoder(PHPlayerCore *player)
{
    this->player = player;
    
    codecContext = 0;
    frameQueue = new FrameQueue(16);
}

Decoder::~Decoder()
{
    delete frameQueue;
}

bool Decoder::open()
{
    return true;
}

void Decoder::start()
{
    
}

void Decoder::close()
{
    packetQueue->push(0);
    frameQueue->push(0);
    
//    if(hardwareAcceleration && type == PH_DECODER_VIDEO){
//        videotoolbox_uninit(codecContext);
//    }
//    avcodec_close(codecContext);
}

void Decoder::decode()
{

}

void Decoder::clear()
{
    frameQueue->clear();
}

void Decoder::flush()
{
    if(codecContext){
        avcodec_flush_buffers(codecContext);
    }
}

AVCodecContext *Decoder::getCodecContex()
{
    return codecContext;
}

FrameQueue *Decoder::getFrameQueue()
{
    return frameQueue;
}
