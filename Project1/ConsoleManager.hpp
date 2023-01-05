/*
* FILE NAME:  ConsoleManager.hpp
* PROGRAMMER: Abdullah Basaad
* DATE:		  Feb 17, 2019
* PURPOSE:    ConsoleManager class definition - inner facade comunicates with Windows32 Api
*
*/

#pragma once
#include <Windows.h>
#undef min

#include <iostream>
#include <sstream>
#include <string>
#include<vector>




class ConsoleMouse
{
public:

	void MouseEventProc(MOUSE_EVENT_RECORD const& mer);


};

class ConsoleKeyboard
{
public:
	// Control Event Handler
	static BOOL CtrlHandler(DWORD ctrlType);
	void ProcessKeyEvent(KEY_EVENT_RECORD const& ker);


};


class ConsoleManager
{
public:
	// Colours
	static WORD  const FOREGROUND_BLACK = 0;
	static WORD const FOREGROUND_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	static WORD const BACKGROUND_BLACK = 0;
	static WORD const BACKGROUND_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;

private:
	// private class will store current user console
	class OldConsole {
	public:
		// System Data

		CONSOLE_SCREEN_BUFFER_INFO	originalCSBI;
		CONSOLE_CURSOR_INFO			originalCCI;
		std::vector<CHAR_INFO>		originalBuffer;
		COORD						originalBufferCoord;
		DWORD						originalConsoleMode;
		std::vector<char>			originalTitle;

		// keyboard
		DWORD						oldConsoleMode;




		OldConsole();
		//{originalTitle = vector<char>(64 * 1024);}
	};

	ConsoleMouse theMouse;
	ConsoleKeyboard theKeyboard;

	HANDLE hConsoleInput, hConsoleOutput;
	WORD	currentConsoleWidth = 0;
	WORD WINDOW_WIDTH = 100;
	WORD WINDOW_HEIGHT = 40;
	DWORD consoleMode;
	OldConsole oldConsole;

	// Application data
	bool applicationQuitting = false;
	DWORD terminationEventIdx = -1;

	bool editControlHasFocus = false;
	std::string editControlString;
	std::string::size_type editControlCursorPos = 0;
	decltype(editControlCursorPos) editControlAperture = 0;

	bool buttonState = false;

public:
	ConsoleManager(WORD WIDTH, WORD HEIGHT, std::string TITLE);

	~ConsoleManager();

	void Resize_window(WORD	height, WORD width);
	void Clear_Buffer();


	void Paint_screen();

	// painting functions 
	void setTextAttr(WORD color);
	void Fill_Entire_Area(WORD color); // "background"
	void Fill_Pixel(COORD loc, WORD color);
	void Print_Text(COORD loc, const WORD Color, std::string msg);
	void SetCursorPos(COORD loc, BOOL);

	// Console Mode Setters
//=======================================================

	void Is_Window_Input_Enabled(BOOL isEnabled);
	void Is_Mouse_Input_Enabled(BOOL isEnabled);
	void Is_Ctrl_C_Enabled(BOOL isEnabled);
	void Hide_cursor(BOOL hide);

	

	void Install_Console_Mode();


	// Event Handlers
//=======================================================
	void ProcessKeyEvent(KEY_EVENT_RECORD const& ker);

	void SetCtrlHandler(PHANDLER_ROUTINE cHndler, BOOL);

	// mouse Event
	void MouseEventProc(MOUSE_EVENT_RECORD const& mer);


	void Save_Current_Console_State();

public:

	void Restore_Original_Console_State();

	bool ReadInputFromConsole(std::vector<INPUT_RECORD>& inBuffer, DWORD& numEvents);
#pragma endregion



};


