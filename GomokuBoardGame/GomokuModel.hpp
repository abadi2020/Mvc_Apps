#pragma once

/*
* FILE NAME:  GomokueMvcApp.hpp
* PROGRAMMER: Abdullah Basaad
* DATE:		  Apr 11, 2019
* PURPOSE:    Mvc Gomoku Model difinition
*
*/

#include "GomokuBoard.hpp"


class GomokuModel
{
private:
	GomokuBoard Board;

public:
	GomokuModel() : Board() {}

	void clear() { Board.clear(); }
};