//Sample file for students to get their code running

#include "file_manager.h"
#include "errors.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <climits>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;

bool debug=false;

bool allPageMergedRun(vector<int> * sortedindex, vector<int> * endindex){
	int numpage= sortedindex->size();
	bool ans = true;
	for(int i=0;i<numpage;i++){
		if((*sortedindex)[i]<(*endindex)[i]){
			ans=false;
			break;
		}
	}
	return ans;
}

int findLeastPage(vector<int>* sortedindex,vector<int>* endindex,vector<int>* pageindex,FileHandler* fh,PageHandler* ph,char** data){
	int numpage= sortedindex->size();
	int i=-1;
	int num;
	vector<int> firstval;
	for(i=0;i<numpage;i++){
		if((*sortedindex)[i]==(*endindex)[i]){
			firstval.push_back(INT_MAX);
			continue;
		}
		*ph = fh->PageAt((*pageindex)[i]);
		*data = ph->GetData();
		int temp = (*sortedindex)[i];
		memcpy(&num, &((*data)[ temp * 4]),sizeof(int));
		firstval.push_back(num);

	}
	int minindex=-1;
	int minval = INT_MAX;
	for(i=0;i<numpage;i++){
		if(firstval[i]<minval){
			minindex=i;
			minval=firstval[i];
		}
	}
	// if(minindex==-1){
	// 	return -1;
	// }else{
	// 	return (*pageindex)[minindex];
	// }
	return minindex;
}

