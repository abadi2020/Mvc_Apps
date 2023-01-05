#pragma once

/*
* FILE NAME:  GomokueMvcApp.hpp
* PROGRAMMER: Abdullah Basaad
* DATE:		  Apr 11, 2019
* PURPOSE:    Mvc Gomoku Event type difinition 
*
*/

#include "../Project1/MvcEvent.hpp"


class MvcGomokuEvent : public MvcEvent
{
public:
	enum GomokuEventType_t
	{
		PiecePlaced,
		Gamewin

	};

	GomokuEventType_t Event_Type;

	MvcGomokuEvent(GomokuEventType_t type) : Event_Type(type) {}
};


class PiecePlacedEvent : public MvcGomokuEvent
{
public:
	typedef struct EventInfo
	{
		ConsoleApp::LOCATION_t loc;
		ConsoleApp::COLOR_t color;

		EventInfo(ConsoleApp::LOCATION_t Location, ConsoleApp::COLOR_t col) : loc(Location), color(col) {}
	}event_info;


public:
	PiecePlacedEvent(ConsoleApp::LOCATION_t Location, ConsoleApp::COLOR_t col) : MvcGomokuEvent(GomokuEventType_t::PiecePlaced), Event_Info(Location, col){}
	EventInfo Event_Info;

};


class GameWinEvent : public MvcGomokuEvent
{
public:
	typedef struct EventInfo
	{
		ConsoleApp::LOCATION_t loc;
		ConsoleApp::COLOR_t color;
		std::string player_name;

		EventInfo(ConsoleApp::LOCATION_t Location, ConsoleApp::COLOR_t col, std::string player) : loc(Location), color(col), player_name(player) {}
	}event_info;


public:
	GameWinEvent(ConsoleApp::LOCATION_t Location, ConsoleApp::COLOR_t col, std::string player) : MvcGomokuEvent(GomokuEventType_t::Gamewin), Event_Info(Location, col, player) {}
	EventInfo Event_Info;

};