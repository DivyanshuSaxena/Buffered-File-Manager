import sys

check_type = int(sys.argv[1])
input_file = sys.argv[2]
output_file = sys.argv[3]

if check_type == 0:
    integer_file = sys.argv[4]
    
    # Check insertion code
    input_arr = []
    integers = []
    with open(input_file) as f:
        content = f.readlines()
    content = [x.strip() for x in content]

    for c in content:
        if c != "page over" and c != "file over":
            input_arr.append(int(c))
        if c == "file over":
            break

    with open(integer_file) as f:
        content = f.readlines()
    integers = [int(x.strip()) for x in content]
    integers.sort()
    
    integer_pointer = 0
    for p in range(len(input_arr)):
        if integers[integer_pointer] <= input_arr[p]:
            input_arr.insert(p, integers[integer_pointer])
            integer_pointer += 1

    # Pages now holds inserted array
    # print ("Desired File: ")
    # print (input_arr)

    output_arr = []
    with open(output_file) as f:
        content = f.readlines()
    content = [x.strip() for x in content]
    
    for c in content:
        if c != "page over" and c != "file over":
            output_arr.append(int(c))
        if c == "file over":
            break
    # print ("Output File: ")
    # print (output_arr)

    problem = False
    problem_index = 0
    for index in range(len(input_arr)):
        if input_arr[index] != output_arr[index]:
            print ("{0}".format(index))
            problem = True
            problem_index = index

    if not problem:
        print ("Insertion worked good")
    else:
        print ("Problem in insertion at index {0}".format(problem_index))

else:
    # Check merge sort code
    print ("Checking merge sort {0} with {1}".format(input_file, output_file))
    array = []
    with open(input_file) as f:
        content = f.readlines()
    content = [x.strip() for x in content]

    for c in content:
        if c != "page over" and c != "file over":
            array.append(int(c))
    array.sort()
    print (array)

    with open(output_file) as f:
        content = f.readlines()
    content = [x.strip() for x in content]

    output_array = []
    for c in content:
        if c != "page over" and c != "file over":
            output_array.append(int(c))
        if c == "file over":
            break
    print (output_array)

    problem = False
    for index in range(len(array)):
        if array[index] != output_array[index]:
            problem = True
            print ("Expected {0}, found {1}".format(array[index], output_array[index]))

    if not problem:
        print ("Success!")
