#pragma once


/*
* FILE NAME:  Command.hpp
* PROGRAMMER: Abdullah Basaad
* DATE:		  Apr 7, 2019
* PURPOSE:    The mvc command - defines a way of communication between model and view withing the program
*			  NOTE: this base class needs to be extended by derived classes.
*/


#include "ConsoleApp.hpp"

class Command {

public:
	ConsoleApp::Events EventType;
	bool isProceed;
	Command(ConsoleApp::Events ev):EventType(ev),isProceed(false){}



};

class MouseCommand : public Command {

public:
	ConsoleApp::MOUSE_EVENT_t EventInfo;

	MouseCommand(ConsoleApp::MOUSE_EVENT_t  Event_info) : Command(ConsoleApp::Events::MOUSE) { EventInfo = Event_info; }


};


class KeyCommand : public Command {

public:
	ConsoleApp::KEY_EVENT_t EventInfo;

	KeyCommand(ConsoleApp::KEY_EVENT_t  Event_info) : Command(ConsoleApp::Events::KEY) { EventInfo = Event_info; }


};