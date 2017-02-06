//
//  CppWrapper.m
//  PHPlayer
//
//  Created by huhexiang on 17/1/16.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#import "cppwrapper.h"
#include "PHPlayerCore.hpp"

@implementation CPPWrapper

-(id) init
{
    self = [super init];
    if (self) {
        _player = new PHPlayerCore();
    }
    return self;
};

-(int) add: (int) a and: (int) b
{
    int ret = static_cast<PHPlayerCore*>(_player)->add(a, b);
    return ret;
}

-(bool) open:(char *) file
{
    bool ret = static_cast<PHPlayerCore*>(_player)->open(file);
    return ret;
}

-(void) dealloc
{
    delete static_cast<PHPlayerCore*>(_player);
}

@end
