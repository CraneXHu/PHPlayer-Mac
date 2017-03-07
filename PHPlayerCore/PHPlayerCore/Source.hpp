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
class PHPlayerCore;

class Source {
public:
    Source(PHPlayerCore *player);
    ~Source();
    static void init();
    bool open(char * file);
    void close();
    double getDuration();
    char *getFileName();
    AVFormatContext *getContext();
    
private:
    AVFormatContext *formatContext;
    PHPlayerCore *player;
};
#endif /* Source_hpp */
