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

@end
