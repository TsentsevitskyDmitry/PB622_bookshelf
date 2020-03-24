#include "inkview.h"
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include "fscrawler.h"

extern const ibitmap background, buttons, charge, folder, back;

class MainView
{
private:
	ifont *font_small;	
	ifont *font_big;
	FSCrawler fscrawler;
	int currentPosition;
	int currentBase;

	void drawUi();
	void updateBase();	
	void updatePosition();
	

public:
	MainView();

	void updateTray();
	void update();

	void forceNext();
	void forcePrev();
	void open();

	void showLogo();
};
