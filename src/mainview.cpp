#include "mainview.h"

MainView::MainView()
{
	char font_name[128];
	sprintf(font_name, "#%s,24,0", DEFAULTFONTB);
	font_small = GetThemeFont("clock.font", font_name);
	sprintf(font_name, "#%s,36,0", DEFAULTFONTB);
	font_big = GetThemeFont("clock.font", font_name);
	currentPosition = 0;

	OpenScreen();
	SetAutoPowerOff(0);

    ClearScreen();
	updateTray();
	drawUi();
}

void MainView::drawUi()
{
	DrawBitmap(0, 750, &buttons);
    DrawLine(0, 750, 600, 750, BLACK);
	update();
}

void MainView::update()
{
	static int fullUpdate;
	if(fullUpdate > 10)
	{
		FullUpdate();
		fullUpdate = 0;
	}
	else ++fullUpdate;

    FillArea(0 , 51 , 600, 699, WHITE);
	updatePosition();
	updateBase();
	SoftUpdate();
}


void MainView::updateBase()
{
    SetFont(font_big, BLACK);
	for(int i = 0; i < 10; ++i)
	{
		if(fscrawler.isBack(currentBase + i)) DrawBitmap(0, 60 + 70 * i, &back);		
		else if(fscrawler.isDirectory(currentBase + i)) DrawBitmap(0, 60 + 70 * i, &folder);		
		DrawTextRect(50, 50 + 70 * i, 550, 70, fscrawler.getName(currentBase + i), ALIGN_CENTER | VALIGN_MIDDLE);
    	DrawLine(50, 120 +  70 * i, 550, 120 + 70 * i, DGRAY);
	}
}

void MainView::updatePosition()
{
	int index = currentPosition + 1;
	FillArea(0 , 50 + 70 * index , 600, 70, LGRAY);
}


void MainView::updateTray()
{
	    SetFont(font_small, BLACK);
	    SetTextStrength(1);

		time_t t = time(NULL);
		struct tm *t_m;
		t_m=localtime(&t);
		char time[11];
		sprintf(time, "%2d:%02d\n",t_m->tm_hour, t_m->tm_min); 

	char capacity[5];
	char status[12];


    ifstream fbat("/sys/class/power_supply/battery/capacity"); 
    if (!fbat.is_open()) 
		cout << "battery capacity error" << endl;
	else 
	{
	  	fbat.getline(capacity, 5);
	    if (capacity[2] == NULL) capacity[1] = '%';
	    else if (capacity[3] == NULL) capacity[2] = '%';
	    else if (capacity[4] == NULL) capacity[3] = '%';
		fbat.close();  
	}    

    ifstream fstat("/sys/class/power_supply/battery/capacity"); 
    if (!fstat.is_open()) 
		cout << "battery status error" << endl;
	else 
	{
	  	fstat.getline(status, 12)
		fstat.close();  
	} 

	FillArea(0 , 0 , 600, 50, WHITE);
	DrawLine(0, 50, 600, 50, BLACK);

	DrawTextRect(500, 0, 100, 50, capacity, ALIGN_CENTER | VALIGN_MIDDLE);
	if (status[0] == 'C')
		DrawBitmap(450, 0, &charge);
	DrawTextRect(0, 0, 600, 50, time, ALIGN_CENTER | VALIGN_MIDDLE);

	SoftUpdate();
}

void MainView::forceNext()
{
	if (currentPosition < 7 && currentPosition < fscrawler.filesCount() - 3)
		++currentPosition;
	else if (currentBase < fscrawler.filesCount() - 9)
		++currentBase;
	else return;

	update();
}

void MainView::forcePrev()
{
	if (currentPosition > 0)
		--currentPosition;
	else if (currentBase > 0)
		--currentBase;
	else return;

	update();
}

void MainView::open()
{
	if (fscrawler.isDirectory(currentBase + currentPosition))
		{
			fscrawler.open(currentBase + currentPosition);
			currentPosition = 0;
			currentBase = 0;
			update();
		}
	else
		OpenBook(fscrawler.getPath(currentBase + currentPosition), "", 0);

}

void MainView::showLogo()
{
    FillArea(0 , 0 , 600, 800, WHITE);
	DrawBitmap(0, 0, &background);
	FullUpdate();
}
