#include "PaintView.hpp"
#include "PaintMvcApp.hpp"
#include <string>

#pragma region PaintCanvasView

void PaintCanvasView::init()
{

	PaintMvcApp::getInstance().FillSquare(Start_Point, End_Point, ConsoleApp::BackgroundColor::white + ConsoleApp::backgroundColors::bk_intensity);

}


bool PaintCanvasView::Proc(Command* theCommand)
{

	bool isProceed = false;


	if (theCommand->EventType == ConsoleApp::MOUSE)
	{


		auto bufferLoc = ((MouseCommand*)theCommand)->EventInfo.dwMousePosition;
		if (isInCoordinate(bufferLoc.X, bufferLoc.Y))
		{
			auto mask = ((MouseCommand*)theCommand)->EventInfo.dwButtonState;
		
			switch (((MouseCommand*)theCommand)->EventInfo.dwEventFlags) {
			
			case 0:	// button pressed or released
			{
				

				if (mask&ConsoleApp::LEFT_BUTTON)
				{
					PaintMvcApp::getInstance().PaintCell(((MouseCommand*)theCommand)->EventInfo.dwMousePosition);
					isProceed = true;
				}


			}
			break;

			case 1: // Mouse Moved
							{
							if (mask&ConsoleApp::LEFT_BUTTON)
								{
								PaintMvcApp::getInstance().PaintCell(((MouseCommand*)theCommand)->EventInfo.dwMousePosition);
								isProceed = true;
								}
				
								break;
							}
			default:break;

			}


	}

	}
	return isProceed;
}


void PaintCanvasView::HandleEvent(MvcEvent* e)
{

	switch (((MvcPaintEvent*)e)->Event_Type)
	{

	

	default:
		break;
	}

}


#pragma endregion

#pragma region PalletComponentView

void PalletComponentView::init()
{

	PaintMvcApp::getInstance().FillSquare(Start_Point, End_Point, this->ComponentColor + ConsoleApp::bk_intensity);

}

bool PalletComponentView::Proc(Command* theCommand)
{

	bool isProceed = false;


	if (theCommand->EventType == ConsoleApp::MOUSE)
	{


		auto bufferLoc = ((MouseCommand*)theCommand)->EventInfo.dwMousePosition;
		if (isInCoordinate(bufferLoc.X, bufferLoc.Y))
		{
			auto mask = ((MouseCommand*)theCommand)->EventInfo.dwButtonState;

			switch (((MouseCommand*)theCommand)->EventInfo.dwEventFlags) {

			case 0:	// button pressed or released
			{

				if (mask&ConsoleApp::LEFT_BUTTON)
				{
					PaintMvcApp::getInstance().SetPaintingColor(this->ComponentColor);
					isProceed = true;
				}


			}
			break;

	
			default:break;

			}


		}

	}
	return isProceed;
}

#pragma endregion

#pragma region PaintPalletsView

void PaintPalletsView::init()
{

	PaintMvcApp::getInstance().FillSquare(Start_Point, End_Point, ConsoleApp::BackgroundColor::white );
	PaintMvcApp::getInstance().Printout_Text({ 2, ((Start_Point.Y + End_Point.Y) / 2) - 1 }, ConsoleApp::white, ConsoleApp::blackk, " Pallets Bar:");

	AllPallets[0] = &RedPallet;
	AllPallets[1] = &GreenPallet;
	AllPallets[2] = &BluePallet;
	AllPallets[3] = &CyanPallet;
	AllPallets[4] = &YellowPallet;
	AllPallets[5] = &MagentaPallet;

	for (size_t i = 0; i < NUM_OF_PALLETS; i++)
		AllPallets[i]->init();
	
	

}

PaintPalletsView::PaintPalletsView(ConsoleApp::LOCATION_t start, ConsoleApp::LOCATION_t end): MvcAppView(start, end)
{
	int DEF_PALLET_SIZE = 3;
	ConsoleApp::LOCATION_t palletLoc_start;
	palletLoc_start.X = 20;
	palletLoc_start.Y = ((start.Y + end.Y) / 2)-1;

#pragma region red pallet
	ConsoleApp::LOCATION_t palletLoc_end;
	palletLoc_end.X = palletLoc_start.X + DEF_PALLET_SIZE;
	palletLoc_end.Y = palletLoc_start.Y + 1;

	RedPallet = PalletComponentView(palletLoc_start, palletLoc_end, ConsoleApp::red);


#pragma endregion


#pragma region Green Pallet
	palletLoc_start.X = palletLoc_end.X + DEF_PALLET_SIZE;
	palletLoc_end.X = palletLoc_start.X + DEF_PALLET_SIZE;
	GreenPallet = PalletComponentView(palletLoc_start, palletLoc_end, ConsoleApp::green);

#pragma endregion

#pragma region Blue Pallet
	palletLoc_start.X = palletLoc_end.X + DEF_PALLET_SIZE;
	palletLoc_end.X = palletLoc_start.X + DEF_PALLET_SIZE;
	BluePallet = PalletComponentView(palletLoc_start, palletLoc_end, ConsoleApp::blue);

#pragma endregion



#pragma region Cyan Pallet
	palletLoc_start.X = palletLoc_end.X + DEF_PALLET_SIZE;
	palletLoc_end.X = palletLoc_start.X + DEF_PALLET_SIZE;
	CyanPallet = PalletComponentView(palletLoc_start, palletLoc_end,  ConsoleApp::red + ConsoleApp::blue );

#pragma endregion



#pragma region Yellow Pallet
	palletLoc_start.X = palletLoc_end.X + DEF_PALLET_SIZE;
	palletLoc_end.X = palletLoc_start.X + DEF_PALLET_SIZE;
	YellowPallet = PalletComponentView(palletLoc_start, palletLoc_end, ConsoleApp::green + ConsoleApp::red);

#pragma endregion



#pragma region Magenta Pallet
	palletLoc_start.X = palletLoc_end.X + DEF_PALLET_SIZE;
	palletLoc_end.X = palletLoc_start.X + DEF_PALLET_SIZE;
	MagentaPallet = PalletComponentView(palletLoc_start, palletLoc_end, ConsoleApp::blue + ConsoleApp::green);

#pragma endregion




	

}

bool PaintPalletsView::Proc(Command* theCommand)
{

	bool isProceed = false;


	for (size_t i = 0; i < NUM_OF_PALLETS; i++)
	if (AllPallets[i]->Proc(theCommand))
		{
		isProceed = true;
		break;
		}
	
		

	return isProceed;
}

#pragma endregion

