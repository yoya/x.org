Window windowId;	   /*  The window identifier for the main window  */


/*
 *  The declarations for the title bar and the info structure 
 *  needed to create the title bar.
 */

xrEditor * titleBar;
xrTitleBarInfo titleBarInfo =
{
   0,				/* windowId                      */
   {0, 0, 0, 0},		/* editor rectangle              */
   XrVISIBLE | XrSENSITIVE,	/* editor state                  */
   -1, -1,			/* editor colors - use defaults  */
   NULL,			/* editor font - use defaults    */
   "Hello World",		/* title name	                 */
   NULL,			/* gadget 1 - unused             */
   NULL				/* gadget 2 - unused             */
};


/*
 *  The declarations for the menu instance, items contained 
 *  in the menu, and the menu info structure.
 */

xrMenu * menu;	
INT8 * menuItems [9] =
{
   "\\KEHHello Message",	/*  item with a keyboad equiv of ^N  */
   "\\KEGGoodbye Message",	/*  item with a keyboad equiv of ^M  */
   "\\=",			/*  double line seperator item       */
   "Quit"
};

xrMenuInfo menuInfo = 
{
   "Hello World",		/*  Menu title bar string        */
   menuItems,			/*  item array declared above    */
   4,				/*  item count                   */
   NULL,			/*  menu context - unused        */
   0				/*  id returned with menu input  */
};


/*
 *  The declarations for the two message box info structures.
 */

INT8 * niceButton[1] = { "Oky Doky" };

xrMsgBoxInfo niceMsgInfo =
{
   { 40, 45 },			/*  message box origin  */
   NULL,			/*  relative to the RootWindow  */
   NULL,			/*  use default panel context   */
   0,				/*  raster height		*/
   0,				/*  raster width		*/
   0,				/*  raster id			*/
   "Hello World!",		/*  Text message		*/
   niceButton,			/*  button strings		*/
   1				/*  button count		*/
};



INT8 * meanButton[1] = { "Adios" };

xrMsgBoxInfo meanMsgInfo =
{
   { 40, 45 },			/*  message box origin  */
   NULL,			/*  relative to the RootWindow  */
   NULL,			/*  use default panel context   */
   0,				/*  raster height		*/
   0,				/*  raster width		*/
   0,				/*  raster id			*/
   "Goodbye World!",		/*  Text message		*/
   meanButton,			/*  button strings		*/
   1				/*  button count		*/
};
