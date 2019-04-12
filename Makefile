sampleObjects = buffer_manager.o file_manager.o sample_run.o
binaryObjects = buffer_manager.o file_manager.o binary_search.o
insertionObjects = buffer_manager.o file_manager.o insertion.o
mergeObjects = buffer_manager.o file_manager.o merge_sort.o
generatorObjects = buffer_manager.o file_manager.o generator.o

sample_run : $(sampleObjects)
	     g++ -std=c++11 -o sample_run $(sampleObjects)

binary_search : $(binaryObjects)
		 g++ -std=c++11 -o binary_search $(binaryObjects)

insertion : $(insertionObjects)
		 g++ -std=c++11 -o insertion $(insertionObjects)

generator : $(generatorObjects)
		 g++ -std=c++11 -o generator $(generatorObjects)

merge_sort : $(mergeObjects)
		 g++ -std=c++11 -o merge_sort $(mergeObjects)

binary_search.o : binary_search.cpp
	g++ -std=c++11 -c binary_search.cpp

insertion.o : insertion.cpp
	g++ -std=c++11 -c insertion.cpp

generator.o : generator.cpp
	g++ -std=c++11 -c generator.cpp

merge_sort.o : merge_sort.cpp
	g++ -std=c++11 -c merge_sort.cpp

sample_run.o : sample_run.cpp
	g++ -std=c++11 -c sample_run.cpp

buffer_manager.o : buffer_manager.cpp
	g++ -std=c++11 -c buffer_manager.cpp

file_manager.o : file_manager.cpp
	g++ -std=c++11 -c file_manager.cpp

clean :
	rm -f *.o
	rm -f sample_run binary_search insertion merge_sort generator


