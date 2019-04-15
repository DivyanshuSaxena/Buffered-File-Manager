#!/bin/bash

insert_test_dir = "./testcases/insertion/"
merge_test_dir = "./testcases/merge_sort/"

if [[ $1 == "insert" ]]; then
    input_file = $insert_test_dir.$2
    readable_input = $insert_test_dir.$3
    output_file = $readable_input."_insert"
    integer_file = $insert_test_dir.$4
    ./insertion $input_file $integer_file
    ./reader $input_file > $output_file
    python checker.py 0 $readable_input $output_file $integer_file
elif [[ $1 == "merge" ]]; then
    input_file = $merge_test_dir.$2
    readable_input = $merge_test_dir.$3
    output_file = $input_file."_merge"
    readable_output = $readable_input."_merge"
    ./merge_sort $input_file $output_file
    ./reader $output_file > $readable_output
    python checker.py 1 $readable_input $readable_output    
fi