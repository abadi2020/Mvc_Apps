/*
* FILE NAME:  ConsoleManager.cpp
* PROGRAMMER: Abdullah Basaad
* DATE:		  Feb 16, 2019
* PURPOSE:    ConsoleManager class implementation ( inner facade )
*
*/

#include "ConsoleManager.hpp"
#include <algorithm>
#include <iomanip>

#include <vector>
using namespace std;


/* ErrorDescription */
std::string ErrorDescription(DWORD dwMessageID) {
	char* msg;
	auto c = FormatMessageA(
		/* flags */			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
		/* source*/			NULL,
		/* message ID */	dwMessageID,
		/* language */		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		/* buffer */		(LPSTR)&msg,
		/* size */			0,
		/* args */			NULL
	);

	std::string strMsg = (c == 0)
		? "unknown"
		: msg;
	LocalFree(msg);
	return strMsg;
}

/* Console error exception class. */
class XError {
public:
	using id_type = decltype(GetLastError());
	using file_type = char const *;
	using string_type = std::string;
private:
	id_type code_;
	int	line_;
	file_type file_;
public:
	XError(int line, file_type file) : code_(GetLastError()), line_(line), file_(file) {}
	auto code() const -> id_type { return code_; }
	auto line() const -> int { return line_; }
	auto file() const -> file_type { return file_; }

	string_type msg() const {
		std::ostringstream oss;
		oss << "Error: " << code() << "\n";
		oss << ErrorDescription(code()) << "\n";
		oss << "In: " << file() << "\n";
		oss << "Line: " << line() << "\n";
		return oss.str();
	}
};

/* Console error exception throw helper macro. */
#define THROW_IF_CONSOLE_ERROR(res) if(!res) throw XError(__LINE__,__FILE__)
#define THROW_CONSOLE_ERROR() throw XError(__LINE__,__FILE__)


bool done = false;

void ConsoleMouse::MouseEventProc(MOUSE_EVENT_RECORD const& mer) {
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


// Control Event Handler
BOOL ConsoleKeyboard::CtrlHandler(DWORD ctrlType) {
	if (ctrlType <= CTRL_CLOSE_EVENT) {
		/*terminationEventIdx = ctrlType;
		applicationQuitting = true;*/
		done = true;
		return TRUE;
	}

	return FALSE;
}

void ConsoleKeyboard::ProcessKeyEvent(KEY_EVENT_RECORD const& ker) {
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


ConsoleManager::OldConsole::OldConsole()
{
	originalTitle = vector<char>(64 * 1024);
}


ConsoleManager::ConsoleManager(WORD WIDTH, WORD HEIGHT, string TITLE)
{
	hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	Save_Current_Console_State();


	theMouse = ConsoleMouse();
	theKeyboard = ConsoleKeyboard();

	WINDOW_WIDTH = WIDTH;
	WINDOW_HEIGHT = HEIGHT;

	SetConsoleTitleA(TITLE.c_str());


	// ResizeWindow
	Resize_window(WINDOW_HEIGHT, WINDOW_WIDTH);


}

ConsoleManager::~ConsoleManager()
{
	//Restore_Original_Console_State();
}

void ConsoleManager::Resize_window(WORD	height, WORD width)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

	SMALL_RECT sr{ 0 };
	THROW_IF_CONSOLE_ERROR(SetConsoleWindowInfo(hConsoleOutput, TRUE, &sr));

	COORD bufferSize;
	bufferSize.X = WINDOW_WIDTH;
	bufferSize.Y = WINDOW_HEIGHT;
	THROW_IF_CONSOLE_ERROR(SetConsoleScreenBufferSize(hConsoleOutput, bufferSize));

	CONSOLE_SCREEN_BUFFER_INFO sbi;
	THROW_IF_CONSOLE_ERROR(GetConsoleScreenBufferInfo(hConsoleOutput, &sbi));

	sr.Top = sr.Left = 0;
	WINDOW_WIDTH = std::min((SHORT)WINDOW_WIDTH, sbi.dwMaximumWindowSize.X);
	WINDOW_HEIGHT = std::min((SHORT)WINDOW_HEIGHT, sbi.dwMaximumWindowSize.Y);
	sr.Right = WINDOW_WIDTH - 1;
	sr.Bottom = WINDOW_HEIGHT - 1;

	THROW_IF_CONSOLE_ERROR(SetConsoleWindowInfo(hConsoleOutput, TRUE, &sr));
	currentConsoleWidth = sr.Right - sr.Left + 1;
}

void ConsoleManager::Hide_cursor(BOOL hide)

{
	auto newCCI = oldConsole.originalCCI;
	newCCI.bVisible = hide;
	THROW_IF_CONSOLE_ERROR(SetConsoleCursorInfo(hConsoleOutput, &newCCI));
}

void ConsoleManager::Fill_Entire_Area(WORD const BACKGROUND_COLOR)
{

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsoleOutput, &csbi))
	{
		return;
	}

	// Fill the entire screen area white
	DWORD charsWritten;
	DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD cursorHomeCoord{ 0, 0 };
	THROW_IF_CONSOLE_ERROR(FillConsoleOutputCharacterA(hConsoleOutput, ' ', consoleSize, cursorHomeCoord, &charsWritten));
	THROW_IF_CONSOLE_ERROR(FillConsoleOutputAttribute(hConsoleOutput, BACKGROUND_COLOR, consoleSize, cursorHomeCoord, &charsWritten));


		
}

