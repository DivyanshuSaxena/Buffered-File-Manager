#include<iostream>
#include "file_manager.h"
#include "errors.h"
#include<cstring>
#include <bits/stdc++.h> 
using namespace std;


int main(int argc, const char* argv[]) {
	FileManager fm;
	FileHandler fh;
	PageHandler ph;
	char*data;


    fh = fm.OpenFile(argv[1]);
    int pagenum=0;
    int pageoffset=0;
    int num;
    int i=0;
    vector<int> vec;
    while(true){
        try
        {
            ph = fh.PageAt(pagenum);
        }
        catch(const std::exception& e)
        {
            cout<<"file over"<<endl;
            break;
        }
        
        
        data=ph.GetData();
        while(true){
            memcpy(&num,&data[pageoffset*4],sizeof(int));
            vec.push_back(num);
            if(num==INT_MIN){
                pagenum+=1;
                pageoffset=0;
                cout<< endl <<"page over\n";
                break;
            }
            cout <<num << " ";
            pageoffset+=1;
        }
    }
    
}