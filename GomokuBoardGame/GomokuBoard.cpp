//
//  GomokuBoard.cpp
//  Gomoku
//
//  Created by Thanh Dang on 9/3/15.
//  Copyright (c) 2015 ga. All rights reserved.
//	Source: https://github.com/tdang33/Gomoku-Five-in-a-row-
//
//	Modified by Abdullah Basaad on Feb 23, 2019
//
//

#include <iostream>
#include "GomokuBoard.hpp"
#include <string>
#include "GomokuMvcApp.hpp"
using namespace std;

//clear the current board, to start a new game
void GomokuBoard::clear() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			board[i][j] = '.';
		}
	}
	
}

//constructor . Create a new empty board
GomokuBoard::GomokuBoard() {
	clear();
}

//check to see the position (x,y) is eligible or not (for both the player and computer)
bool GomokuBoard::checkEmpty(int x, int y) {
	if (x >= 0 && x <= N - 1 && y >= 0 && y <= N - 1 && board[x][y] == '.') {
		return true;
	}
	return false;
}

//put X or O at clear the cell at position (x,y)
void GomokuBoard::setValue(int x, int y, int c) {
	board[x][y] = c;
	


	return;
}

//check if the game is over after the move at position (x,y) or not
bool GomokuBoard::win(int x, int y) {
	int runs = GomokuMvcApp::getInstance().GetCurrentPlayerRun();
	return rowOfFive(x, y, runs) || columnOfFive(x, y, runs) || mainDiagnolOfFive(x, y, runs) || reverseDiagnolOfFive(x, y, runs);
}


bool GomokuBoard::rowOfFive(int x, int y, int runs) {
	//check for rows
	int temp = 1;
	int i = 1;
	while (y - i >= 0 && board[x][y - i] == board[x][y]) {
		temp++;
		i++;
	}
	i = 1;
	while (y + i <= N - 1 && board[x][y + i] == board[x][y]) {
		temp++;
		i++;
	}

	if (temp > runs)
		GomokuMvcApp::getInstance().SetCurrentPlayerRun(temp);
	return (temp >= M);
}

bool GomokuBoard::columnOfFive(int x, int y, int runs) {
	//check for columns
	int temp = 1;
	int i = 1;
	while (x - i >= 0 && board[x - i][y] == board[x][y]) {
		temp++;
		i++;
	}
	i = 1;
	while (x + i <= N - 1 && board[x + i][y] == board[x][y]) {
		temp++;
		i++;
	}
	if (temp > runs)
		GomokuMvcApp::getInstance().SetCurrentPlayerRun(temp);
	return (temp >= M);
}

bool GomokuBoard::mainDiagnolOfFive(int x, int y, int runs) {
	//check for main diagnol
	int temp = 1;
	int i = 1;
	while (x - i >= 0 && y - i >= 0 && board[x - i][y - i] == board[x][y]) {
		temp++;
		i++;
	}
	i = 1;
	while (x + i <= N - 1 && y + i <= N - 1 && board[x + i][y + i] == board[x][y]) {
		temp++;
		i++;
	}
	if (temp > runs)
		GomokuMvcApp::getInstance().SetCurrentPlayerRun(temp);
	return (temp >= M);
}

bool GomokuBoard::reverseDiagnolOfFive(int x, int y, int runs) {
	//check for reverse diagnol
	int temp = 1;
	int i = 1;
	while (x - i >= 0 && y + i <= N - 1 && board[x - i][y + i] == board[x][y]) {
		temp++;
		i++;
	}
	i = 1;
	while (x + i <= N - 1 && y - i >= 0 && board[x + i][y - i] == board[x][y]) {
		temp++;
		i++;
	}
	if (temp > runs)
		GomokuMvcApp::getInstance().SetCurrentPlayerRun(temp);
	return (temp >= M);
}