void ConsoleManager::setTextAttr(WORD color)
{
	SetConsoleTextAttribute(hConsoleOutput,
		color);
}

void ConsoleManager::Fill_Pixel(COORD loc, WORD color)
{
	vector<WORD> attr{
	color 
	};

	DWORD nCharsWritten;
	WriteConsoleOutputAttribute(hConsoleOutput, attr.data(), (DWORD)attr.size(), loc, &nCharsWritten);
	//THROW_IF_CONSOLE_ERROR(WriteConsoleOutputAttribute(hConsoleOutput, attr.data(), (DWORD)attr.size(), loc, &nCharsWritten));

	

}

void ConsoleManager::Print_Text(COORD loc, const WORD Color, std::string msg)
{
	//std::vector<int> vec(number_of_elements, default_value);
	vector<WORD> attr(msg.size(), Color);

	DWORD nCharsWritten;
	WriteConsoleOutputCharacterA(hConsoleOutput, msg.c_str(), (DWORD)msg.size(), loc, &nCharsWritten);
	WriteConsoleOutputAttribute(hConsoleOutput, attr.data(), (DWORD)attr.size(), loc, &nCharsWritten);


}

void ConsoleManager::SetCursorPos(COORD loc, BOOL isShown)
{
	CONSOLE_CURSOR_INFO cci{ 10, isShown };
	THROW_IF_CONSOLE_ERROR(SetConsoleCursorInfo(hConsoleOutput, &cci));

	THROW_IF_CONSOLE_ERROR(SetConsoleCursorPosition(hConsoleOutput, loc));
}

void ConsoleManager::Clear_Buffer()
{
	
	COORD coordScreen = { 0, 0 };    // home for the cursor 
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsoleOutput, &csbi))
	{
		return;
	}



	auto dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	THROW_IF_CONSOLE_ERROR(FillConsoleOutputCharacterA(hConsoleOutput,        // Handle to console screen buffer 
		(TCHAR) ' ',     // Character to write to the buffer
		dwConSize,       // Number of cells to write 
		coordScreen,     // Coordinates of first cell 
		&cCharsWritten)// Receive number of characters written
	);

	

}

void ConsoleManager::Paint_screen()
{
	
	// Print message in the middle of the screen
	string msg = "Hello, TUI";
	vector<WORD> attr{
		FOREGROUND_RED | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
		FOREGROUND_GREEN,
		FOREGROUND_BLUE,
		FOREGROUND_RED | FOREGROUND_GREEN,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN,
		FOREGROUND_RED,
		FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
		BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
	};

	COORD loc;
	loc.X = (WINDOW_WIDTH - (SHORT)msg.size()) / 2;
	loc.Y = WINDOW_HEIGHT / 2;
	DWORD nCharsWritten;
	THROW_IF_CONSOLE_ERROR(WriteConsoleOutputCharacterA(hConsoleOutput, msg.c_str(), (DWORD)msg.size(), loc, &nCharsWritten));
	THROW_IF_CONSOLE_ERROR(WriteConsoleOutputAttribute(hConsoleOutput, attr.data(), (DWORD)attr.size(), loc, &nCharsWritten));
}


// Event Handlers
//=======================================================
void ConsoleManager::ProcessKeyEvent(KEY_EVENT_RECORD const& ker)
{
	theKeyboard.ProcessKeyEvent(ker);
}

void ConsoleManager::SetCtrlHandler(PHANDLER_ROUTINE cHndler, BOOL)
{
	THROW_IF_CONSOLE_ERROR(SetConsoleCtrlHandler(cHndler, TRUE));
}

// mouse Event
void ConsoleManager::MouseEventProc(MOUSE_EVENT_RECORD const& mer) { theMouse.MouseEventProc(mer); }


