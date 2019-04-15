#include <iostream>
#include <cstring>
#include "file_manager.h"
#include "errors.h"
#include <bits/stdc++.h> 
using namespace std;

bool issame(vector<int> * vec1, vector<int> *vec2){
    if(vec1->size()!=vec2->size()){
        return false;
    }else{
        for(int i=0;i<vec1->size();i++){
            if((*vec1)[i]!=(*vec2)[i]){
                return false;
            }
        }
    }
    return true;
}

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
    vector<int> vec1;
    while(true){
        try
        {
            ph = fh.PageAt(pagenum);
        }
        catch(const InvalidPageException& e)
        {
            // cout<<"file over"<<endl;
            break;
        }
        
        
        data=ph.GetData();
        while(true){
            memcpy(&num,&data[pageoffset*4],sizeof(int));
            if(num==INT_MIN){
                pagenum+=1;
                pageoffset=0;
                // cout<< endl <<"page over\n";
                break;
            }
            vec1.push_back(num);
            // cout <<num << " ";
            pageoffset+=1;
        }
        fh.UnpinPage(pagenum-1);
    }
    cout << "num pages is "<<pagenum<<endl;
    cout << "num elements is "<<vec1.size()<<endl;
    fm.CloseFile(fh);

    fh = fm.OpenFile(argv[2]);
    pagenum=0;
    pageoffset=0;
    num;
    i=0;
    vector<int> vec2;
    while(true){
        try
        {
            ph = fh.PageAt(pagenum);
        }
        catch(const InvalidPageException& e)
        {
            // cout<<"file over"<<endl;
            break;
        }
        
        
        data=ph.GetData();
        while(true){
            memcpy(&num,&data[pageoffset*4],sizeof(int));
            if(num==INT_MIN){
                pagenum+=1;
                pageoffset=0;
                // cout<< endl <<"page over\n";
                break;
            }
            vec2.push_back(num);
            // cout <<num << " ";
            pageoffset+=1;
        }
        fh.UnpinPage(pagenum-1);
    }
    cout << "num pages is "<<pagenum<<endl;
    cout << "num elements is "<<vec2.size()<<endl;
    fm.CloseFile(fh);
    sort(vec1.begin(),vec1.end());
    bool ans=issame(&vec1,&vec2);
    if(ans){
        cout << "same"<<endl;
    }else
    {
        cout << "different"<<endl;
    }
    
    
}