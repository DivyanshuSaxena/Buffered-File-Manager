//Sample file for students to get their code running

#include <iostream>
#include <vector>
#include <cstring>
#include <climits>
#include <fstream>
#include "file_manager.h"
#include "binary_search.h"
#include "errors.h"
using namespace std;

int insertSingle(int num, FileHandler fh) {
	PageHandler ph = fh.FirstPage();
	int startPageNum = ph.GetPageNum();
	fh.UnpinPage(startPageNum);

	ph = fh.LastPage();
	int lastPageNum = ph.GetPageNum();
	fh.UnpinPage(lastPageNum);

	int pageNum;
	int pageOffset;
	binarySearchPage(num, fh, startPageNum, lastPageNum, &pageNum, &pageOffset);

	ph = fh.PageAt(pageNum);
	char* data = ph.GetData();
	vector<int> pageData; 

	// Get data from the page
	for(int i = 0; i < PAGE_CONTENT_SIZE/4; i++) {
		int number;
		memcpy(&number, &data[i*4], sizeof(int));
		if(number == INT_MIN) {
			// cout<< "end of page\n";
			break;
		}
		// cout <<num<<endl;
		pageData.push_back(number);
	}

	bool inserted = false;
	while(!inserted) {
		// Space available in page
		if(pageData.size() < PAGE_CONTENT_SIZE/4-1) {
			memcpy(&data[pageData.size()*4], &num, sizeof(int));
			inserted = true;
			break;
		} else {
			int lastNumber = pageData[pageData.size()-1];
			bool update = false;
			// Insert the number to the page
			for(int i = 0; i < PAGE_CONTENT_SIZE/4; i++) {
				if (!update && pageData[i] >= num) {
					memcpy(&data[i], &num, sizeof(int));
					update = true;
					continue;
				}
				if (update) {
					memcpy(&data[i], &pageData[i-1], sizeof(int));
				}
			}
			// Insert lastNumber to next page
			if (pageNum != lastPageNum) {
				// Next page is available
				ph = fh.NextPage(pageNum);
				data = ph.GetData();
				num = lastNumber;
			} else {
				// Add a new page
				ph = fh.NewPage();
				memcpy(&data[0], &num, sizeof(int));
				int endData = INT_MIN;
				memcpy(&data[4], &endData, sizeof(int));
				inserted = true;
				break;
			}
		}
	}
}

int main(int argc, const char* argv[]) {
	// Open the file to read integers from
	ifstream inputFile;
	inputFile.open(argv[1]);

	// Open the given file
	FileManager fm;
	FileHandler fh = fm.OpenFile(argv[2]);
	cout << "File opened" << endl;

	int num;
	while (inputFile >> num) {
		insertSingle(num, fh);
	}

	// Close the file and destory it
	fm.CloseFile (fh);
}
