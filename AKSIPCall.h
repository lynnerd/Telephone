//
//  AKSIPCall.h
//  Telephone
//
//  Copyright (c) 2008-2009 Alexei Kuznetsov. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of the copyright holder nor the names of contributors
//     may be used to endorse or promote products derived from this software
//     without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE THE COPYRIGHT HOLDER
//  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
//  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
//  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
//  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
//  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#import <Foundation/Foundation.h>
#import <pjsua-lib/pjsua.h>


extern const NSInteger kAKSIPCallsMax;

enum {
  // Before INVITE is sent or received.
  kAKSIPCallNullState =         PJSIP_INV_STATE_NULL,
  
  // After INVITE is sent.
  kAKSIPCallCallingState =      PJSIP_INV_STATE_CALLING,
  
  // After INVITE is received.
  kAKSIPCallIncomingState =     PJSIP_INV_STATE_INCOMING,
  
  // After response with To tag.
  kAKSIPCallEarlyState =        PJSIP_INV_STATE_EARLY,
  
  // After 2xx is sent/received.
  kAKSIPCallConnectingState =   PJSIP_INV_STATE_CONNECTING,
  
  // After ACK is sent/received.
  kAKSIPCallConfirmedState =    PJSIP_INV_STATE_CONFIRMED,
  
  // Session is terminated.
  kAKSIPCallDisconnectedState = PJSIP_INV_STATE_DISCONNECTED
};
typedef NSUInteger AKSIPCallState;

@class AKSIPAccount, AKSIPURI;

@interface AKSIPCall : NSObject {
 @private
  id delegate_;
  
  NSInteger identifier_;
  AKSIPURI *localURI_;
  AKSIPURI *remoteURI_;
  AKSIPCallState state_;
  NSString *stateText_;
  NSInteger lastStatus_;
  NSString *lastStatusText_;
  BOOL incoming_;
  BOOL microphoneMuted_;
  
  // Account the call belongs to
  AKSIPAccount *account_;
}

@property(nonatomic, assign) id delegate;
@property(nonatomic, assign) NSInteger identifier;
@property(nonatomic, copy) AKSIPURI *localURI;
@property(nonatomic, copy) AKSIPURI *remoteURI;
@property(assign) AKSIPCallState state;
@property(copy) NSString *stateText;
@property(assign) NSInteger lastStatus;
@property(copy) NSString *lastStatusText;
@property(nonatomic, readonly, assign, getter=isActive) BOOL active;
@property(nonatomic, readonly, assign) BOOL hasMedia;
@property(nonatomic, readonly, assign) BOOL hasActiveMedia;
@property(assign, getter=isIncoming) BOOL incoming;
@property(nonatomic, assign, getter=isMicrophoneMuted) BOOL microphoneMuted;
@property(nonatomic, readonly, assign, getter=isOnLocalHold) BOOL onLocalHold;
@property(nonatomic, readonly, assign, getter=isOnRemoteHold) BOOL onRemoteHold;
@property(nonatomic, assign) AKSIPAccount *account;

// Designated initializer
- (id)initWithSIPAccount:(AKSIPAccount *)anAccount
              identifier:(NSInteger)anIdentifier;

- (void)answer;
- (void)hangUp;
- (void)sendRingingNotification;
- (void)replyWithTemporarilyUnavailable;
- (void)ringbackStart;
- (void)ringbackStop;
- (void)sendDTMFDigits:(NSString *)digits;
- (void)muteMicrophone;
- (void)unmuteMicrophone;
- (void)toggleMicrophoneMute;
- (void)hold;
- (void)unhold;
- (void)toggleHold;

@end

// Callbacks from PJSUA
void AKSIPCallIncomingReceived(pjsua_acc_id, pjsua_call_id, pjsip_rx_data *);
void AKSIPCallStateChanged(pjsua_call_id, pjsip_event *);
void AKSIPCallMediaStateChanged(pjsua_call_id);


// Notifications.

// Calling. After INVITE is sent.
extern NSString * const AKSIPCallCallingNotification;

// Incoming. After INVITE is received.
// Delegate is not subscribed to this notification.
extern NSString * const AKSIPCallIncomingNotification;

// Early. After response with To tag.
// Keys: // @"AKSIPEventCode", @"AKSIPEventReason".
extern NSString * const AKSIPCallEarlyNotification;

// Connecting. After 2xx is sent/received.
extern NSString * const AKSIPCallConnectingNotification;

// Confirmed. After ACK is sent/received.
extern NSString * const AKSIPCallDidConfirmNotification;

// Disconnected. Session is terminated.
extern NSString * const AKSIPCallDidDisconnectNotification;

// Call media is active.
extern NSString * const AKSIPCallMediaDidBecomeActiveNotification;

// Call media is put on hold by local endpoint.
extern NSString * const AKSIPCallDidLocalHoldNotification;

// Call media is put on hold by remote endpoint.
extern NSString * const AKSIPCallDidRemoteHoldNotification;