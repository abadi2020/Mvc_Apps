#include "PaintMvcApp.hpp"


using namespace std;

PaintMvcApp::PaintMvcApp(unsigned int Height, unsigned int Width) : MvcConsoleApp(Width, Height, "Paint MVC V1.0")
{
	Console_Height = Height;
	Console_Width = Width;

	// Console State settings
	IsCursorHidden(true);
	IsWindowInputEnabled(true);
	IsMouseInputEnabled(true);
	IsCtrlCEnabled(true);

	InstallConsoleMode();
	ClearBuffer();
	setCursorPosition({}, false);


	
	LOCATION_t ViewStartLoc;
	ViewStartLoc.X = 0;
	ViewStartLoc.Y = 5;
	LOCATION_t ViewEndLoc;
	ViewEndLoc.X = Width;
	ViewEndLoc.Y = Height;

	CanvasView = PaintCanvasView(ViewStartLoc, ViewEndLoc);


	
	ViewStartLoc.X = 0;
	ViewStartLoc.Y = 0;
	
	ViewEndLoc.X = Width;
	ViewEndLoc.Y = 4;

	PalletsView = PaintPalletsView(ViewStartLoc, ViewEndLoc);



}


void PaintMvcApp::Run()
{
	

	for (size_t i = 0; i < NUM_OF_VIEWS; i++)
		ViewsArray[i]->init();

	// read input buffer repeatedly
	vector<BUFFER_INPUTS_t> inBuffer(128);
	while (!isDone) {
		NUM_EVENTS_t numEvents;
		if (!ReadInputConsole(inBuffer, numEvents)) {
			cerr << "Failed to read console input\n";
			break;
		}

		for (size_t iEvent = 0; iEvent < numEvents; ++iEvent) {
			switch (inBuffer[iEvent].EventType) {

			case Events::MOUSE:
			{
				Command *mouse = new MouseCommand(inBuffer[iEvent].Event.MouseEvent);

				Proc(mouse);
				break;
			}

			case Events::KEY:
			{
				Command *key = new KeyCommand(inBuffer[iEvent].Event.KeyEvent);

				Proc(key);
				break;

			}

			}
		}

	}
}

void PaintMvcApp::Proc(Command * theCommand)
{
	switch (theCommand->EventType)
	{

	case MOUSE:
	{
		for (size_t i = 0; i < NUM_OF_VIEWS; i++)
		{
			if (ViewsArray[i]->Proc(theCommand))
				return;
		}


		return;
	}

	case KEY:
	{

		bool proceed = false;

		for (size_t i = 0; i < NUM_OF_VIEWS; i++)
		{
			if (ViewsArray[i]->Proc(theCommand))
			{
				proceed = true;
				break;
			}

		}
		if (proceed)
			return;

		auto ker = ((KeyCommand*)theCommand)->EventInfo;
		if (ker.bKeyDown)
			if (isgraph(ker.uChar.AsciiChar))
			{

				switch (ker.uChar.AsciiChar)
				{

				case 'r': CanvasView.init(); break;

				default: break;

				}// end switch

			}


	}

	default:
		break;
	}


	delete theCommand;
}

//
//void PaintMvcApp::GameMoveNotifyAll(MvcGomokuEvent* Event)
//{
//
//	switch (Event->Event_Type)
//	{
//	case MvcGomokuEvent::PiecePlaced:
//	{
//		PiecePlacedEvent ev = *(PiecePlacedEvent*)Event;
//		if (GomokuMvcApp::getInstance().GameModel.win(ev.Event_Info.loc.X, ev.Event_Info.loc.Y)) {
//
//
//			MvcGomokuEvent* WinGameEv = new GameWinEvent(ev.Event_Info.loc, getCurrentTurnColor(), getCurrentTurnPlayer());
//			GameMoveNotifyAll(WinGameEv);
//			break;
//		}
//
//
//		MoveToNextTurn(); break;
//
//
//	}
//	case MvcGomokuEvent::Gamewin: gameEnded = true; break;
//
//
//
//	default:
//		break;
//	}
//
//
//	for (size_t i = 0; i < NUM_OF_VIEWS; i++)
//		ViewsArray[i]->HandleEvent(Event);
//
//
//}
