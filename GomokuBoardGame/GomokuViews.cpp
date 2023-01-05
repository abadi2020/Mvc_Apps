#include "GomokuView.hpp"
#include "GomokuMvcApp.hpp"
#include <string>


#include <algorithm>

 unsigned PlayersView::PlayerSelectedIndex;

#pragma region GameBoardView

void GameBoardView::init()
{

	GomokuMvcApp::getInstance().FillSquare(Start_Point, End_Point, ConsoleApp::BackgroundColor::white);

}


bool GameBoardView::Proc(Command* theCommand)
{


	if (theCommand->EventType == ConsoleApp::MOUSE)
	{


		auto bufferLoc = ((MouseCommand*)theCommand)->EventInfo.dwMousePosition;
		if (isInCoordinate(bufferLoc.X, bufferLoc.Y))
			switch (((MouseCommand*)theCommand)->EventInfo.dwEventFlags) {
			case 0:	// button pressed or released
			{
				auto mask = ((MouseCommand*)theCommand)->EventInfo.dwButtonState;

				if (mask&ConsoleApp::LEFT_BUTTON)
				{
					if (GomokuMvcApp::getInstance().gameEnded)
					{
						GomokuMvcApp::getInstance().NewGame();
					}
					if (GomokuMvcApp::getInstance().GameModel.checkEmpty(bufferLoc.X, bufferLoc.Y))
					{

						GomokuMvcApp::getInstance().FillPixel(bufferLoc, GomokuMvcApp::getInstance().getCurrentTurnColor());
						GomokuMvcApp::getInstance().GameModel.setValue(bufferLoc.X, bufferLoc.Y, GomokuMvcApp::getInstance().getCurrentTurnIndex());



						MvcGomokuEvent* PiecePlacedEv = new PiecePlacedEvent(bufferLoc, GomokuMvcApp::getInstance().getCurrentTurnColor());
						GomokuMvcApp::getInstance().AddMove(bufferLoc.X, bufferLoc.Y);

						GomokuMvcApp::getInstance().GameMoveNotifyAll(PiecePlacedEv);
						


						return true;
					}

				}
			}
			break;


			default:break;

			}


	}

	return false;
}


void GameBoardView::HandleEvent(MvcEvent* e)
{

	switch (((MvcGomokuEvent*)e)->Event_Type)
	{

	case MvcGomokuEvent::Gamewin:
	{
		GameWinEvent ev = *(GameWinEvent*)e;

		GomokuMvcApp::getInstance().FillSquare(Start_Point, End_Point, ev.Event_Info.color);

		COORD loc;
		loc.X = Start_Point.X + 2;
		loc.Y = Start_Point.Y + 1;

		GomokuMvcApp::getInstance().Printout_Text(loc, ev.Event_Info.color, ConsoleApp::ForegroundColor::whitee, ev.Event_Info.player_name + " WON!");




	}
	break;

	default:
		break;
	}

}


#pragma endregion


#pragma region MovesListView

void MovesListView::HandleEvent(MvcEvent* e)
{

	switch (((MvcGomokuEvent*)e)->Event_Type)
	{

	case MvcGomokuEvent::PiecePlaced :
	{
		if (MovesLoc_Y >= this->End_Point.Y - 1)
		{
			MovesLoc_Y = 1;
			MovesLoc_X += 8;

			if (MovesLoc_X  + 5 >= this->End_Point.X)
				init();
		}
		
		PiecePlacedEvent ev = *(PiecePlacedEvent*)e;

		MovesLoc_Y++;

		ConsoleApp::LOCATION_t TextLoc;
		TextLoc.X = MovesLoc_X;
		TextLoc.Y = MovesLoc_Y;

		std::string msg =  (char(ev.Event_Info.loc.Y + 59) +( "  " + std::to_string(ev.Event_Info.loc.X)));

		if (msg.size() != 5)
			msg += " ";

		GomokuMvcApp::getInstance().Printout_Text(TextLoc, ev.Event_Info.color, ConsoleApp::whitee, msg);

		
	}
	break;

	default:
		break;
	}
	
}


