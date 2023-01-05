#include "GomokuMvcApp.hpp"

GomokuMvcApp::GomokuMvcApp(unsigned int Height, unsigned int Width) : MvcConsoleApp(Width, Height, "Gomoku MVC V2.0")
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
	

	GameModel = GomokuBoard();

	LOCATION_t board_start;
	board_start.X = 0;
	board_start.Y = 6;   
	LOCATION_t board_end;
	board_end.X = 50;
	board_end.Y = Height;
	
	BoardView = GameBoardView(board_start, board_end);


	board_start.X = board_end.X + 1;
	board_start.Y = 0;

	board_end.X = 70;
	MovesView = MovesListView(board_start, board_end);

	board_start.X = 0;
	board_start.Y = 0;

	board_end.X = 25;
	board_end.Y = 5;
	ScoresView = ScoreView(board_start, board_end);


	board_start.X = board_end.X+1;
	

	board_end.X = 50;
	
	
	PlayerView = PlayersView(board_start, board_end);

}


void GomokuMvcApp::Run()
{
	MovesList =  std::vector<PlayMove_t>();

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

void GomokuMvcApp::Proc(Command * theCommand)
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

			case 'r': NewGame(); break;

			default: break;
			
			}// end switch

		}


	}

	default:
		break;
	}

	delete theCommand;
}


void GomokuMvcApp::GameMoveNotifyAll(MvcGomokuEvent* Event)
{

	switch (Event->Event_Type)
	{
	case MvcGomokuEvent::PiecePlaced:
	{
		PiecePlacedEvent ev = *(PiecePlacedEvent*)Event;
		if (GomokuMvcApp::getInstance().GameModel.win(ev.Event_Info.loc.X, ev.Event_Info.loc.Y)) {


			MvcGomokuEvent* WinGameEv = new GameWinEvent(ev.Event_Info.loc, getCurrentTurnColor(), getCurrentTurnPlayer());
			GameMoveNotifyAll(WinGameEv);
			break;
		}


		MoveToNextTurn(); break; 
	
	
	}
	case MvcGomokuEvent::Gamewin: gameEnded = true; break;



	default:
		break;
	}


	for (size_t i = 0; i < NUM_OF_VIEWS; i++)
		ViewsArray[i]->HandleEvent(Event);


	delete Event;

}


void GomokuMvcApp::NewGame()
{

	this->ClearBuffer();
	GameModel.clear();
	PlayerTurnIndex = 0;
	gameEnded = false;

	for (size_t i = 0; i < NUM_OF_PLAYERS; i++)
	{
		PlayerLongRuns_ARR[i] = 0;
	}
	


	for (size_t i = 0; i < NUM_OF_VIEWS; i++)
		ViewsArray[i]->init();
}

