/*
 Title:		progress.h
 Author:	Badre
 Created:	23/02/2024 
*/
#ifndef _PROGRESS_H
#define _PROGRESS_H

void progressBar(int x, int y, int pmax, char* title, char sep);
void loadingPercent(int x, int y, int pmax, char* title);
void loadingSpinner(int x, int y, int pmax, char* title);
void DoProgress(const char * label, int step, int total, char sep );


#endif
