//Sample file for students to get their code running

#include <iostream>
#include "file_manager.h"
#include "errors.h"
#include <cstring>
#include <bits/stdc++.h> 

using namespace std;


int main() {
	FileManager fm;

	// Create a brand new file
	FileHandler fh = fm.OpenFile("temp.txt");
	cout << "File created " << endl;

	// Create a new page
	PageHandler ph = fh.FirstPage ();
	char *data = ph.GetData ();
	// cout << ph.GetPageNum () << endl;

	int num = 0;

	int i = 0;
	// Output the first integer
	memcpy (&num, &data[i], sizeof(int));

	while(num != INT_MIN) {
		cout << "Number: " << num << endl;
		i += 4;
		memcpy (&num, &data[i], sizeof(int));
	}

	cout << i << endl;

	// Close the file and destory it
	fm.CloseFile (fh);
	// fm.DestroyFile ("temp.txt");
}
