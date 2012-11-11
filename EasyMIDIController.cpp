

#include "EasyMIDIController.h"

#include <Windows.h>

#include <stdio.h>

void CALLBACK midiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	EasyMIDIController* thisptr = reinterpret_cast<EasyMIDIController*>(dwInstance);
	if (wMsg == MIM_DATA)
		thisptr->OnMidiData(dwParam1, dwParam2);
	return;
}


EasyMIDIController::EasyMIDIController()
{
	memset(hMidiIn, 0, sizeof(hMidiIn));
	memset(m_ctrlbuf, 0, sizeof(m_ctrlbuf));
	memset(m_notebuf, 0, sizeof(m_notebuf));
	if (!openMIDI())
		printf("Failed open midi\n");
}

bool EasyMIDIController::openMIDI()
{
	unsigned int nStatus;
	
	int devnum = midiInGetNumDevs();
	if (devnum == 0)
		return false;
	
	if (devnum > MAX_MIDI_NUM)
		devnum = MAX_MIDI_NUM;

	for (int i = 0; i < devnum; ++i) { 
		nStatus = midiInOpen(&hMidiIn[i],0,(DWORD)midiInProc, reinterpret_cast<DWORD_PTR>(this), CALLBACK_FUNCTION);
		if (nStatus != MMSYSERR_NOERROR){
			hMidiIn[i] = 0;
			continue;
		}
		midiInReset(hMidiIn[i]);
		midiInStart(hMidiIn[i]);
	}
	return true;
}

void EasyMIDIController::OnMidiData(DWORD dwData, DWORD dwTimeStamp)
{
	unsigned char mes = dwData & 0xF0;
	unsigned char ch = dwData & 0x0F;
	unsigned dt1 = dwData>> 8 & 0xFF;
	unsigned dt2 = dwData>>16 & 0xFF;
	if (ch != 0)
		return;

    if ((mes == 0x90) && (dt2 != 0)) {
		//printf("note on number = %2.2x / velocity = %2.2x / channel = %2.2x\n",dt1, dt2, ch);
		m_notebuf[dt1] = dt2;
	} else if (mes == 0x80 || mes == 0x90) {
        //printf("note off number = %2.2x / velocity = %2.2x / channel = %2.2x\n", dt1, dt2, ch);
		if (dt1 == 0) // all mute
			memset(m_notebuf, 0, sizeof(m_notebuf));
		else
			m_notebuf[dt1] = 0;
	} else if (mes == 0xB0) {
        //printf("cc number = %2.2x / data = %2.2x / channel = %2.2x\n", dt1, dt2, ch);
		m_ctrlbuf[dt1] = dt2;
    }
}

EasyMIDIController::~EasyMIDIController()
{
	closeMIDI();
}

void EasyMIDIController::closeMIDI()
{
	for (int i = 0; i < MAX_MIDI_NUM; ++i) {
		midiInStop(hMidiIn[i]);
        midiInReset(hMidiIn[i]);
        midiInUnprepareHeader(hMidiIn[i], NULL, sizeof(MIDIHDR));
        midiInClose(hMidiIn[i]);
	}
}

float EasyMIDIController::GetControlParam(int i)
{
    if (0 <= i && i < 128)
        return m_ctrlbuf[i] / 127.0f;
    else
        return 0.0f;
}

float EasyMIDIController::GetNoteParam(int i)
{
    if (0 <= i && i < 128)
        return m_notebuf[i] / 127.0f;
    else
        return 0.0f;
}
