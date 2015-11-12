#pragma once

class FileDirectory
{

private:
	unsigned char fileDirectory[4][32];
	unsigned short int FAT16[20];
public:
	FileDirectory();
	bool create(char filename[], int numberBytes);
	bool deletee(char filename[]);
	bool read(char filename[]);
	bool write(char filename[], int numberBytes, char data[], 
		int year, int month, int day, int hour, int minute,
		int second);
	void printClusters(char filename[]);
	void printDirectory();


	
};