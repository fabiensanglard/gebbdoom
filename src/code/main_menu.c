menuitem_t MainMenu[]= { 
	{1,"M_NGAME", M_NewGame, 'n'}, 
	{1,"M_OPTION",M_Options, 'o'}, 
	{1,"M_LOADG", M_LoadGame,'l'}, 
	{1,"M_SAVEG", M_SaveGame,'s'}, 
	{1,"M_RDTHIS",M_ReadThis,'r'}, 
	{1,"M_QUITG", M_QuitDOOM,'q'} 
}; 

menu_t  MainDef = { 
	main_end, 
	NULL, 
	MainMenu, 
	M_DrawMainMenu, 
	97,64, 
	0 
}; 
