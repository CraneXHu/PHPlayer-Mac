//
//  SubtitleDecoder.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/3/14.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef SubtitleDecoder_hpp
#define SubtitleDecoder_hpp

#include <stdio.h>
#include "Decoder.hpp"
#include "SubtitleQueue.hpp"

class PHPlayerCore;
class SubtitleDecoder: public Decoder
{
public:
    SubtitleDecoder(PHPlayerCore *player);
    ~SubtitleDecoder();
    bool open();
    void start();
    void decode();
    void clear();
    void close();
    
    SubtitleQueue* getSubtitleQueue();
    
private:
    SubtitleQueue *subtitleQueue;
};
#endif /* SubtitleDecoder_hpp */
