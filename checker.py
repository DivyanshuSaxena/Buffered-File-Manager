import sys

check_type = sys.argv[1]
input_file = sys.argv[2]
output_file = sys.argv[3]

if check_type == 0:
    integer_file = sys.argv[4]
    
    # Check insertion code
    pages = []
    integers = []
    with open(input_file) as f:
        content = f.readlines()
    content = [x.strip() for x in content]

    page = []
    for c in content:
        if c == "page over":
            pages.append(page)
            page = []
        elif c == "file over":
            break
        else:
            page.append(int(c))
    print (pages)

    with open(integer_file) as f:
        content = f.readlines()
    integers = [x.strip() for x in content]
    
    integer_pointer = 0
    for page in pages:
        for p in range(len(page)):
            if integers[integer_pointer] <= page[p]:
                page.insert(p, integers[integer_pointer])
                integer_pointer += 1

    # Pages now holds inserted array
    print (pages)

    output_pages = []
    with open(output_file) as f:
        content = f.readlines()
    content = [x.strip() for x in content]

    output_page = []
    for c in content:
        if c == "page over":
            output_pages.append(page)
            output_page = []
        elif c == "file over":
            break
        else:
            output_page.append(int(c))
    print (output_pages)

    problem = False
    problem_page = 0
    problem_index = 0
    for page_index in range(len(pages)):
        for data_index in range(len(pages[page_index])):
            if pages[page_index][data_index] != output_pages[page_index][data_index]:
                problem = True
                problem_page = page_index
                problem_index = data_index

    if problem:
        print ("Insertion worked good")
    else:
        print ("Problem in insertion at page {0} at index {1}".format(problem_page, problem_index))

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
