## CS440/540 - Programming Assignment 2 - Map Container

#### Please find detailed description about assignment at below link:

* http://www.cs.binghamton.edu/~kchiu/cs540/prog/2/

#### Please add your comments below in case your implementation is not fully working:

I found red-black trees too difficult to implement, so I have used implementation from online: https://www.geeksforgeeks.org/red-black-tree-set-3-delete-2/

I haven't managed to remove all the leaks. My implementation does not work for cases where assignment operator in Key_T and Mapped_T is deleted. I have implemented given types if Iterators and Map features. Tests are written in the function other_tests() in test.cpp

all files other than minimal.cpp and test-kec.cpp work
