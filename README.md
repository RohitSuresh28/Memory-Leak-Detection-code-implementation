# Memory-Leak-Detection-code-implementation
A research project, which is a comparative study based on advanced methodologies used to detect memory leaks in software programs. We have gone through research papers, and since we were not able to procure a large number of research on this topic, we've decided to come up with an extensive study on the same

We have currently looked into the following tools:

Valgrind
GDB Debugger
sar
abrt
Electric Fence
mtrace
Memleax
Leak Sanitizer

We also have a simple code implementation for memory leak detection.

In the market, there are a lot of tools for detecting memory leaks. Here I made a program which has to be included in your source file and it will tell the desired information to find where memory leaks happen. 
There are two files: findLeak.c and findLeak.h. The source file is test.c, this is the program which you want to check memory leaks. 
The concept is that when the user calls the library function "malloc or calloc " for allocating dynamic memory, then we have to call our malloc or calloc (In case of my source code 'MyCalloc' function in place of 'calloc' and MyMalloc function in place of 'malloc'). So we have to define malloc to your malloc and in the .c file you have to undef malloc and calloc By this way we can call our MyMalloc or MyCalloc function and in this function, we will call another function which keeps all info about how much memory allocated. We will do the same for the Library function "free" to deallocate memory( In my source file "MyFree" is used in place of the free function).
