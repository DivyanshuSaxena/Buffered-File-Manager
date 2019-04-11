//Sample file for students to get their code running

#include<iostream>
#include "file_manager.h"
#include "errors.h"
#include<cstring>

using namespace std;


int main(int argc, const char* argv[]) {
	FileManager fm;

	// Create a brand new file
	FileHandler fh = fm.CreateFile(argv[1]);
	cout << "File created " << endl;

	// Create a new page
	PageHandler ph = fh.NewPage ();
	char *data = ph.GetData ();

	

	// Close the file and destory it
	fm.CloseFile (fh);
	fm.DestroyFile (argv[1]);
}
