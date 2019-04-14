#include <climits>
#include "functions.h"

bool binarySearchPage(int searchint, FileHandler fh, int startpagenum, int lastpagenum, int * finpage, int * pageoffset){
	bool found=false;
	char *data;
	while(true) {
		if(startpagenum > lastpagenum) {
			found=false;
			break;
		}
		int midpagenum = (startpagenum + lastpagenum)/2;
		PageHandler ph = fh.PageAt(midpagenum);
		data = ph.GetData();
		vector<int> vec;
		for(int i = 0; i < PAGE_CONTENT_SIZE/4; i++) {
			int num;
			memcpy(&num, &data[i*4], sizeof(int));
			if(num == INT_MIN){
				break;
			}
			vec.push_back(num);
		}
		// cout << "Read page " << midpagenum << " into vector" << endl; // Debug
		fh.FlushPage(midpagenum);
		if(searchint < vec[0]) {
			lastpagenum = midpagenum-1;
		} else if(searchint > vec[vec.size()-1]) {
			startpagenum = midpagenum+1;
		} else {
			// cout << "Number is expected to be here" << endl; // Debug
			// We should have the number in this page
			*finpage = midpagenum;
			for (int i = 0; i < vec.size(); i++) {
				if (vec[i] == searchint) {
					*pageoffset = i;
					found = true;
					break;
				} else if (vec[i] > searchint) {
					*pageoffset = i;
					break;
				}
			}
			break;
		}
	}
	return found;
}
