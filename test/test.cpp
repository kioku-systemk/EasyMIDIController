//
//  test.cpp
//  CoreWindowTest
//
//  Created by kioku on 12/07/07.
//  Copyright (c) 2012 System K. All rights reserved.
//

#include "Core/CoreWindow.h"

// GL Headers
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <stdio.h>

#include "../EasyMIDIController.h"

class MainWindow : public CoreWindow
{
public:
	MainWindow(int x, int y, int width, int height)
	: CoreWindow(x, y, width, height, "EasyMIDIController Test")
	{
		m_midi = new EasyMIDIController();
        
        Resize(width, height);
    }
	~MainWindow()
    {
		delete m_midi;
    }
	
    void Draw()
    {
        glClearColor(0.1,0.1,0.1,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        
        const int ctrlnum = 128;
        const int notenum = 128;
        
        struct Vertex {
            float col[3];
            float pos[3];
        };
        Vertex ctrlpoints[ctrlnum * 4];
        Vertex notepoints[notenum * 4];
        for (int i = 0; i < ctrlnum; ++i)
        {
            ctrlpoints[4*i  ].col[0] = 0.5;
            ctrlpoints[4*i  ].col[1] = 1.0;
            ctrlpoints[4*i  ].col[2] = 1.0;
            ctrlpoints[4*i  ].pos[0] = 0.0;
            ctrlpoints[4*i  ].pos[1] = i / static_cast<float>(ctrlnum);
            ctrlpoints[4*i  ].pos[2] = 0.0;
            ctrlpoints[4*i+1].col[0] = 0.5;
            ctrlpoints[4*i+1].col[1] = 1.0;
            ctrlpoints[4*i+1].col[2] = 1.0;
            ctrlpoints[4*i+1].pos[0] = m_midi->GetControlParam(i);
            ctrlpoints[4*i+1].pos[1] = i / static_cast<float>(ctrlnum);
            ctrlpoints[4*i+1].pos[2] = 0.0;
            ctrlpoints[4*i+2].col[0] = 0.5;
            ctrlpoints[4*i+2].col[1] = 1.0;
            ctrlpoints[4*i+2].col[2] = 1.0;
            ctrlpoints[4*i+2].pos[0] = m_midi->GetControlParam(i);
            ctrlpoints[4*i+2].pos[1] = (i+1) / static_cast<float>(ctrlnum);
            ctrlpoints[4*i+2].pos[2] = 0.0;
            ctrlpoints[4*i+3].col[0] = 0.5;
            ctrlpoints[4*i+3].col[1] = 1.0;
            ctrlpoints[4*i+3].col[2] = 1.0;
            ctrlpoints[4*i+3].pos[0] = 0.0f;
            ctrlpoints[4*i+3].pos[1] = (i+1) / static_cast<float>(ctrlnum);
            ctrlpoints[4*i+3].pos[2] = 0.0;
        }
        for (int i = 0; i < notenum; ++i)
        {
            notepoints[4*i  ].col[0] = 1.0;
            notepoints[4*i  ].col[1] = 1.0;
            notepoints[4*i  ].col[2] = 0.5;
            notepoints[4*i  ].pos[0] = 0.0;
            notepoints[4*i  ].pos[1] = i / static_cast<float>(notenum);
            notepoints[4*i  ].pos[2] = 0.0;
            notepoints[4*i+1].col[0] = 1.0;
            notepoints[4*i+1].col[1] = 1.0;
            notepoints[4*i+1].col[2] = 0.5;
            notepoints[4*i+1].pos[0] = m_midi->GetNoteParam(i);
            notepoints[4*i+1].pos[1] = i / static_cast<float>(notenum);
            notepoints[4*i+1].pos[2] = 0.0;
            notepoints[4*i+2].col[0] = 1.0;
            notepoints[4*i+2].col[1] = 1.0;
            notepoints[4*i+2].col[2] = 0.5;
            notepoints[4*i+2].pos[0] = m_midi->GetNoteParam(i);
            notepoints[4*i+2].pos[1] = (i+1) / static_cast<float>(notenum);
            notepoints[4*i+2].pos[2] = 0.0;
            notepoints[4*i+3].col[0] = 1.0;
            notepoints[4*i+3].col[1] = 1.0;
            notepoints[4*i+3].col[2] = 0.5;
            notepoints[4*i+3].pos[0] = 0.0f;
            notepoints[4*i+3].pos[1] = (i+1) / static_cast<float>(notenum);
            notepoints[4*i+3].pos[2] = 0.0;
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glInterleavedArrays(GL_C3F_V3F,0,ctrlpoints);
        glDrawArrays(GL_QUADS,0,ctrlnum*4);
        glInterleavedArrays(GL_C3F_V3F,0,notepoints);
        glDrawArrays(GL_QUADS,0,notenum*4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        SwapBuffer();
    }
     
	void Idle(void)
	{
        Draw();
	}
	
	void Resize(int w, int h)
	{
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 1, 0, 1, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        
        Draw();
	}
    
private:
	EasyMIDIController* m_midi;
};

int main(int argc, char *argv[])
{
	MainWindow win(32, 32, 720, 480);
    CoreWindow::MainLoop();
    return 0;
}
