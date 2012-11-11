

#include "EasyMIDIController.h"

#include <Foundation/Foundation.h>
#include <CoreMIDI/CoreMIDI.h>


static void MIDIInputProc_(const MIDIPacketList *pktlist, void *readProcRefCon, void *srcConnRefCon)
{
    ((EasyMIDIController*)srcConnRefCon)->MIDIInputProc(pktlist, readProcRefCon);
}

void EasyMIDIController::MIDIInputProc(const void *pktlist, void *readProcRefCon)
{
    MIDIPacket *packet = (MIDIPacket *)&(((MIDIPacketList*)pktlist)->packet[0]);
    UInt32 packetCount = ((MIDIPacketList*)pktlist)->numPackets;
    for (NSInteger i = 0; i < packetCount; i++) {
        unsigned char mes = packet->data[0] & 0xF0;
        unsigned char ch = packet->data[0] & 0x0F;
        //NSLog(@"MIDI(%2.2x,%2.2x)",mes,ch);
        if (ch != 0) { // only ch==0
            packet = MIDIPacketNext(packet);
            continue;
        }
        if ((mes == 0x90) && (packet->data[2] != 0)) {
            //NSLog(@"note on number = %2.2x / velocity = %2.2x / channel = %2.2x", packet->data[1], packet->data[2], ch);
            m_notebuf[packet->data[1]] = packet->data[2];
        } else if (mes == 0x80 || mes == 0x90) {
            //NSLog(@"note off number = %2.2x / velocity = %2.2x / channel = %2.2x", packet->data[1], packet->data[2], ch);
            if (packet->data[1] == 0) // all mute
                memset(m_notebuf, 0, sizeof(m_notebuf));
            else
                m_notebuf[packet->data[1]] = 0;
        } else if (mes == 0xB0) {
            //NSLog(@"cc number = %2.2x / data = %2.2x / channel = %2.2x", packet->data[1], packet->data[2], ch);
            m_ctrlbuf[packet->data[1]] = packet->data[2];
        } else {
            //NSLog(@"etc");
        }
        packet = MIDIPacketNext(packet);
    }
}


EasyMIDIController::EasyMIDIController()
{
    memset(m_ctrlbuf, 0, sizeof(m_ctrlbuf));
    memset(m_notebuf, 0, sizeof(m_notebuf));
    openMIDI();
}

bool EasyMIDIController::openMIDI()
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    OSStatus err;
    
    //MIDIクライアントを作成する
    NSString *clientName = @"midiClient";
    err = MIDIClientCreate((CFStringRef)clientName, NULL, NULL, &clientRef);
    if (err != noErr) {
        NSLog(@"MIDIClientCreate err = %d", err);
        return false;
    }
   
    //MIDIポートを作成する
    MIDIPortRef inputPortRef;
    NSString *inputPortName = @"port";
    err = MIDIInputPortCreate(clientRef, (CFStringRef)inputPortName, MIDIInputProc_, NULL, &inputPortRef);
    if (err != noErr) {
        NSLog(@"MIDIInputPortCreate err = %d", err);
        return false;
    }
    
    //MIDIエンドポイントを取得し、MIDIポートに接続する
    ItemCount sourceCount = MIDIGetNumberOfSources();
    for (ItemCount i = 0; i < sourceCount; i++) {
        MIDIEndpointRef sourcePointRef = MIDIGetSource(i);
        CFStringRef strEndPointRef = NULL;
        CFStringRef strDispnameRef = NULL;
        err = MIDIObjectGetStringProperty(sourcePointRef, kMIDIPropertyName, &strEndPointRef);
        err = MIDIObjectGetStringProperty(sourcePointRef, kMIDIPropertyDisplayName, &strDispnameRef);
        NSLog(@"Dispname = %@, EndPoint = %@", strDispnameRef, strEndPointRef);

        err = MIDIPortConnectSource(inputPortRef, sourcePointRef, this);
        if (err != noErr) {
            NSLog(@"MIDIPortConnectSource err = %d", err);
            return false;
        }
    }
    
    [pool drain];
    return true;
}

EasyMIDIController::~EasyMIDIController()
{
    closeMIDI();
}

void EasyMIDIController::closeMIDI()
{
    MIDIClientDispose(clientRef);
}

float EasyMIDIController::GetControlParam(int i)
{
    if (0 <= i && i < 128)
        return m_ctrlbuf[i] / 128.0f;
    else
        return 0.0f;
}

float EasyMIDIController::GetNoteParam(int i)
{
    if (0 <= i && i < 128)
        return m_notebuf[i] / 128.0f;
    else
        return 0.0f;
}
