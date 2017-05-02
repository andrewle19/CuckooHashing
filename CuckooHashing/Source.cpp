/*

Cuckoo Hashing Algorithm
Author: Andrew Le
email: andrewle19@csu.fullerton.edu
5/1/17

Open hashing method of Cuckoo Hashing
INPUT: an input file containg strings of characters, one string per line
OUTPUT: a detailed list of wherew the string are inserted into hash table


*/


#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
using namespace std;


// cuckoo tables' size                                                        
const int tablesize = 17;

// two dimensional arrary that represents two hash tables of char arrarys     
char  hashtable[tablesize][2][255];



// compute the hash functions
// s is the string (the key) to which we apply the hash function
// index indicates which hash function will be used
// index == 0 means the first hash function
// index == 1 means the second hash function
size_t hashs(char *s, size_t index) {
	
	size_t po; 
	int len;
	int i, val, temp;
	
	
	po = 1; // start position at 1
	len = strlen(s); // length of string

	// hash function one for table one
	if (index == 0) {

		// compute the value of the hash at index 0
		val = s[0];
		val = val % tablesize;

		// if the hash value is negative add the table size
		if (val < 0)
		{
			val += tablesize;
		}
		// return value if the string was only one letter
		if (len == 1)
		{
			return val;
		}
		// loop through rest of string computing the hash value
		for (i = 1; i < len; i++)
		{
			temp = s[i];
			po *= 31;

			po = po % tablesize;
			// if position hash was negative
			if (po < 0)
			{
				po += tablesize;
			}

			// add the postioin hash to hash value then hash that
			val += temp * po;
			val = val % tablesize;

			// if the hash was negative add hash
			if (val < 0)
			{
				val += tablesize;
			}
		}
		return val;
	}

	// the second hash function for the second table
	else {
		// compute the value of the hash at index 0
		val = s[len-1];
		val = val % tablesize;

		// if the hash value is negative add the table size
		if (val < 0)
		{
			val += tablesize;
		}
		// return value if the string was only one letter
		if (len == 1)
		{
			return val;
		}
		// loop through rest of string computing the hash value
		for (i = 1; i < len; i++)
		{
			temp = s[len-i-1];
			po *= 31;

			po = po % tablesize;
			// if position hash was negative
			if (po < 0)
			{
				po += tablesize;
			}

			// add the postioin hash to hash value then hash that
			val += temp * po;
			val = val % tablesize;

			// if the hash was negative add hash
			if (val < 0)
			{
				val += tablesize;
			}
		}
		return val;
	}
}



// place a string in one of the hash tables
bool place_in_hash_tables(char *s) {

	bool placed; // if the string was placed in the table
	size_t pos;
	int index;
	char temp_s[255], temp[255];

	// copy the string s into the temp char array
	strcpy(temp_s, s);
	// use a counter to detect loops
	int counter = 0;
	// start with table T1
	index = 0;
	placed = false;
	pos = hashs(temp_s, index);

	while ((!placed) && (counter < 2 * tablesize)) {

		if (strcmp(hashtable[pos][index], "") == 0) {
			// the entry at index <pos> in the <index> hash table is available so store the string <temp_s> there
			cout << "String <" << temp_s << "> will be placed at";
			cout << " t[" << pos << "][" << index << "]" << endl;
			strcpy(hashtable[pos][index], temp_s);
			placed = true;
			return placed;
		}
		else {
			// the entry at index <pos> in the <index> hash table is not available so 
			// obtain the string stored over there in variable <temp> and store the string <temp_s> there
			// now the string <temp> needs to be placed in the other table
			cout << "String <" << temp_s << "> will be placed at" << " t[" << pos;
			cout << "][" << index << "]" << " replacing <" << hashtable[pos][index] << ">";
			cout << endl;

			strcpy(temp, hashtable[pos][index]); // copy old string into temp
			strcpy(hashtable[pos][index], temp_s); // copy new string into hashtable
			strcpy(temp_s, temp); // copy old string into temp_s\

			// changes table depnding on where the last table index
			if (index == 0)
			{
				index = 1;
				pos = hashs(temp_s, index); // rehash
			}
			else
			{
				index = 0;
				pos = hashs(temp_s, index); // rehash
			}

			counter++;
		}
	}
	return placed;
};


int main() {

	// the strings to be stored in the hash tables
	char s[255] = "";
	char null_st[] = "";
	size_t i, len;
	bool placed;

	// clear the tables
	for (i = 0; i< tablesize; i++) {
		strcpy(hashtable[i][0], null_st);
		strcpy(hashtable[i][1], null_st);
	}

	char filename[255] = "";

	// display the header
	cout << endl << "CPSC 335-x - Programming Assignment #4: ";
	cout << "Cuckoo Hashing algorithm" << endl;


	// read the strings from a file
	cout << "Input the file name (no spaces)!" << endl;
	cin >> filename;

	// open the file for reading
	FILE *file = fopen(filename, "r");
	if (file != NULL)
	{
		/* read line by line from the file */
		while (fgets(s, 255, file) != NULL) {

			// place null character at the end of the line instead of <return>
			len = strlen(s);
			s[len - 1] = '\0';
			// insert the string in the cuckoo table
			placed = place_in_hash_tables(s);
			// check whether the placement was successful
			if (!placed) {
				cout << "Placement has failed" << endl;
				return -1;
			}
		}
		fclose(file);
		
	}
	else
	{
		perror(filename); /* why didn't the file open? */
	}

	system("pause");
	return 0;
}




