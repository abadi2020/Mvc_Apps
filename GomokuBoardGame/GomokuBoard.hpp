//
//  GomokuBoard.h
//  Gomoku
//
//  Created by Thanh Dang on 9/3/15.
//  Copyright (c) 2015 ga. All rights reserved.
//	Source: https://github.com/tdang33/Gomoku-Five-in-a-row-
//
//	Modified by Abdullah Basaad on Feb 23, 2019
//
//

#ifndef __Gomoku__Board__
#define __Gomoku__Board__

#include <stdio.h>
#include <string>
using namespace std;


const int N = 50;
const int M = 5;


class GomokuBoard {
private:
	char board[N][N];

public:
	GomokuBoard();
	void clear();
	bool checkEmpty(int, int);
	bool win(int, int);
	void setValue(int, int, int);
	
	bool rowOfFive(int, int,int);
	bool columnOfFive(int, int,int);
	bool mainDiagnolOfFive(int, int,int);
	bool reverseDiagnolOfFive(int, int,int);

};
#endif /* defined(__Gomoku__Board__) */