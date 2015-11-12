#include "stdafx.h"
#include "FileDirectory.h"
#define EOF 0xffff
#include <iostream>
using namespace std;

FileDirectory::FileDirectory()
{
	//to initialize all entries in the fileDirectory and FAT16 to be 0, i.e. safe values.
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 32; j++)
			fileDirectory[i][j] = 0;
	for (int i = 0; i < 20; i++)
		FAT16[i] = 0;
};

bool FileDirectory::create(char filename[], int numberBytes)
{
	/*
	(1)	to check if there is an unused entry in the File Directory,  (i.e.  the first character of the file name in the File Directory is zero). Return false if not true.
	(2)	to check if there are enough unused clusters to store the file with the numberBytes. Return false if not true.
	*/
	int i=0;
	int count = 0;
	int checksize = 0;
	for (i = 0; i < 4; i++)
	{
		
		if (fileDirectory[i][1] == 0)
		{
			break;
		}
	}
	if (i == 4)
	{
		cout << "There are no unused entries in the File Directory\n";
		return false;
	}
	for (i = 1; i < 16; i++)
	{
		if (FAT16[i] == 0)
			count++;
	}
	checksize = (count * 512) - numberBytes;
	if (checksize < 0)
	{
		cout << "Not enough free data for " << filename << endl;
		return false;
	}
	return true;
};

bool FileDirectory::deletee(char filename[])
{
	/*
	(0)	to check if the file to be deleted, filename[], is in the Directory. If not, return false.
	(1)	to change the first character of the file name in the File Directory to be zero, 
	(2)	to change all entries of the clusters of this file in the FAT to 1, i.e., unused.
	*/
	int j = 0;
	int i = 0;
	unsigned short int clustadd;
	
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (fileDirectory[i][j] != filename[j])
			{
				break;
			}
			else if (j == 4)
			{
				goto deletefile;
				
			}

		}
		if (i == 3)
			return false;
	}
deletefile:
	clustadd = fileDirectory[i][30] * 256 + fileDirectory[i][31];
	for (int k = 0; k < 32; k++)
		fileDirectory[i][k] = 0;
	for (int aa = clustadd; aa<20; aa++)
	{
		if (FAT16[aa] == EOF)
		{
			FAT16[aa] = 0;
			break;
		}
		FAT16[aa] = 0;
		
	}



}

bool FileDirectory::read(char filename[])
{
	/*
	purpose: to read  of data from data[] array from the file with the specified file name.
	(0)	to check if the file to be deleted, filename[], is in the Directory. If not, return false.
	(1)	use the file name to get the file information from the File Directory, including date, time, number of bytes and the first cluster address,
	(2)	use the first cluster address to get all the cluster addresses of this file from the FAT-16,
	(3)	use all the cluster addresses to read the data from the disk/flash memory.
	*/
	int i, j;
	unsigned short int firstSector, sectors[16];
	bool found = false;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (filename[j] != fileDirectory[i][j])
				break;
		}
		if (j == 8)
		{
			found = true;
			break;
		}
	}
	
	if (found == false)
		return found;

	firstSector = fileDirectory[i][30]*256 + fileDirectory[i][31]; //multiply by 256 to shift <- 8

	i = 0;
	sectors[i] = firstSector;
	while (FAT16[sectors[i]] != EOF)//0xffff == EOF
	{
		sectors[i+1] = FAT16[sectors[i]];
		i++;
	}

	return found;
}

