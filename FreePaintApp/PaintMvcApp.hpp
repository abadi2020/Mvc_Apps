#pragma once

/*
* FILE NAME:  PaintMvcApp.hpp
* PROGRAMMER: Abdullah Basaad
* DATE:		  Apr 16, 2019
* PURPOSE:    Difinition of Paint App with MVC pattern applied
*
*/

#include "../Project1/MvcConsoleApp.hpp"
#include "PaintView.hpp"

class PaintMvcApp : public MvcConsoleApp
{


protected:
	~PaintMvcApp() { RestoreConsoleToOriginal(); }
	PaintMvcApp(unsigned int Height, unsigned int Width);
public:
	PaintMvcApp(PaintMvcApp const &) = delete;
	PaintMvcApp& operator=(PaintMvcApp const &) = delete;
	virtual void Proc(Command* theCommand);
	virtual void Run();
	static PaintMvcApp& getInstance(){static PaintMvcApp instance(35, 90);return instance;}




	PaintCanvasView CanvasView;
	PaintPalletsView PalletsView;
	static const int NUM_OF_VIEWS = 2;
	MvcAppView* ViewsArray[NUM_OF_VIEWS] = { &CanvasView , &PalletsView };

	unsigned int Console_Width;
	unsigned int Console_Height;


	void PaintCell(LOCATION_t loc) { FillPixel(loc, PaintingColor + bk_intensity); }
	void SetPaintingColor(COLOR_t col) { PaintingColor = col; }
private:
	ConsoleApp::COLOR_t PaintingColor = ConsoleApp::red; // default painting color


};