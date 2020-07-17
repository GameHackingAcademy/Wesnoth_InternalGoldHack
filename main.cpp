/*
    A DLL that sets the player's gold in Wesnoth 1.14.9 to the value of 999 when the "M" key is pressed. 
    
    Once injected, it creates a thread within the game. This thread waits for a player to hit the "M" key 
    and then uses a series of pointers to directly set the player's gold value to 999. The address 0x017EED18
    represents the player's base pointer in Wesnoth. Finding the address and offsets are covered in
    https://gamehacking.academy/lesson/13
    
    This must be injected into the Wesnoth process to work. One way to do this is to use a DLL injector. 
    Another way is to enable AppInit_DLLs in the registry.
    
    This code is covered in the lab at https://gamehacking.academy/lesson/16
*/

// CreateThread and GetAsyncKeyState are defined within windows.h
#include <Windows.h>

// Our injected thread. Since we want to monitor for the user's keypresses,
// we use a while loop to ensure that this thread never exits. Inside the thread, we
// check if the "M" key is being held down. If so, we directly access the game's memory
// through the use of pointers. We use these pointers to set our player's gold value.
void injected_thread() {
	while (true) {
		if (GetAsyncKeyState('M')) {
			DWORD* player_base = (DWORD*)0x017EED18;
			DWORD* game_base = (DWORD*)(*player_base + 0xA90);
			DWORD* gold = (DWORD*)(*game_base + 4);
			*gold = 999;
		}

        // So our thread doesn't constantly run, we have it pause execution for a millisecond.
        // This allows the processor to schedule other tasks.
		Sleep(1);
	}
}

// When injected, the parent process looks for the DLL's DllMain, similar to the main function in regular executables.
// There are several events that can occur, the most important one for us being DLL_PROCESS_ATTACH. This occurs when the
// DLL is fully loaded into the process' memory.
//
// Once loaded, we create a thread. This thread will run in the background of the game as long as the process remains open.
// The code that this thread will execute is shown above.
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved ) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)injected_thread, NULL, 0, NULL);
	}

	return true;
}
