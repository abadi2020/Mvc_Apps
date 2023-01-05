#pragma once
/*
* FILE NAME:  GomokueMvcApp.hpp
* PROGRAMMER: Abdullah Basaad
* DATE:		  Apr 10, 2019
* PURPOSE:    Difinition of Gomoku game with MVC pattern applied
*
*/

#include "GomokuModel.hpp"
#include "../Project1/MvcConsoleApp.hpp"

#include "GomokuEvent.hpp"

#include "GomokuView.hpp"

// Scrolling -  https://docs.microsoft.com/en-us/windows/console/scrolling-a-screen-buffer-s-window


 class GomokuMvcApp : public MvcConsoleApp
{

	 size_t PlayerTurnIndex = 0;
#define  NUM_OF_PLAYERS  2



public:
	const BackgroundColor COLORS_ARR[NUM_OF_PLAYERS] = { BackgroundColor::red, BackgroundColor::black };
	  std::string PlayerNames_ARR[NUM_OF_PLAYERS] = { "Player1", "Player2" };
	  int PlayerLongRuns_ARR[NUM_OF_PLAYERS] = { 0,0 };
	

	static GomokuMvcApp& getInstance()
	{
		static GomokuMvcApp instance(25,70);
		return instance;
	}
protected:
	~GomokuMvcApp() { RestoreConsoleToOriginal(); }
	GomokuMvcApp(unsigned int Height, unsigned int Width);
public:
	GomokuMvcApp(GomokuMvcApp const &) = delete;
	GomokuMvcApp& operator=(GomokuMvcApp const &) = delete;
	

public:
	struct PlayMove_t
	{
		int x;
		int y;
		BackgroundColor color;

		PlayMove_t(int X, int Y, BackgroundColor col) : x(X), y(Y), color(col) {}
	};
	unsigned int Console_Width;
	unsigned int Console_Height;
	GomokuBoard GameModel;
	GameBoardView BoardView;
	MovesListView MovesView;
	ScoreView ScoresView;
	PlayersView PlayerView;
	bool gameEnded = false;
	std::vector< PlayMove_t> MovesList;

	static const int NUM_OF_VIEWS = 4;
	MvcAppView* ViewsArray[NUM_OF_VIEWS] = { &BoardView , &MovesView, &ScoresView,&PlayerView };
	


public:
	virtual void Proc(Command* theCommand);
	



	virtual void Run();
	void NewGame();


	void MoveToNextTurn()
	{

		PlayerTurnIndex++;
		if (PlayerTurnIndex >= NUM_OF_PLAYERS)
			PlayerTurnIndex = 0;
	}
	void setWinnerConsole()
	{

		




	}

	void GameMoveNotifyAll(MvcGomokuEvent* Event );
	
	BackgroundColor getCurrentTurnColor() { return COLORS_ARR[PlayerTurnIndex]; }
	std::string getCurrentTurnPlayer() { return PlayerNames_ARR[PlayerTurnIndex]; }
	size_t getCurrentTurnIndex() { return PlayerTurnIndex; }
	void SetCurrentPlayerRun(int runs) { PlayerLongRuns_ARR[PlayerTurnIndex] = runs; ScoresView.UpdateRuns(); }
	void SetPlayerName(int index, std::string name) { if (name != "") { PlayerNames_ARR[index] = name.substr(0, 15); ScoresView.UpdateRuns(); } }
	int GetCurrentPlayerRun() { return  PlayerLongRuns_ARR[PlayerTurnIndex]; }
	void AddMove(int x, int y) { MovesList.push_back(PlayMove_t(x, y, getCurrentTurnColor())); }

};


