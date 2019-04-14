#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include<iostream>
#include "file_manager.h"
#include "errors.h"
#include<cstring>
#include <bits/stdc++.h> 
using namespace std;

bool binarySearchPage(int searchInt, int startPageNum, int lastPageNum, int firstPageNum, int endPageNum, FileHandler fh, int * finPage, int * pageOffset);

#endif