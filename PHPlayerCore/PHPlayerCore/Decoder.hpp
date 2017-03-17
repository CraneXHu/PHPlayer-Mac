//
//  Decoder.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef Decoder_hpp
#define Decoder_hpp

#include <stdio.h>
#include <atomic>
#include "globalenum.h"

struct AVCodecContext;
class Demuxer;
class PHPlayerCore;
class PacketQueue;
class FrameQueue;

class Decoder{
public:
    Decoder(PHPlayerCore *player);
    ~Decoder();

    virtual bool open();
    virtual void start();
    void clear();
    void flush();
    virtual void decode();
    void close();
    
    AVCodecContext *getCodecContex();
    FrameQueue *getFrameQueue();
    
protected:
    PHPlayerCore *player;
    AVCodecContext *codecContext;
    PacketQueue *packetQueue;
    FrameQueue *frameQueue;
};
#endif /* Decoder_hpp */
