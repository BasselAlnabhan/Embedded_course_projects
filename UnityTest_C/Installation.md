 To install unity

If you use Windows
Add C:\Program Files\Git\bin to the environment variables
Remove the installed MingW on your PC and download mingw-setup.zip, extract and install it.
Download unity.zip and extract it
Inside the extracted folder there are some source(.c) and header(.h) files
Inside the extracted folder run git bash
Run gcc -c unity.c -o unity.o
Run gcc -c unity_fixture.c -o unity_fixture.o
Run gcc -c unity_memory.c -o unity_memory.o 
Run ar crv libunity.a unity.o unity_fixture.o unity_memory.o 
Remove all the object(.o) and source(.c) files. Now you have only some header(.h) files and libunity.a
Copy libunity.a to the default libraries path of MinGW; e.g. path/to/MinGW/lib/
Copy all the header(.h) files to the default include path of MinGW; e.g. path/to/MinGW/include/
 

To use unity in your program

You can include unity.h like a standard library (#include <unity.h>)
In order to compile your program with unity you should add -lunity as an agument to gcc command. Like gcc main.c -lunity -o main
