//
//  AudioDecoder.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/3/14.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef AudioDecoder_hpp
#define AudioDecoder_hpp

#include <stdio.h>
#include "Decoder.hpp"

class PHPlayerCore;
class AudioDecoder: public Decoder
{
public:
    AudioDecoder(PHPlayerCore *player);
    bool open();
    void start();
    void decode();
};
#endif /* AudioDecoder_hpp */
