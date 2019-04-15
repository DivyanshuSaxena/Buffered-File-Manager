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

bool debugprintinsert=false;

int insertSingle(int num, int startPageNum, FileHandler* fh, FileManager* fm) {
	PageHandler ph = fh->FirstPage();
	int firstPageNum = ph.GetPageNum();
	fh->FlushPage(firstPageNum);
	
	ph = fh->LastPage();
	int lastPageNum = ph.GetPageNum();
	fh->FlushPage(lastPageNum);

	int pageNum;
	int pageOffset;
	if(debugprintinsert) cout << "Searching in pages " << startPageNum << " to " << lastPageNum << endl; 
	bool found = binarySearchPage(num, startPageNum, lastPageNum, firstPageNum, lastPageNum, *fh, &pageNum, &pageOffset);
	if (found)
		if(debugprintinsert) cout << "Found number in page number " << pageNum << " at offset " << pageOffset << endl;
	else
		if(debugprintinsert) cout << "Insert number in pagenum " << pageNum << " at offset " << pageOffset << endl;

	int returnPageNum = pageNum;
	ph = fh->PageAt(pageNum);
	char* data = ph.GetData();

	bool inserted = false;
	while(!inserted) {
		vector<int> pageData; 
		// Get data from the page
		for(int i = 0; i < PAGE_CONTENT_SIZE/4; i++) {
			int number;
			memcpy(&number, &data[i*4], sizeof(int));
			if(number == INT_MIN) {
				// if(debugprintinsert) cout<< "end of page\n";
				break;
			}
			// if(debugprintinsert) cout <<num<<endl;
			pageData.push_back(number);
		}
		if(debugprintinsert) cout << "Read data into vector with size " << pageData.size() << endl;

		// Insert the number to the page
		int lastNumber = pageData[pageData.size()-1];
		int entryNum = num;
		if (entryNum > lastNumber) {
			// Swap if the incoming number is larger than the last element of the page
			entryNum = lastNumber;
			lastNumber = num;
		}
		memcpy(&data[pageOffset*4], &entryNum, sizeof(int));		

		// Update remaining page
		for(int i = pageOffset+1; i < pageData.size(); i++) {
			if(debugprintinsert) cout << pageData[i-2] << " ";
			int newEntryNum = pageData[i-1];
			memcpy(&data[i*4], &newEntryNum, sizeof(int));
		}
		if(debugprintinsert) cout << endl << "Updated entire page" << endl;

		// Space available in page
		if(pageData.size() < PAGE_CONTENT_SIZE/4-1) {
			if(debugprintinsert) cout << "Space available in page" << endl;
			memcpy(&data[pageData.size()*4], &lastNumber, sizeof(int));
			int endData = INT_MIN;
			memcpy(&data[pageData.size()*4+4], &endData, sizeof(int));
			fh->MarkDirty(pageNum);
			fh->FlushPage(pageNum);

			inserted = true;
			break;
		} else {
			// Insert lastNumber to next page
			if (pageNum != lastPageNum) {
				// Next page is available, mark the current page as dirty before going to next page
				int endData = INT_MIN;
				memcpy(&data[pageData.size()*4], &endData, sizeof(int));
				fh->MarkDirty(pageNum);
				fh->FlushPage(pageNum);

				if(debugprintinsert) cout << "Writing " << lastNumber << " in next page" << endl;
				ph = fh->NextPage(pageNum);
				pageNum = ph.GetPageNum();
				data = ph.GetData();
				pageOffset = 0;
				num = lastNumber;
			} else {
				// Add a new page, mark it dirty and unpin it
				int endData = INT_MIN;
				memcpy(&data[pageData.size()*4], &endData, sizeof(int));
				fh->MarkDirty(pageNum);
				fh->FlushPage(pageNum);

				if(debugprintinsert) cout << "Adding a new page" << endl;
				ph = fh->NewPage();
				data = ph.GetData();
				int newPageNum = ph.GetPageNum();
				memcpy(&data[0], &lastNumber, sizeof(int));
				memcpy(&data[4], &endData, sizeof(int));
				fh->MarkDirty(newPageNum);
				fh->FlushPage(newPageNum);

				inserted = true;
				break;
			}
		}
	}

	if(debugprintinsert) cout << "Returning page number " << returnPageNum << endl;
	return returnPageNum;
}

int main(int argc, const char* argv[]) {
	// Open the file to read integers from
	ifstream inputFile;
	inputFile.open(argv[2]);
	if(debugprintinsert) cout << "Input file for integers taken" << endl;

	// Open the given file
	FileManager fm;
	FileHandler fh = fm.OpenFile(argv[1]);
	if(debugprintinsert) cout << "File opened" << endl;
	
	int num;
	vector<int> numbers;
	while (inputFile >> num) {
		if(debugprintinsert) cout << "Read number " << num << endl;
		numbers.push_back(num);
	}
	sort(numbers.begin(), numbers.end());
	if(debugprintinsert) cout << "Sorted numbers" << endl;

	int lastFoundPage = fh.FirstPage().GetPageNum();
	if(debugprintinsert) cout << "First page number is " << lastFoundPage << endl;
	for (int i = 0; i < numbers.size(); i++) {
		if(debugprintinsert) cout << "Beginning insertion for " << numbers[i] << endl;
		if(debugprintinsert) cout << "Last found page is "<<lastFoundPage<<endl;
		lastFoundPage = insertSingle(numbers[i], lastFoundPage, &fh, &fm);
		if(debugprintinsert) cout << numbers[i] << " was found on page number " << lastFoundPage << endl;
	}

	// Flush the pages, close the file and destroy it
	// fh.FlushPages();
	fm.CloseFile (fh);
}
