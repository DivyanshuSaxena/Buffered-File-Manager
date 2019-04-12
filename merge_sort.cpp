//Sample file for students to get their code running

#include<iostream>
#include "file_manager.h"
#include "errors.h"
#include<cstring>
#include <bits/stdc++.h> 

using namespace std;


int main(int argc, const char* argv[]) {
	FileManager fm;
	FileHandler fh;
	PageHandler ph;
	char*data;
	int*datai;
	// int * personalbuffer = (int *)malloc(PAGE_CONTENT_SIZE* sizeof(int));
	fh = fm.OpenFile(argv[1]);
	//create runs
	int runiter=0;
	int pagestartiter=0;
	int num;
	bool pagesnotover=true;
	int end;
	while(pagesnotover){
		vector<int> endindex;
		vector<int> pageindex;
		vector<int> sortindex;
		for(int pageiter=pagestartiter;pageiter<pagestartiter+ BUFFER_SIZE-1;pageiter++){
			try
			{
				ph = fh.PageAt(pageiter);
			}
			catch(const std::exception& e)
			{
				pagesnotover=false;
				break;
			}
			pageindex.push_back(pageiter);
			data = ph.GetData();
			// memcpy(personalbuffer)
			for(int i=0;i<PAGE_CONTENT_SIZE/4;i++){
				memcpy(&num,&data[i*4],sizeof(int));
				if(num==INT_MIN){
					end=i;
					break;
				}
			}
			datai = (int *)data;
			sort(&datai[0],&datai[end]);
			endindex.push_back(end);
			sortindex.push_back(0);
			if(pageiter==pagestartiter+ BUFFER_SIZE-2){
				pagestartiter=pageiter+1;
				break;
			}
		}
		//now merge all the pages into a file
		string s = to_string(runiter);
		char const *pchar = s.c_str();
		string runstr("run");
		char* runptr = &runstr[0];
		strcat(runptr, pchar);
		strcat(runptr,".txt");
		FileHandler fhrun = fm.CreateFile(runptr);
		PageHandler phrun;
		while(true){
			phrun = fhrun.NewPage();
			
		}

	}
}
