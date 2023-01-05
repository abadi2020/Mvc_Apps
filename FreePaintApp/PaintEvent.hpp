#pragma once

/*
* FILE NAME:  PaintEvent.hpp
* PROGRAMMER: Abdullah Basaad
* DATE:		  Apr 11, 2019
* PURPOSE:    Mvc Paint Event type difinition
*
*/

#include "../Project1/MvcEvent.hpp"


class MvcPaintEvent : public MvcEvent
{
public:
	enum PaintEventType_t
	{
		PiecePlaced
		

	};

	PaintEventType_t Event_Type;

	MvcPaintEvent(PaintEventType_t type) : Event_Type(type) {}
};


class PiecePlacedEvent : public MvcPaintEvent
{
public:
	typedef struct EventInfo
	{
		ConsoleApp::LOCATION_t loc;
		ConsoleApp::COLOR_t color;

		EventInfo(ConsoleApp::LOCATION_t Location, ConsoleApp::COLOR_t col) : loc(Location), color(col) {}
	}event_info;


public:
	PiecePlacedEvent(ConsoleApp::LOCATION_t Location, ConsoleApp::COLOR_t col) : MvcPaintEvent(PaintEventType_t::PiecePlaced), Event_Info(Location, col) {}
	EventInfo Event_Info;

};
