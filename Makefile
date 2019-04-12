sampleObjects = buffer_manager.o file_manager.o sample_run.o
binaryObjects = buffer_manager.o file_manager.o binary_search.o
insertionObjects = buffer_manager.o file_manager.o insertion.o
mergeObjects = buffer_manager.o file_manager.o merge_sort.o
debugObjects = buffer_manager.o file_manager.o debug.o

sample_run : $(sampleObjects)
	     g++ -std=c++11 -o sample_run $(sampleObjects)

binary_search : $(binaryObjects)
		 g++ -std=c++11 -o binary_search $(binaryObjects)

insertion : $(insertionObjects)
		 g++ -std=c++11 -o insertion $(insertionObjects)

merge_sort : $(mergeObjects)
		 g++ -std=c++11 -o merge_sort $(mergeObjects)

debug : $(debugObjects)
	     g++ -std=c++11 -o debug $(debugObjects)

binary_search.o : binary_search.cpp
	g++ -std=c++11 -c binary_search.cpp

insertion.o : insertion.cpp
	g++ -std=c++11 -c insertion.cpp

merge_sort.o : merge_sort.cpp
	g++ -std=c++11 -c merge_sort.cpp

sample_run.o : sample_run.cpp
	g++ -std=c++11 -c sample_run.cpp

debug.o : debug.cpp
	g++ -std=c++11 -c debug.cpp

buffer_manager.o : buffer_manager.cpp
	g++ -std=c++11 -c buffer_manager.cpp

file_manager.o : file_manager.cpp
	g++ -std=c++11 -c file_manager.cpp

clean :
	rm -f *.o
	rm -f sample_run binary_search insertion merge_sort debug

