//Sample file for students to get their code running
#include "functions.h"

int main(int argc, const char* argv[]) {
	FileManager fm;
	FileHandler fh;
	PageHandler ph;
	fh = fm.OpenFile(argv[1]);
	int searchint = atoi(argv[2]);
	// cout << argv[1]<<endl;
	// cout << searchint<<endl;
	ph = fh.FirstPage();
	int startpagenum= ph.GetPageNum();
	fh.UnpinPage(startpagenum);
	ph = fh.LastPage();
	int lastpagenum= ph.GetPageNum();
	fh.UnpinPage(lastpagenum);
	int finpage;
	int pageoffset;
	bool found= binarySearchPage(searchint,startpagenum,lastpagenum,startpagenum,lastpagenum,fh,&finpage,&pageoffset);	
	if(found){
		cout << finpage<<","<<pageoffset<<endl;
	}else{
		cout << "-1,-1"<<endl;
	}

}