void ConsoleManager::Save_Current_Console_State()
{
	oldConsole = OldConsole();
	oldConsole.originalTitle.resize(size_t(GetConsoleTitleA(oldConsole.originalTitle.data(), (DWORD)oldConsole.originalTitle.size())) + 1);
	oldConsole.originalTitle.shrink_to_fit();

	// Get the old window/buffer size
	THROW_IF_CONSOLE_ERROR(GetConsoleScreenBufferInfo(hConsoleOutput, &oldConsole.originalCSBI));

	// Save the desktop
	oldConsole.originalBuffer.resize(size_t(oldConsole.originalCSBI.dwSize.X)*oldConsole.originalCSBI.dwSize.Y);
	oldConsole.originalBufferCoord = COORD{ 0 };
	SMALL_RECT bufferRect{ 0 };
	bufferRect.Right = oldConsole.originalCSBI.dwSize.X - 1;
	bufferRect.Bottom = oldConsole.originalCSBI.dwSize.Y - 1;
	THROW_IF_CONSOLE_ERROR(ReadConsoleOutputA(hConsoleOutput, oldConsole.originalBuffer.data(), oldConsole.originalCSBI.dwSize, oldConsole.originalBufferCoord, &bufferRect));

	// Save the cursor
	THROW_IF_CONSOLE_ERROR(GetConsoleCursorInfo(hConsoleOutput, &oldConsole.originalCCI));

	// Save Text Attribute 
	
		
	// Save Console Mode
	GetConsoleMode(hConsoleInput, &oldConsole.oldConsoleMode);


}


// console mode setters and installer

void ConsoleManager::Install_Console_Mode()
{
	THROW_IF_CONSOLE_ERROR(SetConsoleMode(hConsoleInput, consoleMode));
}

void ConsoleManager::Is_Window_Input_Enabled(BOOL isEnabled)
{
	
	if (isEnabled)
		consoleMode += ENABLE_WINDOW_INPUT ;
	else
		consoleMode -= ENABLE_WINDOW_INPUT;

}
void ConsoleManager::Is_Mouse_Input_Enabled(BOOL isEnabled)
{
	if (isEnabled)
	{
		consoleMode += ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT;

	}

	else
		consoleMode -= ENABLE_MOUSE_INPUT;

}
void ConsoleManager::Is_Ctrl_C_Enabled(BOOL isEnabled)
{
	if (isEnabled)
		consoleMode += ENABLE_PROCESSED_INPUT;
	else
		consoleMode -= ENABLE_PROCESSED_INPUT;

}




void ConsoleManager::Restore_Original_Console_State()
{
	

	// Restore the original settings/size
	SMALL_RECT sr{ 0 };
	THROW_IF_CONSOLE_ERROR(SetConsoleWindowInfo(hConsoleOutput, TRUE, &sr));
	THROW_IF_CONSOLE_ERROR(SetConsoleScreenBufferSize(hConsoleOutput, oldConsole.originalCSBI.dwSize));
	THROW_IF_CONSOLE_ERROR(SetConsoleWindowInfo(hConsoleOutput, TRUE, &oldConsole.originalCSBI.srWindow));
	Resize_window(oldConsole.originalCSBI.dwSize.X, oldConsole.originalCSBI.dwSize.Y);
	
	// Restore the desktop contents
	SMALL_RECT bufferRect{ 0 };
	bufferRect.Right = oldConsole.originalCSBI.dwSize.X - 1;
	bufferRect.Bottom = oldConsole.originalCSBI.dwSize.Y - 1;
	THROW_IF_CONSOLE_ERROR(SetConsoleTextAttribute(hConsoleOutput,oldConsole.originalCSBI.wAttributes));
	THROW_IF_CONSOLE_ERROR(WriteConsoleOutputA(hConsoleOutput, oldConsole.originalBuffer.data(), oldConsole.originalCSBI.dwSize, oldConsole.originalBufferCoord, &bufferRect));
	
	
	THROW_IF_CONSOLE_ERROR(SetConsoleTitleA(oldConsole.originalTitle.data()));



	// Restore the cursor
	THROW_IF_CONSOLE_ERROR(SetConsoleCursorInfo(hConsoleOutput, &oldConsole.originalCCI));
	THROW_IF_CONSOLE_ERROR(SetConsoleCursorPosition(hConsoleOutput, oldConsole.originalCSBI.dwCursorPosition));


	// rstore keyboard
	THROW_IF_CONSOLE_ERROR(SetConsoleMode(hConsoleInput, oldConsole.oldConsoleMode));



}

bool ConsoleManager::ReadInputFromConsole(vector<INPUT_RECORD>& inBuffer, DWORD& numEvents)
{
	if (!ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), inBuffer.data(), (DWORD)inBuffer.size(), &numEvents)) {
		return false;
	}

	return true;
}


