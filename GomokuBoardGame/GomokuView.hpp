#pragma once

/*
* FILE NAME:  GomokueMvcApp.hpp
* PROGRAMMER: Abdullah Basaad
* DATE:		  Apr 11, 2019
* PURPOSE:    Mvc Gomoku Views  difinition - inherits from MvcView
*
*/

#include "../Project1/MvcAppView.hpp"
#include "GomokuEvent.hpp"


class GameBoardView : public MvcAppView
{


public:
	GameBoardView(ConsoleApp::LOCATION_t start, ConsoleApp::LOCATION_t end) : MvcAppView(start, end) {};
	GameBoardView() {};
	virtual bool Proc(Command* theCommand);
	virtual void init();
	virtual void HandleEvent(MvcEvent* e) override;
};


class MovesListView : public MvcAppView
{

	int MovesLoc_Y = 0;
	int MovesLoc_X = 0;
	 std::string TITLE = "* Moves List *";
public:
	MovesListView(ConsoleApp::LOCATION_t start, ConsoleApp::LOCATION_t end) : MvcAppView(start, end) {};
	MovesListView() {};
	virtual bool Proc(Command* theCommand);
	virtual void init();
	virtual void HandleEvent(MvcEvent* e) override;

	
};



class ScoreView : public MvcAppView
{

	int NUM_OF_MOVES = 0;
	int PLAYER_NAMES_LENGTH = 20;
	std::string TITLE = "* Score Board *";
	std::string MovesLbl = "Pieces Played: " + std::to_string( NUM_OF_MOVES);
	ConsoleApp::LOCATION_t MovesLoc;

	void UpdateScore();


public:
	ScoreView(ConsoleApp::LOCATION_t start, ConsoleApp::LOCATION_t end) : MvcAppView(start, end) { MovesLoc.X = 3; MovesLoc.Y = 2; };
	ScoreView() {};
	virtual bool Proc(Command* theCommand);
	virtual void init();
	virtual void HandleEvent(MvcEvent* e) override;
	void UpdateRuns();

};


class EditControl : public MvcAppView
{

	bool HasFocus = false;
	std::string Content;
	std::string::size_type CursorPos = 0;
	decltype(CursorPos) Aperture = 0;
	unsigned Length = 0;

public:
	EditControl(ConsoleApp::LOCATION_t start, ConsoleApp::LOCATION_t end) : MvcAppView(start, end) { Length = end.X - start.X; };
	EditControl() {};
	virtual bool Proc(Command* theCommand);
	virtual void init();
	virtual void HandleEvent(MvcEvent* e) override;
};


class PlayersView : public MvcAppView
{


	std::string TITLE = "* Players Board *";
	std::string CHANGE_NAME_MSG = "Change Player Name";
	 int DEFAULT_LENGHT = 15;
	ConsoleApp::LOCATION_t PlayerNameFldLoc = {Start_Point.X + 3,Start_Point.Y + 3 };
	EditControl PlayerNameTxtFld;
	ConsoleApp::LOCATION_t Player1ButtLoc = { Start_Point.X,Start_Point.Y + 4 };
	ConsoleApp::LOCATION_t Player2ButtLoc = { Player1ButtLoc.X,Player1ButtLoc.Y + 1 };
	


	


public:
	PlayersView(ConsoleApp::LOCATION_t start, ConsoleApp::LOCATION_t end) : MvcAppView(start, end) 
	{ 
		PlayerNameTxtFld = EditControl({ PlayerNameFldLoc.X,PlayerNameFldLoc.Y  }, { (short)(PlayerNameFldLoc.X + DEFAULT_LENGHT), PlayerNameFldLoc.Y  });
		
	};
	PlayersView() {};
	virtual bool Proc(Command* theCommand);
	virtual void init();
	virtual void HandleEvent(MvcEvent* e) override;

	static unsigned PlayerSelectedIndex ;
	void SwitchPlayerSelection();
};