void MovesListView::init()
{

	GomokuMvcApp::getInstance().FillSquare(Start_Point, End_Point, ConsoleApp::BackgroundColor::red+ ConsoleApp::BackgroundColor::green);
	ConsoleApp::LOCATION_t loc;

	loc.X = (Start_Point.X + End_Point.X)/2 - TITLE.length()/2;
	loc.Y = Start_Point.Y;
	GomokuMvcApp::getInstance().Printout_Text(loc, ConsoleApp::red, ConsoleApp::whitee, TITLE);


	MovesLoc_Y = 1;
	MovesLoc_X = Start_Point.X + 2;
}

bool MovesListView::Proc(Command* theCommand)
{

	if (theCommand->EventType == ConsoleApp::MOUSE)
	{


		//auto bufferLoc = ((MouseCommand*)theCommand)->EventInfo.dwMousePosition;
		//if ( isInCoordinate(bufferLoc.X, bufferLoc.Y))
		//	switch (((MouseCommand*)theCommand)->EventInfo.dwEventFlags) {
		//	case 0:	// button pressed or released
		//	{
		//		auto mask = ((MouseCommand*)theCommand)->EventInfo.dwButtonState;

		//		if (mask&ConsoleApp::LEFT_BUTTON)
		//		{
		//			if (GomokuMvcApp::getInstance().gameEnded)
		//			{
		//				GomokuMvcApp::getInstance().NewGame();
		//			}
		//			if (GomokuMvcApp::getInstance().GameModel.checkEmpty(bufferLoc.X, bufferLoc.Y))
		//			{

		//				GomokuMvcApp::getInstance().FillPixel(bufferLoc, GomokuMvcApp::getInstance().getCurrentTurnColor());
		//				GomokuMvcApp::getInstance().GameModel.setValue(bufferLoc.X, bufferLoc.Y, GomokuMvcApp::getInstance().getCurrentTurnIndex());


		//				if (GomokuMvcApp::getInstance().GameModel.win(bufferLoc.X, bufferLoc.Y)) {
		//					GomokuMvcApp::getInstance().setWinnerConsole();
		//					GomokuMvcApp::getInstance().gameEnded = true;


		//				}


		//				GomokuMvcApp::getInstance().MoveToNextTurn();


		//				return true;
		//			}

		//		}
		//	}
		//	break;


		//	default:break;

		//	}


	}

	return false;
}

#pragma endregion


#pragma region ScoreView

void ScoreView::HandleEvent(MvcEvent* e)
{

	switch (((MvcGomokuEvent*)e)->Event_Type)
	{

	case MvcGomokuEvent::PiecePlaced:
	{
		PiecePlacedEvent ev = *(PiecePlacedEvent*)e;

		NUM_OF_MOVES++;

		UpdateScore();

	}
	break;

	default:
		break;
	}

}


void ScoreView::init()
{

	GomokuMvcApp::getInstance().FillSquare(Start_Point, End_Point, ConsoleApp::BackgroundColor::red + ConsoleApp::BackgroundColor::blue);
	ConsoleApp::LOCATION_t loc;

	loc.X = (Start_Point.X + End_Point.X) / 2 - TITLE.length() / 2;
	loc.Y = Start_Point.Y;
	GomokuMvcApp::getInstance().Printout_Text(loc, ConsoleApp::red, ConsoleApp::whitee, TITLE);
	GomokuMvcApp::getInstance().Printout_Text(MovesLoc, ConsoleApp::BackgroundColor::red + ConsoleApp::BackgroundColor::blue, ConsoleApp::whitee, MovesLbl);
	UpdateRuns();

	NUM_OF_MOVES = 0;
}

bool ScoreView::Proc(Command* theCommand)
{

	if (theCommand->EventType == ConsoleApp::MOUSE)
	{

	}

	return false;
}


void ScoreView::UpdateScore()
{
	ConsoleApp::LOCATION_t MovesNumLoc = MovesLoc;
	MovesNumLoc.X = (MovesLoc.X + MovesLbl.length())-1;
	GomokuMvcApp::getInstance().Printout_Text(MovesNumLoc, ConsoleApp::BackgroundColor::red + ConsoleApp::BackgroundColor::blue, ConsoleApp::whitee, std::to_string(NUM_OF_MOVES));



}