int findminindex(vector<int>* vec){
	int minindex=0;
	int minval=INT_MAX;
	for(int i=0;i< vec->size();i++){
		if((*vec)[i]<minval){
			minindex=i;
			minval=(*vec)[i];
		}
	}
	return minindex;
}

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
	queue<string> filestr;
	vector<int> endindex;
	vector<int> pageindex;
	vector<int> sortindex;
	while(pagesnotover){
		if(debug) cout << "runiter is "<<runiter<<endl;
		// fm.PrintBuffer();
		endindex.clear();
		pageindex.clear();
		sortindex.clear();
		for(int pageiter=pagestartiter;pageiter<pagestartiter+ BUFFER_SIZE-1;pageiter++){
			try
			{
				ph = fh.PageAt(pageiter);
			}
			catch(const InvalidPageException& e)
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
		// cout << "numpages to be merged are "<<pageindex.size()<<endl;
		if(pageindex.size()==0){
			break;
		}
		//now merge all the pages into a file
		string s = to_string(runiter);
		char const *pchar = s.c_str();
		string runstr("run");
		char* runptr = &runstr[0];
		strcat(runptr, pchar);
		strcat(runptr,".txt");
		FileHandler fhrun;
		try
		{
			fhrun = fm.CreateFile(runptr);
		}
		catch(const InvalidFileException& e)
		{
			fm.DestroyFile(runptr);
			fhrun = fm.CreateFile(runptr);
		}
		
		PageHandler phrun;
		char * datarun;
		// cout << "before creating wrigghting run"<<endl;
		// fm.PrintBuffer();
		// cout <<"begining merge step"<<endl;
		while(!allPageMergedRun(&sortindex,&endindex)){
			phrun = fhrun.NewPage();
			int runpageiter=0;
			datarun=phrun.GetData();
			for(runpageiter=0;runpageiter<PAGE_CONTENT_SIZE/4-1;runpageiter++){
				int leastpageindexvec = findLeastPage(&sortindex,&endindex,&pageindex,&fh,&ph,&data);
				// cout << "least page is "<<leastpageindexvec<<endl;
				if(leastpageindexvec==-1){
					break;
				}else{
					ph = fh.PageAt(pageindex[leastpageindexvec]);
					data = ph.GetData();
					memcpy(&num,&data[sortindex[leastpageindexvec]*4],sizeof(int));
					memcpy(&datarun[runpageiter*4],&num,sizeof(int));
					sortindex[leastpageindexvec]=sortindex[leastpageindexvec]+1;
				}
			}
			num=INT_MIN;
			memcpy(&datarun[runpageiter*4],&num,sizeof(int));
			fhrun.FlushPage(phrun.GetPageNum());
		}
		fm.CloseFile(fhrun);
		runiter++;
		// cout<<"after creating run"<<(runiter-1)<<endl;
		// fm.PrintBuffer();
		for(int i=0;i<pageindex.size();i++){
			fh.UnpinPage(pageindex[i]);
		}
		// cout<<"after unpinning run"<<endl;
		// fm.PrintBuffer();
		string runstr2(runptr);
		filestr.push(runstr2);

	}
	fm.CloseFile(fh);

	// exit(0);
	//run files created now merge them
	int mergeitr=0;
	if(debug) cout <<"num run files are "<< filestr.size()<<endl; 
	vector<string> filehdrstrvec;
	vector<FileHandler> filehdrvec;
	// vector<int> pageindex;
	vector<int> offsetindex;
	vector<bool> statusfile;
	// true means still values there false means values not there
	vector<int> valuesvec;
	vector<bool> updatedvalvec;
	// false means not updated true means it is updated
	while(true){
		//reduce first BUFFER_SIZE-1 files to one and push to the filestr
		if(debug) cout << "merge iter "<<mergeitr<<endl;
		int numfilesmerge;
		bool directmerge=false;
		if(filestr.size()<=BUFFER_SIZE-1){
			numfilesmerge=filestr.size();
			directmerge=true;
			if(debug) cout << "final merge\n";
		}else{
			numfilesmerge= BUFFER_SIZE-1;
			directmerge=false;
			if(debug) cout << "intermin merge file size "<<filestr.size()<<endl;
		}
		if(debug) cout << "numfiles merge is "<<numfilesmerge <<endl;

		filehdrstrvec.clear();
		filehdrvec.clear();
		pageindex.clear();
		offsetindex.clear();
		statusfile.clear();
		// true means still values there false means values not there
		valuesvec.clear();
		updatedvalvec.clear();
		// false means not updated true means it is updated
		int writefilepage=0;
		int writefileoffset=0;
		for(int i=0;i<numfilesmerge;i++){
			string filename = filestr.front();
			filehdrstrvec.push_back(filename);
			if(debug) cout << "merging "<<filename<<endl;
			filestr.pop();
			FileHandler fhtemp = fm.OpenFile(filename.c_str());
			filehdrvec.push_back(fhtemp);
			pageindex.push_back(0);
			offsetindex.push_back(0);
			statusfile.push_back(true);
			valuesvec.push_back(INT_MAX);
			updatedvalvec.push_back(false);
		}

		FileHandler fhmerge;
		string mergefilename;
		if(directmerge){
			try
			{
				fhmerge=fm.CreateFile(argv[2]);
			}
			catch(const InvalidFileException& e)
			{
				fm.DestroyFile(argv[2]);
				fhmerge=fm.CreateFile(argv[2]);
			}
			ph = fhmerge.NewPage();
			string strnew(argv[2]);
			filestr.push(strnew);
			mergefilename=strnew;
		}else{

			string s = to_string(mergeitr);
			char const *pchar = s.c_str();
			string runstr("merge");
			char* runptr = &runstr[0];
			strcat(runptr, pchar);
			strcat(runptr,".txt");
			try
			{
				fhmerge = fm.CreateFile(runptr);
			}
			catch(const InvalidFileException& e)
			{
				fm.DestroyFile(runptr);
				fhmerge = fm.CreateFile(runptr);
			}
			ph = fhmerge.NewPage();
			string strnew(runptr);
			filestr.push(strnew);
			mergefilename=strnew;
		}
		while(true){
			for(int i=0;i<numfilesmerge;i++){
				if(updatedvalvec[i]){
					//do nothing cause it is updated
				}
				else if(!statusfile[i]){
					valuesvec[i]=INT_MAX;
					updatedvalvec[i] =true;
				}else{
					ph = (filehdrvec[i]).PageAt(pageindex[i]);
					data = ph.GetData();
					memcpy(&num, &data[offsetindex[i] * 4], sizeof(int));
					if(num==INT_MIN){
						//change page
						(filehdrvec[i]).UnpinPage(pageindex[i]);
						pageindex[i] = pageindex[i]+1;
						offsetindex[i] = 0;
						try
						{
							ph = (filehdrvec[i]).PageAt(pageindex[i]);
							data = ph.GetData();
							memcpy(&num, &data[offsetindex[i] * 4], sizeof(int));
						}
						catch(const InvalidPageException& e)
						{
							valuesvec[i]=INT_MAX;
							statusfile[i] = false;
							updatedvalvec[i]=true;
							continue;
						}

						
					}
					valuesvec[i]=num;
					updatedvalvec[i]=true;

				}
			}
			int minvalfileindex = findminindex(&valuesvec);
			// cout << "min file index is "<<minvalfileindex<< " value is "<<valuesvec[minvalfileindex]<< " at page "<< pageindex[minvalfileindex] << " offset "<< offsetindex[minvalfileindex] <<endl;
			if(valuesvec[minvalfileindex]==INT_MAX){
				//merge done
				num=INT_MIN;
				ph = fhmerge.PageAt(writefilepage);
				data = ph.GetData();
				memcpy(&data[writefileoffset*4],&num,sizeof(int));
				fhmerge.FlushPage(writefilepage);
				for(int i=0;i<filehdrstrvec.size();i++){
					string tempfilename = filehdrstrvec[i];
					char const *pchartemp = tempfilename.c_str();
					fm.CloseFile(filehdrvec[i]);
					fm.DestroyFile(pchartemp);

				}
				fm.CloseFile(fhmerge);
				break;
			}else{
				ph = (filehdrvec[minvalfileindex]).PageAt(pageindex[minvalfileindex]);
				data = ph.GetData();
				memcpy(&num, &data[offsetindex[minvalfileindex] * 4],sizeof(int));
				offsetindex[minvalfileindex] = offsetindex[minvalfileindex] +1;
				updatedvalvec[minvalfileindex]=false;
				//write num to page
				if(writefileoffset==PAGE_CONTENT_SIZE/4-1){
					ph = fhmerge.PageAt(writefilepage);
					data = ph.GetData();
					int nummin = INT_MIN;
					memcpy(&data[writefileoffset*4], &nummin,sizeof(int));
					fhmerge.FlushPage(writefilepage);
					ph = fhmerge.NewPage();
					writefilepage+=1;
					writefileoffset=0;
				}
				ph = fhmerge.PageAt(writefilepage);
				data = ph.GetData();
				memcpy(&data[writefileoffset*4],&num,sizeof(int));
				writefileoffset+=1;
			}
		}
		if(directmerge){
			if(debug) cout << "merge done"<<endl;
			break;
		}else{
			mergeitr++;
		}

	// exit(0);
	}
	//done everthing

}
