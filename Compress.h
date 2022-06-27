#ifndef COMPRESS_H
#define COMPRESS_H

#include<assert.h>
#include<stack>
#include<vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

//////////////////////functions to use////////////////////////////////////////
void compress_data(const char *file_from, const char*file_to);
void decompress_data(const char *file_from, const char *file_to);
//////////////////////////////////////////////////////////////////////////////

using namespace std;
class Key {
public:
	char pair_1;
	char pair_2;
	char token;
    Key(char c1, char c2, char token) {
		pair_1 = c1;
		pair_2 = c2;
		this->token = token;
	}
	void print() {
		cout << pair_1 << " " << pair_2 << " " << token;
	}
};

class Encoder {
private:
	bool bytes_occupied[256];
	unsigned int byte_Count;
	char*data_bytes;
	vector<char> encoded_data;
	stack<Key>Keys;
	bool find_most_ocurred(char &pair_1, char &pair_2);
	int data_size_change();
	void back_step();
public:
    Encoder(const char* filename);
	int getBytesCount();
	void Compress();
	void printCompressed();
	vector<char> getEncodedData();
	stack<Key> getKeys();
    void SaveFile(const char * filename);
};

class Decoder {
private:
	vector<char>data;
	stack<Key>Keys;
public:
    Decoder(const char *filename);
	void decompress();
	void printDecompressed();
    void SaveFile(const char *filename);

};






#endif
