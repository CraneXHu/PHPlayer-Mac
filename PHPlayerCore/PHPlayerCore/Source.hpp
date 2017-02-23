//
//  Source.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef Source_hpp
#define Source_hpp

#include <stdio.h>

struct AVFormatContext;

class Source {
public:
    Source();
    ~Source();
    static void init();
    bool open(char * file);
    __int64_t getDuration();
    char *getFileName();
    AVFormatContext *getContext();
    
private:
    AVFormatContext *formatContext;
};
#endif /* Source_hpp */
