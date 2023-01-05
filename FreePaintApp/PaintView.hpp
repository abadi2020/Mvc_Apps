#pragma once

/*
* FILE NAME:  PaintView.hpp
* PROGRAMMER: Abdullah Basaad
* DATE:		  Apr 16, 2019
* PURPOSE:    Mvc paint Views  difinition - inherits from MvcView
*
*/

#include "../Project1/MvcAppView.hpp"
#include "PaintEvent.hpp"

class PaintCanvasView : public MvcAppView
{


public:
	PaintCanvasView(ConsoleApp::LOCATION_t start, ConsoleApp::LOCATION_t end) : MvcAppView(start, end) {};
	PaintCanvasView() {};
	virtual bool Proc(Command* theCommand);
	virtual void init();
	virtual void HandleEvent(MvcEvent* e) override;
};


class PalletComponentView : public MvcAppView
{
private: 
	ConsoleApp::COLOR_t ComponentColor;

public:
	PalletComponentView(ConsoleApp::LOCATION_t start, ConsoleApp::LOCATION_t end, ConsoleApp::COLOR_t col) : MvcAppView(start, end), ComponentColor(col){};
	PalletComponentView() {};
	virtual bool Proc(Command* theCommand);
	virtual void init();
};


class PaintPalletsView : public MvcAppView
{

private:
	PalletComponentView RedPallet;
	PalletComponentView GreenPallet;
	PalletComponentView BluePallet;
	PalletComponentView CyanPallet;
	PalletComponentView MagentaPallet;
	PalletComponentView YellowPallet;

	static const int NUM_OF_PALLETS = 6;
	PalletComponentView* AllPallets[NUM_OF_PALLETS] = { &RedPallet, &GreenPallet,&BluePallet,&CyanPallet,&MagentaPallet,&YellowPallet };


public:
	PaintPalletsView(ConsoleApp::LOCATION_t start, ConsoleApp::LOCATION_t end) ;
	PaintPalletsView() {};
	virtual bool Proc(Command* theCommand);
	virtual void init();
	
};

