EasyMIDIController
==================

リアルタイムアプリ向けのMIDI機器の入力を利用するライブラリ(note on/off, control change対応) 

* 特徴
** Windows/MacOSX対応
** 接続されているMIDI機器（複数対応）からパラメータを取得する。
Channel=0のみを取得する。

*使い方:
- - -
<pre><code>
EasyMIDIController* midi = new EasyMIDIController();
//--
float cc   = midi->GetControlParam(i);
float note = midi->GetNoteParam(i);
//--
delete midi;
- - -   

*License:
** This software is public domain.

 