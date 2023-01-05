#pragma once

/*
* FILE NAME:  GomokueMvcApp.hpp
* PROGRAMMER: Abdullah Basaad
* DATE:		  Apr 8, 2019
* PURPOSE:    An Enhanced Console App with MVC pattern applied
*
*/

#include "ConsoleApp.hpp"
#include "Command.hpp"



class MvcConsoleApp : public ConsoleApp
{

public:


	MvcConsoleApp(unsigned WIDTH, unsigned HEIGHT, std::string TITLE) : ConsoleApp(WIDTH, HEIGHT, TITLE) {}
	virtual void Proc(Command* theCommand) = 0;
	virtual void Run() = 0;
};




