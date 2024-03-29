#include <iostream>
#include <fstream>
#include "file_manager.h"
#include "errors.h"
#include <cstring>
#include <bits/stdc++.h>
#include <cmath>
#include <string>

using namespace std;

int generateRandomPoint(int K) {
	int random = ((int)rand()%K);
	return random;
}

int main(int argc, const char * argv[]) {
	FileManager fm;
	char* data;

	if(argc < 3) {
		cout << "Run format : ./generator <test file path> <sorted readable file path>" << endl;
		exit(1);
	}

	srand(time(0));

	int numPages = 0;
	int occupancy = 0;
	int occupancyLast = 0;
	int maxInt = 0;
	int sorted = 0;

	cout << "Enter numPages " << endl;
	cin >> numPages;
	cout << "Enter occupancyFactor as number of ints in a page: " << endl;
	cin >> occupancy;
	cout << "Enter occupancyFactor as number of ints in last page: " << endl;
	cin >> occupancyLast;
	cout << "Enter max value for ints generated: " << endl;
	cin >> maxInt;
	cout << "Enter whether you want sorted(1) or unsorted(0) file as int: " << endl;
	cin >> sorted;

	const char * testFile = argv[1];
	const char * writeFile = argv[2];

	FileHandler fh = fm.CreateFile(testFile);

	int n = (numPages - 1)*occupancy + occupancyLast;
	// int arr[n];
	int* arr = new int[n];

	for (int i = 0; i < n; i++) {
		arr[i] = generateRandomPoint(maxInt);
	}

	if(sorted == 1)
		sort(arr, arr+n);

	int pageNum = 0;
	int num = 0;
	int l = INT_MIN; 
	while(pageNum != numPages) {

		PageHandler ph = fh.NewPage ();
		char *data = ph.GetData ();
		
		if(pageNum == numPages - 1) {
			memcpy(&data[0], &arr[pageNum*occupancy], sizeof(int)*occupancyLast);
			memcpy(&data[occupancyLast*4], &l, sizeof(int));
		}
		else {
			memcpy(&data[0], &arr[pageNum*occupancy], sizeof(int)*occupancy);
			memcpy(&data[occupancy*4], &l, sizeof(int));
		}
		fh.FlushPage(ph.GetPageNum());
		pageNum++;
	}

	ofstream fs;
	fs.open(writeFile);

	for (int i = 0; i < n; i++) {
		if (i > (numPages - 1)*occupancy) {
			if (i % occupancyLast == 0)
				fs << "page over" << endl;
		} else if (i % occupancy == 0 && i != 0) {
			fs << "page over" << endl;
		}
		fs << arr[i] << endl;
	}
	fs << "page over" << endl;
	fs << "file over" << endl;

	fm.CloseFile(fh);
}
