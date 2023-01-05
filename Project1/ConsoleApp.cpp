/*
* FILE NAME:  ConsoleApp.cpp
* PROGRAMMER: Abdullah Basaad
* DATE:		  Feb 17, 2019
* PURPOSE:    ConsoleApp class implementation ( outer facade )
*
*/
#include "ConsoleApp.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;



bool ConsoleApp::isDone = false;


ConsoleApp::ConsoleApp(unsigned WIDTH, unsigned HEIGHT, string TITLE) : conMngr(WIDTH, HEIGHT, TITLE)
{
	isDone = false;
	conMngr.SetCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);

}

ConsoleApp::~ConsoleApp()
{
	RestoreConsoleToOriginal();
}

void ConsoleApp::ClearBuffer()
{
	conMngr.Clear_Buffer();

}


void ConsoleApp::FillPixel(LOCATION_t loc, COLOR_t color)
{
	conMngr.Fill_Pixel(loc, color);
}

void ConsoleApp::FillSquare(LOCATION_t fromLoc, LOCATION_t toLoc, COLOR_t color)
{


	short startX = fromLoc.X;
	short startY = fromLoc.Y;

	for (; startX <= toLoc.X; startX++)
	{
		for (; startY <= toLoc.Y; startY++)
		{
			LOCATION_t loc;
			loc.X = startX;
			loc.Y = startY;
			conMngr.Fill_Pixel(loc, color);
		}

		startY = fromLoc.Y;
	}

}


void ConsoleApp::Printout_Text(LOCATION_t loc, ConsoleApp::COLOR_t background_color, ForegroundColor foreground_color, std::string msg)
{
	conMngr.Print_Text(loc, background_color+ foreground_color, msg);
}

void ConsoleApp::setCursorPosition(LOCATION_t loc, bool isShown)
{
	conMngr.SetCursorPos(loc, isShown);
}

void ConsoleApp::setTextAttribute(COLOR_t color)
{
	conMngr.setTextAttr(color);
}


void ConsoleApp::SetBackgroundColor(BackgroundColor color)
{
	conMngr.Fill_Entire_Area(color);
}



bool ConsoleApp::ReadInputConsole(vector<BUFFER_INPUTS_t>& inBuffer, NUM_EVENTS_t & numEvents)
{
	return conMngr.ReadInputFromConsole(inBuffer, numEvents);
}

void ConsoleApp::IsCursorHidden(bool isHidden)
{
	conMngr.Hide_cursor(isHidden);

}

void ConsoleApp::RestoreConsoleToOriginal()
{
	conMngr.Restore_Original_Console_State();
}

void ConsoleApp::SaveConsoleState()
{
	conMngr.Save_Current_Console_State();
}


void ConsoleApp::InstallConsoleMode()
{
	conMngr.Install_Console_Mode();
}

void ConsoleApp::IsWindowInputEnabled(bool isEnabled)
{
	conMngr.Is_Window_Input_Enabled(isEnabled);

}
void ConsoleApp::IsMouseInputEnabled(bool isEnabled)
{
	conMngr.Is_Mouse_Input_Enabled(isEnabled);

}
void ConsoleApp::IsCtrlCEnabled(bool isEnabled)
{
	conMngr.Is_Ctrl_C_Enabled(isEnabled);

}




void ConsoleApp::MouseEventProc(MOUSE_EVENT_RECORD const & mer)
{
#if !defined(MOUSE_HWHEELED)
#define MOUSE_HWHEELED 0x0008
#endif
	cout << "Mouse event:";
	switch (mer.dwEventFlags) {
	case 0:	// button pressed or released
	{
		auto mask = mer.dwButtonState;
		if (mask&FROM_LEFT_1ST_BUTTON_PRESSED)
			cout << " left pressed";
		if (mask&RIGHTMOST_BUTTON_PRESSED)
			cout << " right pressed";
		if (!mask)
			cout << " no button pressed";
	}
	break;

	case DOUBLE_CLICK:
		cout << " double click";
		break;

	case MOUSE_HWHEELED:
		cout << " horizontal mouse wheel";
		break;

	case MOUSE_MOVED:
		cout << " mouse moved";
		break;

	case MOUSE_WHEELED:
		cout << " vertical mouse wheel";
		break;

	default:
		cout << " unknown";
		break;
	}
	auto bufferLoc = mer.dwMousePosition;
	cout << " screen buffer pos=" << bufferLoc.Y << "," << bufferLoc.X;
	cout << endl;
}

BOOL ConsoleApp::CtrlHandler(DWORD ctrlType)
{
	if (ctrlType <= CTRL_CLOSE_EVENT) {
		/*terminationEventIdx = ctrlType;
		applicationQuitting = true;*/
		isDone = true;
		return TRUE;
	}

	return FALSE;
}


void ConsoleApp::ProcessKeyEvent(KEY_EVENT_RECORD const & ker)
{
	cout << (ker.bKeyDown ? "KP" : "KR");
	cout << ' ' << unsigned(ker.uChar.AsciiChar);
	if (isgraph(ker.uChar.AsciiChar))
		cout << "\t'" << ker.uChar.AsciiChar << '\'';
	cout << ' ';
	auto ks = ker.dwControlKeyState;
	if (ks&CAPSLOCK_ON)
		cout << ",CAPS";
	if (ks&ENHANCED_KEY)
		cout << ",ENH";
	if (ks&LEFT_ALT_PRESSED)
		cout << ",LALT";
	if (ks&LEFT_CTRL_PRESSED)
		cout << ",LCTRL";
	if (ks&NUMLOCK_ON)
		cout << ",NUMLOCK";
	if (ks&RIGHT_ALT_PRESSED)
		cout << ",RALT";
	if (ks&RIGHT_CTRL_PRESSED)
		cout << ",RCTRL";
	if (ks&SCROLLLOCK_ON)
		cout << ",SCROLLLOCK";
	if (ks&SHIFT_PRESSED)
		cout << ",SHIFT";
	cout << "x" << ker.wRepeatCount << "}";
	cout << endl;
}


//
//
//int main()
//{
//
//	ConsoleApp cm = ConsoleApp(100, 20, "TEST");
//	// read input buffer repeatedly
//	vector<INPUT_RECORD> inBuffer(128);
//	while (!isDone) {
//		DWORD numEvents;
//		if (!cm.ReadInputConsole(inBuffer, numEvents)) {
//			cerr << "Failed to read console input\n";
//			break;
//		}
//
//		for (size_t iEvent = 0; iEvent < numEvents; ++iEvent) {
//			switch (inBuffer[iEvent].EventType) {
//
//			case MOUSE_EVENT:
//			{
//				cm.MouseEventProc(inBuffer[iEvent].Event.MouseEvent);
//				break;
//			}
//
//			case KEY_EVENT:
//			{
//
//				cm.ProcessKeyEvent(inBuffer[iEvent].Event.KeyEvent);
//				break;
//			}
//
//			}
//		}
//
//
//
//	}
//
//	cout << "Finished" << endl;
//
//}