void ScoreView::UpdateRuns()
{
	ConsoleApp::LOCATION_t MovesNumLoc = MovesLoc;
	MovesNumLoc.Y = MovesLoc.Y+1;

	int index = 0;
	std::string msg ="Longest runs: ";

	GomokuMvcApp::getInstance().Printout_Text(MovesNumLoc, ConsoleApp::BackgroundColor::red + ConsoleApp::BackgroundColor::blue, ConsoleApp::whitee, msg);

	for (size_t i = 0; i < NUM_OF_PLAYERS; i++)
	{
		msg = GomokuMvcApp::getInstance().PlayerNames_ARR[i];
		std::string space =::string(PLAYER_NAMES_LENGTH - msg.size(), ' ');
		msg  += space  +std::to_string(GomokuMvcApp::getInstance().PlayerLongRuns_ARR[i]);
		MovesNumLoc.Y++;
		GomokuMvcApp::getInstance().Printout_Text(MovesNumLoc, ConsoleApp::BackgroundColor::red + ConsoleApp::BackgroundColor::blue, ConsoleApp::whitee, msg);

		
	}


}
#pragma endregion


#pragma region EditControl

void EditControl ::HandleEvent(MvcEvent* e)
{
	switch (((MvcGomokuEvent*)e)->Event_Type)
	{

	case MvcGomokuEvent::PiecePlaced:
	{
		PiecePlacedEvent ev = *(PiecePlacedEvent*)e;



	}
	break;

	default:
		break;
	}

}


void EditControl::init()
{
	GomokuMvcApp::getInstance().FillSquare(Start_Point, End_Point, ConsoleApp::BackgroundColor::white | ConsoleApp::BackgroundColor::bk_intensity);
	Content.clear();
}

bool EditControl::Proc(Command* theCommand)
{

	if (theCommand->EventType == ConsoleApp::MOUSE)
	{	auto ev = ((MouseCommand*)theCommand)->EventInfo;
		if(ev.dwEventFlags == 0)
		{ 
		auto bufferLoc = ((MouseCommand*)theCommand)->EventInfo.dwMousePosition;
		auto isHit = isInCoordinate(bufferLoc.X, bufferLoc.Y);
		if (isHit)
		{
				// show the cursor at the selection point
				CursorPos = min(ev.dwMousePosition.X - Start_Point.X + Aperture, Content.size());
				ConsoleApp::LOCATION_t loc{ SHORT(CursorPos - Aperture + Start_Point.X), Start_Point.Y };
				GomokuMvcApp::getInstance().setCursorPosition(loc, true);
			

		}
		else if (!isHit && HasFocus) 
				GomokuMvcApp::getInstance().setCursorPosition({}, false);
		

		HasFocus = isHit;

		}

	}

	if (theCommand->EventType == ConsoleApp::KEY)

	{	auto ke = ((KeyCommand*)theCommand)->EventInfo;

		if (ke.bKeyDown) {

			if (HasFocus) {
				switch (ke.wVirtualKeyCode) {
				case ConsoleApp::BACK_KEY :
					// backspace to remove at cursor location
					if (0 < CursorPos && CursorPos <= Content.size()) {
						--CursorPos;
						Content.erase(CursorPos, 1);
					}
					break;

				case ConsoleApp::DELETE_KEY:
					if (0 <= CursorPos && CursorPos < Content.size())
						Content.erase(CursorPos, 1);
					break;

				case ConsoleApp::LEFT_KEY:
					if (CursorPos > 0)
						--CursorPos;
					break;

				case ConsoleApp::RIGHT_KEY:
					if (CursorPos < Content.size())
						++CursorPos;
					break;

				case ConsoleApp::END_KEY:
					CursorPos = Content.size();
					break;

				case ConsoleApp::HOME_KEY:
					CursorPos = 0;
					break;

				case ConsoleApp::RETURN_KEY:
					GomokuMvcApp::getInstance().SetPlayerName(PlayersView::PlayerSelectedIndex, Content);
					break;

				default:
					// add character
					char ch = ke.uChar.AsciiChar;
					if (isprint(ch))
						Content.insert(CursorPos++ + Content.begin(), ch);
				
				
				}

				// show the string in the control
				auto practicalSize = Content.size() + 1;
				while (CursorPos < Aperture)
					--Aperture;

				while (CursorPos - Aperture >= Length)
					++Aperture;

				while (practicalSize - Aperture<Length && practicalSize > Length)
					--Aperture;

				auto s = Content.substr(Aperture, Length);
				s += string(Length - s.size(), ' ');

				GomokuMvcApp::getInstance().Printout_Text(this->Start_Point, (unsigned short)(ConsoleApp::white | ConsoleApp::BackgroundColor::bk_intensity), ConsoleApp::blackk, s);
					
		

				// place cursor in the control
				COORD cursorLoc = Start_Point;
				cursorLoc.X += SHORT(CursorPos - Aperture);
				GomokuMvcApp::getInstance().setCursorPosition(cursorLoc, true);

				return true;
			}

		}

	}


	
	return false;
}


