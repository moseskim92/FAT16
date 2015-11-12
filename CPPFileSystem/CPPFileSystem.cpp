// CPPFileSystem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileDirectory.h"

int _tmain(int argc, _TCHAR* argv[])
{
	
	//Write a driver function, i.e.main(), to test this program by
	FileDirectory fat1;
	char data[1024];
	unsigned short int a = 2015;
	//1.	create and write a file, file1, of 1000 bytes,
	if(fat1.create("file1", 1000))
		fat1.write("file1", 1000, data, 2015, 5, 27, 9, 42, 58);
	fat1.printDirectory();
	fat1.printClusters("file1");

	//2.	create and write a file, file2, of 2000 bytes,
	if (fat1.create("file2", 2000))
		fat1.write("file2", 2000, data, 2015, 6, 22, 1, 32, 54);
	fat1.printDirectory();
	fat1.printClusters("file2");

	//3.	create and write a file, file3, of 3000 bytes,
	if (fat1.create("file3", 3000))
		fat1.write("file3", 3000, data, 2015, 2, 17, 9, 34, 22);
	fat1.printDirectory();
	fat1.printClusters("file3");

	//4.	create and write a file, file4, of 4000 bytes.
	if (fat1.create("file4", 4000))
		fat1.write("file4", 4000, data, 2015, 3, 13, 12, 22, 44);
	fat1.printDirectory();
	fat1.printClusters("file4");

	//5.	delete file2,
	fat1.deletee("file2");
	fat1.printDirectory();

	//6.	create and write a file, file4, of 4000 bytes.
	if (fat1.create("file4", 4000))
		fat1.write("file4", 4000, data, 2015, 5, 27, 9, 42, 59);
	fat1.printDirectory();
	fat1.printClusters("file4");

	if (fat1.create("file5", 2500))
		fat1.write("file5", 2500, data, 2014, 3, 12, 2, 35, 32);
	fat1.printDirectory();
	fat1.printClusters("file5");
	return 0;
}

