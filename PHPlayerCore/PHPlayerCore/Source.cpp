//
//  Source.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "Source.hpp"
extern "C"{
#include "avformat.h"
}

Source::Source()
{
    formatContext = 0;
}

Source::~Source()
{
    avformat_close_input(&formatContext);
}

void Source::init(){
    av_register_all();
    avformat_network_init();
}

bool Source::open(char *file)
{
    if (avformat_open_input(&formatContext, file, NULL, NULL) < 0) {
        return false;
    }
    
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        return false;
    }
    return true;
}

AVFormatContext* Source::getContext(){
    return formatContext;
}