#pragma endregion


#pragma region PlayersView

void PlayersView::HandleEvent(MvcEvent* e)
{

	switch (((MvcGomokuEvent*)e)->Event_Type)
	{

	case MvcGomokuEvent::PiecePlaced:
	{
		PiecePlacedEvent ev = *(PiecePlacedEvent*)e;



	}
	break;

	default:
		break;
	}

}


void PlayersView::init()
{

	GomokuMvcApp::getInstance().FillSquare(Start_Point, End_Point, ConsoleApp::BackgroundColor::blue | ConsoleApp::BackgroundColor::red);
	GomokuMvcApp::getInstance().FillPixel(Player1ButtLoc, ConsoleApp::red);
	GomokuMvcApp::getInstance().FillPixel(Player2ButtLoc, ConsoleApp::black);
	
	
	GomokuMvcApp::getInstance().Printout_Text({ PlayerNameFldLoc.X,PlayerNameFldLoc.Y - 1 }, ConsoleApp::red, ConsoleApp::whitee, CHANGE_NAME_MSG);
	ConsoleApp::LOCATION_t loc;

	loc.X = (Start_Point.X + End_Point.X) / 2 - TITLE.length() / 2;
	loc.Y = Start_Point.Y;
	GomokuMvcApp::getInstance().Printout_Text(loc, ConsoleApp::red, ConsoleApp::whitee, TITLE);

	PlayerSelectedIndex = 1;
	SwitchPlayerSelection();
	PlayerNameTxtFld.init();


}

bool PlayersView::Proc(Command* theCommand)
{

	bool proceed = false;

	if (theCommand->EventType == ConsoleApp::MOUSE)
	{
		if (((MouseCommand*)theCommand)->EventInfo.dwEventFlags == 0)
		{

		
		auto m = ((MouseCommand*)theCommand)->EventInfo.dwMousePosition;
	
		if (m.X == Player1ButtLoc.X &&  m.Y == Player1ButtLoc.Y && PlayerSelectedIndex)
		{
			SwitchPlayerSelection();
			proceed = true;

		}

		else if (m.X == Player2ButtLoc.X &&  m.Y == Player2ButtLoc.Y && !PlayerSelectedIndex)
		{
			SwitchPlayerSelection();
			proceed = true;
		}

		}
	}

	if (theCommand->EventType == ConsoleApp::KEY)
	{
	

	}

	if(!proceed)
	if (PlayerNameTxtFld.Proc(theCommand))
		proceed = true;

	if(proceed)
	delete theCommand;

	return proceed;
}


void PlayersView::SwitchPlayerSelection()
{
	if (PlayerSelectedIndex)
	{
		GomokuMvcApp::getInstance().Printout_Text(Player1ButtLoc, ConsoleApp::red, ConsoleApp::whitee, "X");
		GomokuMvcApp::getInstance().Printout_Text(Player2ButtLoc, ConsoleApp::blackk, ConsoleApp::whitee, " ");
		PlayerSelectedIndex = false;

	}

	else

	{
		GomokuMvcApp::getInstance().Printout_Text(Player1ButtLoc, ConsoleApp::red, ConsoleApp::whitee, " ");
		GomokuMvcApp::getInstance().Printout_Text(Player2ButtLoc, ConsoleApp::blackk, ConsoleApp::whitee, "X");
		PlayerSelectedIndex = true;
	}

	

}

#pragma endregion
