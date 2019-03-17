/* 
File Name: olc_camera.cpp 
Purpose: this file contains the 'main' function. Program execution begins and ends there.
Author: Evan Gertis
Date: 03/17/2019
*/

#include "pch.h"
#include <iostream>
class OneLoneCoder_Video : public olcConsoleGameEngine
{
public:
	OneLoneCoder_Video() {
		m_sAppName = L"Video";
	}

private:
	int nCameras = 0;
	SimpleCapParams capture;

protected:
	virtual bool OnUserCreate() {
		nCameras = setupESCAPI();

		//gaurd (if no cameras are found then return false.)
		if (nCameras == 0)
			return false;

		capture.mWidth = ScreenWidth();
		capture.mHeight = ScreenHeight();
		capture.mTargetBuf = new int[ScreenWidth() * ScreenHeight()];

		//gaurd if capture was unsuccessful.
		if (initCapture(0, &capture) == 0) {
			return false;
		}

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) {
		
		//capture webcam image.
		doCapture(0);
		
		//wait for capture notification.
		while (isCaptureDone(0) == 0) {

		}

		for (int x = 0; x < capture.mWidth; x++)
		{
			for (int y = 0; y < capture.mHeight; y++)
			{
				//Get pixel.

				/*
					int - > 32-bits
						   ||	||	||	||
						   \/   \/  \/  \/ 
						   xx   R   G   B(0-255)
				*/
				union RGBint //since this is a union this struct contains 4bytes not 8. The int and char array share the same memory loc.
				{
					int rgb;
					unsigned char c[4];
				};

				RGBint col;
				int id = y * capture.mWidth + x;

				col.rgb = capture.mTargetBuf[id]; 
				int r = col.c[2];
				int g = col.c[1];
				int b = col.c[0];

				//normalize values to a value between 0-1.
				float fR = (float)r / 255.0f;
				float fG = (float)g / 255.0f;
				float fB = (float)b / 255.0f;

				//Convert pixel into char/rgb combination.

				//Draw the pixel.
				if (fG > 0.5f)
					Draw(x, y, PIXEL_SOLID, FG_WHITE);
				else
					Draw(x, y, PIXEL_SOLID, FG_BLACK);

			}

		}
		return true;
	}


};


int main()
{
	//Instantiate the olc Game Engine.
	OneLoneCoder_Video game;
	//game.ConstructConsole(320, 240, 4, 4);
	//game.Start();
	return 0;
}

