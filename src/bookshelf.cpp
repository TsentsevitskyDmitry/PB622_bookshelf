#include <iostream>
#include <pthread.h>
#include <signal.h>
#include "mainview.h"

MainView* mainview;
pthread_t controller;

using namespace std;

void test();
struct sigaction logoSigaction;

int main_handler(int type, int par1, int par2)
{
	if (type==EVT_SHOW) 
	{  
		show_logo(); 
	}
	else if (type==EVT_KEYPRESS)
	{ 
		switch (par1)
		{
		case KEY_PREV:
			mainview->forcePrev();
		break;

		case KEY_NEXT:
			mainview->forceNext();
		break;  

		case KEY_MENU:
			mainview->open();
		break;
		}
	}
}

void* background_process(void* lpParam)
{
	while(true)
	{
		usleep(1000000);
		mainview->updateTray();
	}
}


void  close_handler(int sign)
{
	mainview->showLogo();
}

int main(int argc, char **argv)
{  
	mainview = new MainView();
	pthread_create(&controller, NULL, background_process, NULL);
	logoSigaction.sa_handler = close_handler;
	sigaction(SIGTERM, &logoSigaction, NULL);

	InkViewMain(main_handler);
	return 0;
}

