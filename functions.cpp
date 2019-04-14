#include <climits>
#include "functions.h"

bool binarySearchPage(int searchInt, int startPageNum, int lastPageNum, int firstPageNum, int endPageNum, FileHandler fh, int * finPage, int * pageOffset){
	bool found=false;
	char *data;
	int foundPage = -1;
	while(true) {
		if(startPageNum > lastPageNum) {
			found=false;
			break;
		}

		int midPageNum = (startPageNum + lastPageNum)/2;
		PageHandler ph = fh.PageAt(midPageNum);
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
		cout << "Read page " << midPageNum << " into vector" << endl; // Debug
		fh.FlushPage(midPageNum);
		
		// Check if number is in the range of mid page
		if (midPageNum == firstPageNum) {
			if (searchInt <= vec[vec.size()-1]) {
				foundPage = midPageNum;
			} else {
				startPageNum = midPageNum+1;
			}
		} else if (midPageNum == lastPageNum) {
			if (searchInt >= vec[0]) {
				foundPage = midPageNum;
			} else {
				lastPageNum = midPageNum-1;
			}
		} else {
			PageHandler prevPage = fh.PrevPage(midPageNum);
			char* prevData = prevPage.GetData();
			PageHandler nextPage = fh.NextPage(midPageNum);
			char* nextData = nextPage.GetData();
			vector<int> prevVec;
			for(int i = 0; i < PAGE_CONTENT_SIZE/4; i++) {
				int num;
				memcpy(&num, &prevData[i*4], sizeof(int));
				if(num == INT_MIN){
					break;
				}
				prevVec.push_back(num);
			}
			int lowerBound = prevVec[prevVec.size()-1];
			int upperBound;
			memcpy(&upperBound, &nextData[0], sizeof(int));
			if (searchInt > lowerBound && searchInt < upperBound) {
				foundPage = midPageNum;
			} else if (searchInt >= upperBound) {
				startPageNum = midPageNum+1;
			} else {
				lastPageNum = midPageNum-1;
			}
		}

		// Check if a matching page has been found
		if (foundPage != -1) {
			// We should have the number in this page
			cout << "Set finpage to " << foundPage << endl;
			*finPage = foundPage;
			int i = 0;
			for (i = 0; i < vec.size(); i++) {
				cout << i << " ";
				if (vec[i] == searchInt) {
					*pageOffset = i;
					cout << "Number is expected to be at position " << i << endl; // Debug
					found = true;
					break;
				} else if (vec[i] > searchInt) {
					*pageOffset = i;
					cout << "Number is expected to be at position " << i << endl; // Debug
					break;
				}
			}
			if (i == vec.size()) {
				*pageOffset = i;
			}
			break;
		}
	}
	return found;
}