bool FileDirectory::write(char filename[], int numberBytes, char data[],
	int year, int month, int day, int hour, int minute,
	int second)
{
	/*
	purpose: to write numberBytes bytes of data from data[] array into the file with the specified file name.
	(1)	to look for the first unused entry (0 or 1) in the FAT-16, and use it as the First Cluster Address.
	(2)	to look for the next unused entry (0 or 1) in the FAT-16, and use it as the Next Cluster Address, and write its value into the FAT-16.
	(3)	Repeat Step 2 until all clusters are found and the FAT-16 is updated.
	(4)	to write/update the file name, extension, date, time, file length and first cluster address into the first unused entry in the File Directory,
	*/
	int i, j;
	for (i=0;i<4;i++)
	{
		if (fileDirectory[i][1]==0)
		{
				for (j=0;j<8;j++)
					fileDirectory[i][j]=filename[j];
				break;
		}
		
	}
	if (i==4)
		return false;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (filename[j] != fileDirectory[i][j])
			{
				
				break;
				
			}
		}
		if (j == 8)
		{
			break;
		}
	}
	
	int filenum = i;
	_int8 a=(numberBytes & 0x00FF);
	_int8 b = numberBytes>>8;
	fileDirectory[filenum][15]=a;
	fileDirectory[filenum][16]=b;
	
	
	//look for first unused entry (0 or 1) in the FAT16
	for (int i = 1; i < 20; i++)
	{
		if (FAT16[i] == 0)
		{
			fileDirectory[filenum][31] = i;
			break;
		}
	}

	numberBytes = numberBytes-512;
	int z;
	int x=0;
	int temp;
	int temppp;
	int count = 0;
	for (z =1; z<20; z++)
	{
		if(FAT16[z]==0)
		{
			FAT16[z]=55; //show that it is being used
			temp = z;
			if (count > 0)
			{
				FAT16[temppp] = temp;
			}
			count++;
			if(numberBytes<=0)
			{
				FAT16[z]=EOF;
				break;
			}
			else if (numberBytes>0)
			{
				for (x = 1; x<20; x++)
				{
					if (FAT16[x]==55)
					{
						temppp = z;
						numberBytes=numberBytes-512;
						break;
					}
				}
			}
		}
		if(numberBytes<=0)
		{
			FAT16[z]=z;
		}

	}
	//set date and time
	year = year-1980;
	fileDirectory[filenum][11]=(month<<5)+day;
	fileDirectory[filenum][12]=(month>>3)+(year<<1);
	fileDirectory[filenum][13]=(minute<<5)+(second/2);
	fileDirectory[filenum][14]=(hour<<3)+(minute>>3);


	return true;
}

void FileDirectory::printClusters(char filename[])
{
	/*
	purpose: to print all the clusters of a file.
	(1)	to check if the file to be printed, filename[], is in the Directory. If not, return false.
	(2)	use the file name to get the file information from the File Directory, including the first cluster address,
	(3)	use the first cluster address to get all cluster addresses from the FAT-16,
	*/
	int j = 0;
	int i = 0;
	unsigned short int clustadd;
	
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (fileDirectory[i][j] != filename[j])
			{
				break;
			}
			else if (j == 4)
			{
				goto printclust;
			}
		}
	}
printclust:
	if (i != 4)
	{
		cout << filename << ":  ";
		clustadd = fileDirectory[i][30] * 256 + fileDirectory[i][31];
		cout << clustadd;
		int tempp;
		int count = 0;
		while (true)
		{
			if (count == 0)
				tempp = FAT16[clustadd];
			if (count > 0)
			{
				tempp = FAT16[tempp];
				if (tempp == EOF)
				{
					cout << endl;
					break;
				}
			}
			cout << " -> " << tempp;
			count++;
		}
		cout << endl << endl;
	}
	if (i == 4)
		cout << "\n" << filename << " does not exist within the fileDirectory\n\n";
}

void FileDirectory::printDirectory()
{
	/*
	(1)	use the file name to get the file information from the File Directory, including the first cluster address,
	(2)	use the first cluster address to get all cluster addresses from the FAT-16,
	*/
	unsigned short int date;
	unsigned short int time;
	unsigned int filelength;
	unsigned short int clustadd;
	
	cout << "\n\n***File System***\n\n"; 
	for (int i = 0; i < 4; i++)
	{
		if (fileDirectory[i][0] != 0)
		{
			for (int j = 0; j < 8; j++)
			{
				//print the file name to get the file info from the File Directory
				cout << fileDirectory[i][j];
			}
			cout << ".";
			//print file extension
			for (int j = 8; j < 11; j++)
			{
				if (fileDirectory[i][j]!=0)
					cout << fileDirectory[i][j];
				
			}
			clustadd=fileDirectory[i][30]*256 + fileDirectory[i][31];
			filelength = fileDirectory[i][15]+fileDirectory[i][16]*256; //set file length
			cout << "    ";
			date = fileDirectory[i][12] * 256 + fileDirectory[i][11]; //set date
			cout << " " << (date & 0x01e0) / 32;	//print month
			cout << '/' << (date & 0x001f) << '/'; //print day
			cout << date / 512 + 1980; // print year
			time = fileDirectory[i][14] * 256 + fileDirectory[i][13]; //set time
			cout << " " << time / 2048 << ':' << (time & 0x07e0) / 32 << ':' << (time & 0x001f) *2; //print time
			cout << "    " << filelength; //print file length
			cout << "  " << clustadd;
			cout << "    \n" ;
			cout << endl << endl;
		}
		
	}
	
}