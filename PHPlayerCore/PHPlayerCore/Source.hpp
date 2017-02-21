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
    void init();
    bool open(char * file);
    AVFormatContext *getContext();
    
private:
    AVFormatContext *formatContext;
};
#endif /* Source_hpp */
