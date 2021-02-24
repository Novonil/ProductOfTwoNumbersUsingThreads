Input Files:

A.txt :This contains the 100,000 digit number to be multiplied.
B.txt : This contains the 100,000 digit number with which we are multiplying

Output Files:
c1.txt : The single_thread.c file runs and stores the result in this file.
c2.txt : The Source.c file runs and stores the result in this file.
 
Compilation and execution:

cc Source.c -lpthread

cc single_thread -lpthread
For Source.c : 
./a.out A.txt B.txt 10

For single_thread.c :
./a.out A.txt B.txt 
---------------------------------------------------------------------------------------
The single_thread.c takes 90.67 seconds to run
The Source.c program with 10 threads takes 82.55 seconds to run
 
