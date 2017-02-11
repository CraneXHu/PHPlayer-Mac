//
//  cppwrapper.h
//  PHPlayer
//
//  Created by huhexiang on 17/1/16.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CPPWrapper : NSObject
{
    void* _player;
}

-(int) add: (int) a and: (int) b;

-(bool) open: (char*) file;

-(void) start;

typedef void (*Callback)(void *cxt, unsigned char *data, int width, int height, int *linesize);

-(void) setCallback:(Callback) callback context: (void *) ctx;

@end
