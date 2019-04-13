//Sample file for students to get their code running

#include <iostream>
#include <vector>
#include <cstring>
#include <climits>
#include <fstream>
#include <algorithm>
#include "file_manager.h"
#include "functions.h"
#include "errors.h"
using namespace std;

int insertSingle(int num, int startPageNum, FileHandler fh) {
	PageHandler ph = fh.LastPage();
	int lastPageNum = ph.GetPageNum();
	fh.UnpinPage(lastPageNum);

	int pageNum;
	int pageOffset;
	cout << "Searching in pages " << startPageNum << " to " << lastPageNum << endl; 
	bool found = binarySearchPage(num, fh, startPageNum, lastPageNum, &pageNum, &pageOffset);
	if (found)
		cout << "Found number in page number " << pageNum << endl;
	else
		cout << "Could not find number" << endl;

	int returnPageNum = pageNum;
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
	cout << "Read page data into vector" << endl;

	bool inserted = false;
	while(!inserted) {
		// Insert the number to the page
		int lastNumber = pageData[pageData.size()-1];
		memcpy(&data[pageOffset], &num, sizeof(int));
		// Update remaining page
		for(int i = pageOffset+1; i < pageData.size(); i++) {
			cout << pageData[i] << " ";
			memcpy(&data[i], &pageData[i-1], sizeof(int));
		}
		cout << endl << "Updated entire page" << endl;

		// Space available in page
		if(pageData.size() < PAGE_CONTENT_SIZE/4-1) {
			cout << "Space available in page" << endl;
			memcpy(&data[pageData.size()*4], &lastNumber, sizeof(int));
			int endData = INT_MIN;
			memcpy(&data[pageData.size()*4+4], &endData, sizeof(int));
			inserted = true;
			break;
		} else {
			// Insert lastNumber to next page
			if (pageNum != lastPageNum) {
				// Next page is available, mark the current page as dirty before going to next page
				cout << "Writing " << lastNumber << " in next page" << endl;
				fh.MarkDirty(pageNum);
				fh.UnpinPage(pageNum);
				ph = fh.NextPage(pageNum);
				pageNum = ph.GetPageNum();
				data = ph.GetData();
				num = lastNumber;
			} else {
				// Add a new page, mark it dirty and unpin it
				cout << "Adding a new page" << endl;
				ph = fh.NewPage();
				int newPageNum = ph.GetPageNum();
				fh.MarkDirty(newPageNum);
				fh.UnpinPage(newPageNum);
				memcpy(&data[0], &num, sizeof(int));
				int endData = INT_MIN;
				memcpy(&data[4], &endData, sizeof(int));
				inserted = true;
				break;
			}
		}
	}

	cout << "Returning page number " << returnPageNum << endl;
	return returnPageNum;
}

int main(int argc, const char* argv[]) {
	// Open the file to read integers from
	ifstream inputFile;
	inputFile.open(argv[1]);
	cout << "Input file for integers taken" << endl;

	// Open the given file
	FileManager fm;
	FileHandler fh = fm.OpenFile(argv[2]);
	cout << "File opened" << endl;

	int num;
	vector<int> numbers;
	while (inputFile >> num) {
		cout << "Read number " << num << endl;
		numbers.push_back(num);
	}
	sort(numbers.begin(), numbers.end());
	cout << "Sorted numbers" << endl;

	int lastFoundPage = fh.FirstPage().GetPageNum();
	cout << "First page number is " << lastFoundPage << endl;
	for (int i = 0; i < numbers.size(); i++) {
		cout << "Beginning insertion for " << numbers[i] << endl;
		lastFoundPage = insertSingle(numbers[i], lastFoundPage, fh);
		cout << numbers[i] << " was found on page number " << lastFoundPage << endl;
	}

	// Close the file and destroy it
	fm.CloseFile (fh);
}
