EasyMIDIController
==================

リアルタイムアプリ向けのMIDI機器の入力を利用するライブラリ(note on/off, control change対応) 
Windows/MacOSX対応

接続されているMIDI機器（複数対応）からパラメータを取得する。
Channel=0のみを取得する。

Usage:
   EasyMIDIController* midi = new EasyMIDIController();
   ~~
   midi->GetControlParam(i);
   m_midi->GetNoteParam(i);
   ~~
   delete midi;
   
 