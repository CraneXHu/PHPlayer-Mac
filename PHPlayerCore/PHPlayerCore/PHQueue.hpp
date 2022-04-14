//
//  PHQueue.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 2022/4/14.
//  Copyright © 2022 huhexiang. All rights reserved.
//

#ifndef PHQueue_hpp
#define PHQueue_hpp

#include <stdio.h>
#include <stdio.h>
#include <queue>
#include <thread>
extern "C" {
#include "avformat.h"
#include "avcodec.h"
}

template <typename T>
class PHQueue
{
public:
    PHQueue(int capacity);
    bool push(T* data);
    T* front();
    void clear();
    
private:
    std::queue<T*> queue;
    std::mutex mutex;
    std::condition_variable conditionEmpty;
    std::condition_variable conditionFull;
    int capacity;
};

#endif /* PHQueue_hpp */
