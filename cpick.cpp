#include <iostream>
#include <sstream>
#include <cstdlib>
#include <Windows.h>

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

HHOOK keyboardHook;

void printColor() {
	POINT cursorPos;
	COLORREF color;
	GetCursorPos(&cursorPos);
	HDC hdc = GetDC(NULL);
	color = GetPixel(hdc, cursorPos.x, cursorPos.y);
	ReleaseDC(NULL, hdc);

	int red = GetRValue(color);
	int green = GetGValue(color);
	int blue = GetBValue(color);
	int hexValue = (red << 16) | (green << 8) | blue;

	std::stringstream hexStream;
	hexStream << std::hex << hexValue;

	while (hexStream.str().length() < 6) {
		std::stringstream temp;
		temp << "0";
		temp << hexStream.rdbuf();
		hexStream = std::move(temp);
	}

	std::cout << "#" << hexStream.str() << "\n";
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0 && wParam == WM_KEYDOWN) {
		KBDLLHOOKSTRUCT* pKeyInfo = (KBDLLHOOKSTRUCT*)lParam;
		if (pKeyInfo->vkCode == VK_RETURN) {
			system(CLEAR_COMMAND);
			printColor();
		}
	}
	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

int main() {
	std::cout << "Enter to return selected pixel's color...";
	
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

	if (keyboardHook == NULL) {
		std::cerr << "Failed to set hook!" << std::endl;
		return 1;
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(keyboardHook);
	return 0;
}
