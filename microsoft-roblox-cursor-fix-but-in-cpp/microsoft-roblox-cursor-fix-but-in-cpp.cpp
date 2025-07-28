// microsoft-roblox-cursor-fix-but-in-cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <iostream>
#include <string>
#include <Psapi.h>


BOOL CALLBACK enum_child_windows(HWND hwnd, LPARAM lParam) {
	char windowText[256];
	GetWindowTextA(hwnd, windowText, sizeof(windowText));
	RECT rect;
	GetWindowRect(hwnd, &rect);
	if (IsWindowVisible(hwnd)) {
		std::cout << windowText << rect.left << " " << rect.top << " " << rect.right << " " << rect.bottom << std::endl;
	}
	//std::cout << windowText << hwnd << std::endl;
	return true;
}

BOOL point_is_equal(POINT p1, POINT p2) {
	return (p1.x == p2.x && p1.y == p2.y);
}

int main()
{
    std::cout << "set Microsoft Roblox as your active window...\n";

	
	
	const int size = 32;
	char currentWindow_str_array[size];
	memset(currentWindow_str_array, 0, sizeof(currentWindow_str_array));
	std::string currentProcessName;
	
	DWORD savedProcessId = 0;
	DWORD currentProcessId = 0;
	HWND savedProcessHwnd = 0;
	int maxMemoryUsage = 200; //max memory usage in MB

	int offset = 20;
	int bottomOffset = offset;
	int topOffset = offset + 20; //account for the title bar height
	int leftOffset = offset;
	int rightOffset = offset;
	
	bool hasSetPosAfterMouseDown = false;
	bool isMouseDown = false;
	bool isKeyToggleDown = false;
	bool cursorClipEnabled = false;
	POINT lastMousePosition;
	GetCursorPos(&lastMousePosition);

	while (true)
	{
		//get the current active window
		HWND currentActiveWindow = GetForegroundWindow();
		if (savedProcessId == 0) {
			//get window title text
			GetWindowTextA(currentActiveWindow, currentWindow_str_array, sizeof(currentWindow_str_array));
			currentProcessName = std::string(currentWindow_str_array);
			if (currentProcessName == "Roblox") {
				//get memory size to not confuse it with website roblox, assuming it doesnt use much memory ig
				GetWindowThreadProcessId(currentActiveWindow, &currentProcessId);
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, currentProcessId);
				if (hProcess != NULL) {
					PROCESS_MEMORY_COUNTERS memCounters;
					bool success = GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&memCounters, sizeof(memCounters));
					if (success) {
						if (memCounters.WorkingSetSize / 1000000 < maxMemoryUsage) {
							savedProcessId = currentProcessId;
							savedProcessHwnd = currentActiveWindow;
							std::cout << "Microsoft Roblox HWND: " << savedProcessHwnd << std::endl;
						}
					}
				}
			}
		}
		//if roblox is active window
		else if (savedProcessHwnd != 0 && currentActiveWindow == savedProcessHwnd) {
			RECT rect;
			GetWindowRect(currentActiveWindow, &rect);
			POINT cursorPos;
			GetCursorPos(&cursorPos);
			
			//border stuff
			int leftBorder = rect.left + leftOffset;
			int rightBorder = rect.right - rightOffset;
			int topBorder = rect.top + topOffset;
			int bottomBorder = rect.bottom - bottomOffset;
			RECT windowRect = { leftBorder, topBorder, rightBorder, bottomBorder };

			if (GetAsyncKeyState(VK_INSERT)) {
				if (!isKeyToggleDown) {
					isKeyToggleDown = true;
					if (cursorClipEnabled) {
						cursorClipEnabled = false;
						std::cout << "cursor clipping: false" << std::endl;
					}
					else {
						cursorClipEnabled = true;
						std::cout << "cursor clipping: true" << std::endl;
					}
				}			
			}
			else {
				isKeyToggleDown = false;
			}

			if (cursorClipEnabled) {
				ClipCursor(&windowRect);
			}
			else {
				ClipCursor(NULL);
			}

			if (GetAsyncKeyState(VK_RBUTTON)) {
				hasSetPosAfterMouseDown = false;
				if (!isMouseDown) {
					isMouseDown = true;
					GetCursorPos(&lastMousePosition);
				}
				SetCursorPos(lastMousePosition.x, lastMousePosition.y);
			}
			else {
				isMouseDown = false;
				if (hasSetPosAfterMouseDown && !point_is_equal(lastMousePosition, cursorPos)) {
					SetCursorPos(lastMousePosition.x, lastMousePosition.y);
					hasSetPosAfterMouseDown = true;
				}
			}
		}

		
		
		
		//std::cout << currentProcessName << std::endl;

		
		//std::cout << currentProcessId << std::endl;
		Sleep(25);
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
