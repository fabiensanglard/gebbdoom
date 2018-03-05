typedef struct { 
  short  status;	   // {no cursor, ok, arrow ok} 
  char   name[10]; 
  void   (*routine)(int choice); // choice = menu item #. 
  char   alphaKey;     // hotkey in menu 
} menuitem_t; 

typedef struct menu_s { 
  short	numitems;             // # of menu items 
	struct menu_s   *prevMenu;  // previous menu 
	menuitem_t      *menuitems; // menu items 
	void    (*routine)();       // draw routine 
	short   x,y;                // x,y of menu 
	short   lastOn;             // last menu menuitem index 
} menu_t; 