/*
* FILE NAME:  ConsoleApp.hpp
* PROGRAMMER: Abdullah Basaad
* DATE:		  Feb 15, 2019
* PURPOSE:    ConsoleApp class definition - outer facade to connect the client to the inner facade ( ConsoleManager)
			  The class is to be extended by the client 
*
*/

#pragma once
#include"ConsoleManager.hpp"



class ConsoleApp
{
	private : 
	ConsoleManager conMngr;

	



public:
	
	typedef  DWORD NUM_EVENTS_t;
	typedef  COORD LOCATION_t;
	typedef  MOUSE_EVENT_RECORD MOUSE_EVENT_t;
	typedef  KEY_EVENT_RECORD KEY_EVENT_t;

	typedef INPUT_RECORD BUFFER_INPUTS_t;
	typedef WORD COLOR_t; 

	typedef  enum BackgroundColor {
		black = ConsoleManager::BACKGROUND_BLACK,
		white = ConsoleManager::BACKGROUND_WHITE,
		red = BACKGROUND_RED, green = BACKGROUND_GREEN,
		blue = BACKGROUND_BLUE,
		bk_intensity = BACKGROUND_INTENSITY

	}backgroundColors;
	typedef  enum ForegroundColor {
		blackk = ConsoleManager::FOREGROUND_BLACK,
		whitee = ConsoleManager::FOREGROUND_WHITE,
		redd = FOREGROUND_RED, greenn = FOREGROUND_GREEN,
		bluee = FOREGROUND_BLUE, intensity =  FOREGROUND_INTENSITY
	}foregroundColors;

	

typedef  enum Events {
		MOUSE = MOUSE_EVENT,
		KEY = KEY_EVENT,
		BUFFER_CHANGE = WINDOW_BUFFER_SIZE_EVENT,
		MENU = MENU_EVENT,
		FOCUS = FOCUS_EVENT
	}events;

typedef  enum MouseEvents {
	LEFT_BUTTON = FROM_LEFT_1ST_BUTTON_PRESSED,
	RIGHT_BUTTON = RIGHTMOST_BUTTON_PRESSED,

}mouseEvents;

typedef  enum KeyEvents {
	BACK_KEY = VK_BACK,
	DELETE_KEY = VK_DELETE,
	LEFT_KEY = VK_LEFT,
	RIGHT_KEY = VK_RIGHT,
	END_KEY = VK_END,
	HOME_KEY = VK_HOME,
	RETURN_KEY = VK_RETURN,

}keyEvents;

	ConsoleApp(unsigned WIDTH, unsigned HEIGHT, std::string TITLE);
	virtual  ~ConsoleApp();
	bool ReadInputConsole(std::vector<BUFFER_INPUTS_t>& inBuffer, NUM_EVENTS_t& numEvents);

	

	void RestoreConsoleToOriginal();
	void SaveConsoleState();
	void ClearBuffer();

	static bool isDone;




	// Control Event Handler
	static BOOL  CtrlHandler(DWORD ctrlType);
	void virtual ProcessKeyEvent(KEY_EVENT_RECORD const& ker);
	void virtual MouseEventProc(MOUSE_EVENT_RECORD const& mer);

	// Console Mode
	void IsWindowInputEnabled(bool isEnabled);
	void IsMouseInputEnabled(bool isEnabled);
	void IsCtrlCEnabled(bool isEnabled);
    void IsCursorHidden(bool isHidden);
	void InstallConsoleMode();


	// paint
	void setTextAttribute(COLOR_t color);
	void FillPixel(LOCATION_t loc, COLOR_t color);
	void FillSquare(LOCATION_t fromLoc, LOCATION_t toLoc, COLOR_t color);
	void SetBackgroundColor(BackgroundColor color);
	void Printout_Text(LOCATION_t loc, ConsoleApp::COLOR_t background_color, ForegroundColor foreground_color, std::string msg);
	void setCursorPosition(LOCATION_t, bool);
};
