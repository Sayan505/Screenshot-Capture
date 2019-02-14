//CONTROLS:
//CTRL+F12, CAPTURE SCREENSHOT.

#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>
#include <tchar.h> //_t MACRO

#include <Gdiplus.h> ////USING GDI+
#include <atlimage.h> //USING ACTIVE TEMPLATE LIBRARY


#define KEY1 1

using namespace std;

//ENTRY POINT
int __fastcall _tmain(int argc, TCHAR *argv[]) {
	int x;
	
	if (RegisterHotKey(NULL, KEY1, MOD_CONTROL | MOD_NOREPEAT, 0x7B)) {	//0x7B = F12.
		_tprintf(_T("GLOBAL HOTKEY CTRL+F12 IS REGISTERED ON MEMORY \n \n"));
	}

	MSG msg;
	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);

	while (GetMessage(&msg, NULL, 0, 0) != 0) {
		Sleep(125);
	
		if (msg.message == WM_HOTKEY)
		{
				_tprintf(_T("HOTKEY RECEIVED \n"));
				//MAKE DEVICE CONTEXT FOR THE SCREEN.
				HDC hScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);
				//CREATE A DEVICE CONTEXT FOR MEMORY.
				HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
				
				EnumDisplaySettingsA(0, ENUM_CURRENT_SETTINGS, &dm); // <3
				int width = dm.dmPelsWidth;
				int height = dm.dmPelsHeight;

				HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
				//NEW BITMAP.	
				HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);
				BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
				
				//FINAL BITMAP.
				
				hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);
				
				//COPY hBitmap TO Clipboard.
				OpenClipboard(NULL); //LOCK CLIPBOARD.
				EmptyClipboard(); //CLEAR CLIPBOARD.
				SetClipboardData(CF_BITMAP, hBitmap);
				CloseClipboard(); //RELEASE CLIPBOARD.

				// CLEAR DEVICE CONTEXT/MEMORY. NO NEED TO ReleaseDC().
				DeleteDC(hMemoryDC);
				DeleteDC(hScreenDC);
				
				//IMAGE SAVING CImage
				srand(time(NULL)); //GENERATE RANDOM NUMBER.
				x = rand(); //STORE RANDOM NUMBER.
				CString s;
				s.Format(_T("%d"), x); //CONVERT INT TO LPCTSTR
				CImage img;
				img.Attach(hBitmap);
				img.Save(s+".BMP", Gdiplus::ImageFormatBMP); //SAVE TO FILE.
				
				_tprintf(_T("BITMAP COPIED TO CLIPBOARD, SAVED AND CLEARED DEVICE CONTEXT\n \n"));
		}
	}
	return 0;
}