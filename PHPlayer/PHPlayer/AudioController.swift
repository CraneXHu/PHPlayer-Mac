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
    var audioQueueRef: AudioQueueRef?
    var audioQueueBufferRefs:[AudioQueueBufferRef] = []
    
    override init() {
        super.init()
    }
    
    func initWithAudioSpec(sampleRate: Float64, channels: UInt32) {
        
        //s16p
        var streamDescription : AudioStreamBasicDescription? = AudioStreamBasicDescription()
        streamDescription!.mSampleRate = sampleRate
        streamDescription!.mFormatID = kAudioFormatLinearPCM
        streamDescription!.mFormatFlags = kLinearPCMFormatFlagIsPacked | kLinearPCMFormatFlagIsSignedInteger
        streamDescription!.mChannelsPerFrame = channels
        streamDescription!.mFramesPerPacket = 1
        streamDescription!.mBitsPerChannel = 16
        
        streamDescription!.mBytesPerFrame = streamDescription!.mBitsPerChannel * streamDescription!.mChannelsPerFrame / 8;
        streamDescription!.mBytesPerPacket = streamDescription!.mBytesPerFrame * streamDescription!.mFramesPerPacket;
        
        /* Set the desired format */
        var status: OSStatus = AudioQueueNewOutput(&streamDescription!,
                                              audioQueueOuptutCallback,
                                              UnsafeMutableRawPointer(Unmanaged.passRetained(self).toOpaque()),
                                              nil,
                                              nil,
                                              0,
                                              &audioQueueRef);
        if (status != noErr) {
            return;
        }
        
        var bufferSize: UInt32 = 512*streamDescription!.mBytesPerFrame
        
        
        for i in 0..<audioQueueNumberBuffers
        {
            var audioQueueBufferRef: AudioQueueBufferRef?
            AudioQueueAllocateBuffer(audioQueueRef!, bufferSize, &audioQueueBufferRef)
            audioQueueBufferRefs.append(audioQueueBufferRef!)
        }

        
        var propValue: UInt32 = 1;
        AudioQueueSetProperty(audioQueueRef!, kAudioQueueProperty_EnableTimePitch, &propValue, UInt32(MemoryLayout<UInt32>.size))
        propValue = 1;
        AudioQueueSetProperty(audioQueueRef!, kAudioQueueProperty_TimePitchBypass, &propValue, UInt32(MemoryLayout<UInt32>.size));
        propValue = kAudioQueueTimePitchAlgorithm_Spectral;
        AudioQueueSetProperty(audioQueueRef!, kAudioQueueProperty_TimePitchAlgorithm, &propValue, UInt32(MemoryLayout<UInt32>.size));
        

        
    }
    
    func play() {
        let status: OSStatus = AudioQueueStart(audioQueueRef!, nil)
        if (status != noErr) {
            return ;
        }
    }
    
    func pause() {
        let status: OSStatus = AudioQueuePause(audioQueueRef!)
        if (status != noErr) {
            return ;
        }
    }
    
    func flush() {
        AudioQueueFlush(audioQueueRef!)
    }
    
    func stop() {
        AudioQueueStop(audioQueueRef!, true);
        AudioQueueDispose(audioQueueRef!, true);
    }
    
    func setPlaybackRate(playbackRate: Float) {
        if fabsf(playbackRate - 1.0) <= 0.000001 {
            var propValue: UInt32 = 1
            AudioQueueSetProperty(audioQueueRef!, kAudioQueueProperty_TimePitchBypass, &propValue, UInt32(MemoryLayout<UInt32>.size))
            AudioQueueSetParameter(audioQueueRef!, kAudioQueueParam_PlayRate, 1.0)
        } else {
            var propValue: UInt32 = 0
            AudioQueueSetProperty(audioQueueRef!, kAudioQueueProperty_TimePitchBypass, &propValue, UInt32(MemoryLayout<UInt32>.size))
            AudioQueueSetParameter(audioQueueRef!, kAudioQueueParam_PlayRate, playbackRate)
        }
    }
    
    func setPlaybackVolume(playbackVolume: Float) {
        var aq_volume: Float = playbackVolume
        if (fabsf(aq_volume - 1.0) <= 0.000001) {
            AudioQueueSetParameter(audioQueueRef!, kAudioQueueParam_Volume, 1.0)
        } else {
            AudioQueueSetParameter(audioQueueRef!, kAudioQueueParam_Volume, aq_volume)
        }
    }
    
}

func audioQueueOuptutCallback(clientData: UnsafeMutableRawPointer?, AQ: AudioQueueRef, buffer: AudioQueueBufferRef) {
    
    
    AudioQueueEnqueueBuffer(AQ, buffer, 0, nil)
}
