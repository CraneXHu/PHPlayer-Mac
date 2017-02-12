//
//  Audio.swift
//  PHPlayer
//
//  Created by huhexiang on 17/2/12.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

import Foundation
import AVFoundation
import AudioToolbox

class AudioController : NSObject {
    
    let audioQueueNumberBuffers = 3
    
    func initWithAudioSpec(spec : UnsafeMutableRawPointer) {
        
        AudioStreamBasicDescription streamDescription;
        streamDescription->mSampleRate = spec->freq;
        streamDescription->mFormatID = kAudioFormatLinearPCM;
        streamDescription->mFormatFlags = kLinearPCMFormatFlagIsPacked;
        streamDescription->mChannelsPerFrame = spec->channels;
        streamDescription->mFramesPerPacket = 1;
        
//        streamDescription->mBitsPerChannel = SDL_AUDIO_BITSIZE(spec->format);
//        if (SDL_AUDIO_ISBIGENDIAN(spec->format))
//        streamDescription->mFormatFlags |= kLinearPCMFormatFlagIsBigEndian;
//        if (SDL_AUDIO_ISFLOAT(spec->format))
//        streamDescription->mFormatFlags |= kLinearPCMFormatFlagIsFloat;
//        if (SDL_AUDIO_ISSIGNED(spec->format))
//        streamDescription->mFormatFlags |= kLinearPCMFormatFlagIsSignedInteger;
        
        streamDescription->mBytesPerFrame = desc->mBitsPerChannel * desc->mChannelsPerFrame / 8;
        streamDescription->mBytesPerPacket = desc->mBytesPerFrame * desc->mFramesPerPacket;
        
        /* Set the desired format */
        AudioQueueRef audioQueueRef;
        OSStatus status = AudioQueueNewOutput(&streamDescription,
                                              IJKSDLAudioQueueOuptutCallback,
                                              (__bridge void *) self,
                                              NULL,
                                              kCFRunLoopCommonModes,
                                              0,
                                              &audioQueueRef);
        if (status != noErr) {
            return;
        }
        
        UInt32 propValue = 1;
        AudioQueueSetProperty(audioQueueRef, kAudioQueueProperty_EnableTimePitch, &propValue, sizeof(propValue));
        propValue = 1;
        AudioQueueSetProperty(_audioQueueRef, kAudioQueueProperty_TimePitchBypass, &propValue, sizeof(propValue));
        propValue = kAudioQueueTimePitchAlgorithm_Spectral;
        AudioQueueSetProperty(_audioQueueRef, kAudioQueueProperty_TimePitchAlgorithm, &propValue, sizeof(propValue));
        
        status = AudioQueueStart(audioQueueRef, NULL);
        if (status != noErr) {
            NSLog(@"AudioQueue: AudioQueueStart failed (%d)\n", (int)status);
            self = nil;
            return nil;
        }
        
        SDL_CalculateAudioSpec(&_spec);
        
        _audioQueueRef = audioQueueRef;
        
        for (int i = 0;i < kIJKAudioQueueNumberBuffers; i++)
        {
            AudioQueueAllocateBuffer(audioQueueRef, _spec.size, &_audioQueueBufferRefArray[i]);
            _audioQueueBufferRefArray[i]->mAudioDataByteSize = _spec.size;
            memset(_audioQueueBufferRefArray[i]->mAudioData, 0, _spec.size);
            AudioQueueEnqueueBuffer(audioQueueRef, _audioQueueBufferRefArray[i], 0, NULL);
        }
    }
    
    func play() {
        <#function body#>
    }
    {
    if (!_audioQueueRef)
    return;
    
    @synchronized(_lock) {
    _isPaused = NO;
    NSError *error = nil;
    if (NO == [[AVAudioSession sharedInstance] setActive:YES error:&error]) {
    NSLog(@"AudioQueue: AVAudioSession.setActive(YES) failed: %@\n", error ? [error localizedDescription] : @"nil");
    }
    
    OSStatus status = AudioQueueStart(_audioQueueRef, NULL);
    if (status != noErr)
    NSLog(@"AudioQueue: AudioQueueStart failed (%d)\n", (int)status);
    }
    }
    
    - (void)pause
    {
    if (!_audioQueueRef)
    return;
    
    @synchronized(_lock) {
    if (_isStopped)
    return;
    
    _isPaused = YES;
    OSStatus status = AudioQueuePause(_audioQueueRef);
    if (status != noErr)
    NSLog(@"AudioQueue: AudioQueuePause failed (%d)\n", (int)status);
    }
    }
    
    - (void)flush
    {
    if (!_audioQueueRef)
    return;
    
    @synchronized(_lock) {
    if (_isStopped)
    return;
    
    AudioQueueFlush(_audioQueueRef);
    }
    }
    
    - (void)stop
    {
    if (!_audioQueueRef)
    return;
    
    @synchronized(_lock) {
    if (_isStopped)
    return;
    
    _isStopped = YES;
    }
    
    // do not lock AudioQueueStop, or may be run into deadlock
    AudioQueueStop(_audioQueueRef, true);
    AudioQueueDispose(_audioQueueRef, true);
    }
    
    - (void)close
    {
    [self stop];
    _audioQueueRef = nil;
    }
    
    - (void)setPlaybackRate:(float)playbackRate
    {
    if (fabsf(playbackRate - 1.0f) <= 0.000001) {
    UInt32 propValue = 1;
    AudioQueueSetProperty(_audioQueueRef, kAudioQueueProperty_TimePitchBypass, &propValue, sizeof(propValue));
    AudioQueueSetParameter(_audioQueueRef, kAudioQueueParam_PlayRate, 1.0f);
    } else {
    UInt32 propValue = 0;
    AudioQueueSetProperty(_audioQueueRef, kAudioQueueProperty_TimePitchBypass, &propValue, sizeof(propValue));
    AudioQueueSetParameter(_audioQueueRef, kAudioQueueParam_PlayRate, playbackRate);
    }
    }
    
    - (void)setPlaybackVolume:(float)playbackVolume
    {
    float aq_volume = playbackVolume;
    if (fabsf(aq_volume - 1.0f) <= 0.000001) {
    AudioQueueSetParameter(_audioQueueRef, kAudioQueueParam_Volume, 1.f);
    } else {
    AudioQueueSetParameter(_audioQueueRef, kAudioQueueParam_Volume, aq_volume);
    }
    }
    
    - (double)get_latency_seconds
    {
    return ((double)(kIJKAudioQueueNumberBuffers)) * _spec.samples / _spec.freq;
    }
    
    static void IJKSDLAudioQueueOuptutCallback(void * inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer) {
    @autoreleasepool {
    IJKSDLAudioQueueController* aqController = (__bridge IJKSDLAudioQueueController *) inUserData;
    
    if (!aqController) {
    // do nothing;
    } else if (aqController->_isPaused || aqController->_isStopped) {
    memset(inBuffer->mAudioData, aqController.spec.silence, inBuffer->mAudioDataByteSize);
    } else {
    (*aqController.spec.callback)(aqController.spec.userdata, inBuffer->mAudioData, inBuffer->mAudioDataByteSize);
    }
    
    AudioQueueEnqueueBuffer(inAQ, inBuffer, 0, NULL);
    }
    }
    
}
