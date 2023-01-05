#pragma once


/*
* FILE NAME:  MvcAppView.hpp
* PROGRAMMER: Abdullah Basaad
* DATE:		  Apr 8, 2019
* PURPOSE:    Mvc View Class difintion - all views in the app should derived from it
*
*/

#include "CommandTarget.hpp"
#include "MvcEvent.hpp"


class MvcAppView : public CommandTarget
{
public:

public:
	ConsoleApp::LOCATION_t Start_Point;
	ConsoleApp::LOCATION_t End_Point;


public:
	virtual bool Proc(Command* theCommand) = 0;
	virtual void init() = 0;
	virtual void HandleEvent(MvcEvent* e) {};
	bool isInCoordinate(int x, int y)
	{
		if (this->Start_Point.X <= x && this->End_Point.X >= x && this->Start_Point.Y <= y && this->End_Point.Y >= y)
			return true;

		return false;
	}

	MvcAppView(ConsoleApp::LOCATION_t start, ConsoleApp::LOCATION_t end) : Start_Point(start), End_Point(end) {};
	MvcAppView() {}




};
