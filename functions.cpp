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
		// cout << "13: Finding in page number " << midpagenum << endl; // Debug
		PageHandler ph = fh.PageAt(midpagenum);
		data = ph.GetData();
		vector<int> vec;
		for(int i = 0; i < PAGE_CONTENT_SIZE/4; i++) {
			int num;
			memcpy(&num, &data[i*4], sizeof(int));
			if(num == INT_MIN){
				// cout << "22: end of page\n"; // Debug
				break;
			}
			vec.push_back(num);
		}
		bool foundvec;
		if(searchint < vec[0]) {
			foundvec = false;
		} else if(searchint > vec[vec.size()-1]) {
			foundvec = false;
		} else {
			foundvec = binary_search(vec.begin(), vec.end(), searchint);
		}

		fh.UnpinPage(midpagenum);
		if(foundvec) {
			*finpage = midpagenum;
			auto temp = find(vec.begin(), vec.end(), searchint);
			*pageoffset = distance(vec.begin(), temp);
			found = true;
			// cout << "42: Found num in page " << midpagenum << " at offset " << *pageoffset << endl; // Debug
			break;
		} else {
			if(searchint < vec[0]) {
				lastpagenum = midpagenum-1;
			} else if (searchint > vec[vec.size()-1]) {
				startpagenum = midpagenum+1;
			} else {
				found = false;
				break;
			}
		}
	}
	return found;
}
