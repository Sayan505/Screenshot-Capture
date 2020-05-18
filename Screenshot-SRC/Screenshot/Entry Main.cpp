//CONTROLS:
//CTRL+F12, CAPTURE SCREENSHOT.

#include <iostream>
#include <Windows.h>
#include <string>
#include <chrono>

#include <gdiplus.h>//USING GDI+
#include <atlimage.h> //USING ACTIVE TEMPLATE LIBRARY


#define KEY1 1

using namespace std::chrono;


int main(int argc, char **argv) {
	__int64 t;

	//for geting screen input and screen reso
	MSG msg;
	DEVMODEA dm;

	//create device context for screen and memory
	HDC hScreenDC = NULL, hMemoryDC = NULL;

	//handle to Bitmaps
	HBITMAP hBitmap = NULL, hOldBitmap = NULL;

	CString _filename;

	CImage img;


	dm.dmSize = sizeof(DEVMODE);

	EnumDisplaySettingsA(0, ENUM_CURRENT_SETTINGS, &dm);

	//set reso
	int width = dm.dmPelsWidth;
	int height = dm.dmPelsHeight;

	t = duration_cast<std::chrono::milliseconds>
		(system_clock::now().time_since_epoch()).count();
	
	std::ios_base::sync_with_stdio(false);

	if (RegisterHotKey(NULL, KEY1, MOD_CONTROL | MOD_NOREPEAT, 0x7B)) {	//0x7B = F12.
		std::cout << "GLOBAL HOTKEY CTRL+F12 IS REGISTERED ON MEMORY\n\n";
	}


	while (GetMessage(&msg, NULL, 0, 0) != 0) {
		Sleep(10);
	
		if (msg.message == WM_HOTKEY)
		{
			//populate DCs
			hScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);
			hMemoryDC = CreateCompatibleDC(hScreenDC);

			hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

			//store result
			hOldBitmap = (HBITMAP) SelectObject(hMemoryDC, hBitmap);

			//copy color data from hScreenDC to hMemoryDC
			BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

			//FINAL BITMAP
			hBitmap = (HBITMAP) SelectObject(hMemoryDC, hOldBitmap);


			//SAVING CImage
			++t;

			_filename.Format("%llu", t); //convert __int64 to LPCTSTR

			img.Attach(hBitmap);
			img.Save(_filename + ".BMP", Gdiplus::ImageFormatBMP); //save

			std::cout << "Screenshot Captured!\n\n";

			img.Destroy();

			ResetDCA(hMemoryDC, &dm);
			ResetDCA(hScreenDC, &dm);
		}
	}

	return 0;
}
