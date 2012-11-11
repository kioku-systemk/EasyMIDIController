//
//  MIDIController.h
//
//  Cross platform MIDIController class(Win,MacOSX)
//
//  Created by kioku on 2012/11/05.
//  Copyright (c) 2012 kioku. All rights reserved.
//
//
//  !! This version is Channel0 only. !!
//
//  Reference: http://objective-audio.jp/2008/06/core-midi-midipacket.html
//

#ifndef CoreWindowTest_EasyMIDIController_h
#define CoreWindowTest_EasyMIDIController_h

#if _WIN32
#include <Windows.h>
#endif

class EasyMIDIController
{
public:
    EasyMIDIController();
    ~EasyMIDIController();
    
    float GetControlParam(int i);
    float GetNoteParam(int i);
    
#ifdef __APPLE__
    void MIDIInputProc(const void *pktlist, void *readProcRefCon);
#else
    void OnMidiData(DWORD dwData, DWORD dwTimeStamp);
#endif
    
private:
    bool openMIDI();
    void closeMIDI();
    unsigned char m_ctrlbuf[128];
    unsigned char m_notebuf[128];
    
#ifdef __APPLE__
    unsigned int clientRef;
#else
	static const int MAX_MIDI_NUM = 256;
	HMIDIIN hMidiIn[MAX_MIDI_NUM];
#endif
};

#endif
