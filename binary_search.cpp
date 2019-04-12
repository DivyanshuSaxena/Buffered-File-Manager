//Sample file for students to get their code running
#include "binary_search.h"
bool binarySearchPage(int searchint, PageHandler ph, FileHandler fh, int startpagenum, int lastpagenum, int * finpage, int * pageoffset){
	bool found=false;
	char *data;
	int num;
	while(true){
		if(startpagenum>lastpagenum){
			found=false;
			break;
		}
		int midpagenum = (startpagenum+lastpagenum)/2;
		ph = fh.PageAt(midpagenum);
		int currentpagenum = ph.GetPageNum();
		data = ph.GetData();
		vector<int> vec;
		for(int i=0;i<1024;i++){
			memcpy(&num, &data[i*4],sizeof(int));
			if(num==INT_MIN){
				// cout<< "end of page\n";
				break;
			}
			// cout <<num<<endl;
			vec.push_back(num);
		}
		bool foundvec = binary_search(vec.begin(),vec.end(),searchint);
		fh.UnpinPage(midpagenum);
		if(foundvec){
			*finpage=midpagenum;
			auto temp = find(vec.begin(),vec.end(),searchint);
			*pageoffset = distance(vec.begin(), temp);
			found=true;
			break;
		}else{
			if(searchint<vec[0]){
				lastpagenum=midpagenum-1;
			}else if(searchint>vec[vec.size()-1]){
				startpagenum=midpagenum+1;
			}else{
				found=false;
				break;
			}
		}
	}
	return found;
}
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
	int num;
	int finpage;
	int pageoffset;
	bool found= binarySearchPage(searchint,ph,fh,startpagenum,lastpagenum,&finpage,&pageoffset);	
	if(found){
		cout << finpage<<","<<pageoffset<<endl;
	}else{
		cout << "-1,-1"<<endl;
	}

}
