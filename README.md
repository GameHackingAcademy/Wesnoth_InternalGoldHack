# Wesnoth Internal Gold Hack
Referenced in https://gamehacking.academy/lesson/3/3.

An internal memory hack for Wesnoth 1.14.9 that modifies the player's gold.

This is an example of a DLL that needs to be injected into Wesnoth. Once injected, it creates a thread within the game. This thread waits for a player to hit the “M” key and then uses a series of pointers to directly set the player's gold value to 999.

This must be injected into the Wesnoth process to work. One way to do this is to use a DLL injector. Another way is to enable AppInit_DLLs in the registry.
