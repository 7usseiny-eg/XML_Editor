#include "Compress.h"

//Encoder:
bool Encoder::find_most_ocurred(char &pair_1, char &pair_2) {
	int pairs_ocurance[256][256];
	//setting ocurance ==0
	for (int i = 0;i < 256;i++)
		for (int j = 0;j < 256;j++)
			pairs_ocurance[i][j] = 0;
	//getting most ocurred pair
	int max = 0;
	for (unsigned int i = 0;i < encoded_data.size() - 1;i++) {
		pairs_ocurance[(unsigned char)encoded_data[i]][(unsigned char)encoded_data[i + 1]]++;
		if (pairs_ocurance[(unsigned char)encoded_data[i]][(unsigned char)encoded_data[i + 1]]>max) {
			max = pairs_ocurance[(unsigned char)encoded_data[i]][(unsigned char)encoded_data[i + 1]];
			pair_1 = encoded_data[i];
			pair_2 = encoded_data[i + 1];
		}
	}
	//because key size=3 bytes because(pair_1,pair_2,token) and pairs are replaced with 1 byte so data_change(in case of max==2)=(-4)+2+3=1 so it increases
	//data_change(in case of max==3)=(-6)+3+3=0 no merit
	//so data pairs is worth encoding when is repeated more than three times
	if (max <= 3)
		return false;
	else
		return true;
}
int Encoder::data_size_change() {
	//(one byte storing number of keys)+encoded_data_size + every key takes 3 bytes -(original data)
	return 1+(encoded_data.size() + Keys.size() * 3) - byte_Count;
}
void Encoder::back_step() {
	Key key = Keys.top();
	Keys.pop();
	char pair_1 = key.pair_1;
	char pair_2 = key.pair_2;
	char token = key.token;
	for (unsigned int i = 0;i < encoded_data.size();i++) {
		if (encoded_data[i] == token) {
			encoded_data[i] = pair_1;
			encoded_data.insert(encoded_data.begin() + i + 1, pair_2);
			i++;
		}
	}
}

Encoder::Encoder(const char* filename) {
	for (int i = 0;i < 256;i++) {
		bytes_occupied[i] = false;
	}
	struct stat status;
	if (stat(filename, &status) == 0)
		byte_Count = status.st_size;
	else
		assert("can't access file");
	ifstream file(filename, ios::in | ios::binary);
	data_bytes = new char[byte_Count];
	file.read(data_bytes, byte_Count);
}
int Encoder::getBytesCount() {
	return byte_Count;
}
void Encoder::Compress() {
	stack<char>tokens;
	//pushing data into vector and which tokens can be used
	for (unsigned int i = 0;i < byte_Count;i++) {
		encoded_data.push_back(data_bytes[i]);
		bytes_occupied[(unsigned char)data_bytes[i]] = true;
	}
	for (int i = 0;i < 256;i++) {
		if (bytes_occupied[i] == false)
			tokens.push((char)i);
	}
	unsigned int key_size_optimal = 0;
	int data_change = 0;
	while (!tokens.empty()) {
		char pair_1, pair_2;
		if (find_most_ocurred(pair_1, pair_2)) {
			char token = tokens.top();
			tokens.pop();
			//making key and saving it in queue
			Key key(pair_1, pair_2, token);
			Keys.push(key);
			//replacing pairs with token
			for (unsigned int i = 0;i < encoded_data.size() - 1;i++) {
				if (encoded_data[i] == pair_1 && encoded_data[i + 1] == pair_2) {
					encoded_data[i] = token;
					encoded_data.erase(encoded_data.begin() + i + 1);
				}
			}
			if (data_size_change() < data_change) {
				data_change = data_size_change();
				key_size_optimal = Keys.size();
			}
		}
		else
			break;
	}
	//go back to reach optimal number of keys with biggest negative change
	while (Keys.size() > key_size_optimal) {
		back_step();
	}
}
void Encoder::printCompressed() {
	cout << "size encoded:" << encoded_data.size() << endl;
	cout << "number of keys:" << Keys.size() << endl;
	cout << "change in data size :" << data_size_change() << endl;
}
vector<char> Encoder::getEncodedData() {
	return encoded_data;
}
stack<Key> Encoder::getKeys() {
	return Keys;
}
void Encoder::SaveFile(const char * filename) {
	int num_keys =Keys.size();

	int total_size = 1 + num_keys * 3 + encoded_data.size();
	ofstream file(filename, ios::out | ios::binary);
	char *buffer = new char[total_size];
	int index = 0;
	buffer[index++] = (unsigned char)num_keys;
	for (int i = 0;i < num_keys;i++){
		Key key=Keys.top();
		Keys.pop();
		buffer[index++] = key.pair_1;
		buffer[index++] = key.pair_2;
		buffer[index++] = key.token;
		
	}
	for (unsigned int i = 0;i < encoded_data.size();i++) {
		buffer[index++] = encoded_data[i];
	}
	file.write(buffer, total_size);
	delete[]buffer;
}
//decoder
Decoder::Decoder(const char *filename) {
	struct stat status;
	int data_size = 0;
	if (stat(filename, &status) == 0)
		data_size = status.st_size;
	else
		assert("can't access file");
	ifstream file(filename, ios::in | ios::binary);
	char * buffer = new char[data_size];
	file.read(buffer, data_size);
	int index = 0;
	int num_keys=(unsigned char)buffer[index++];
	stack<Key>temp;
	for (int i = 0;i < num_keys;i++) {
		char pair_1 = buffer[index++];
		char pair_2 = buffer[index++];
		char token = buffer[index++];
		Key key(pair_1, pair_2, token);
		temp.push(key);
		
	}
	while(!temp.empty()){
		Keys.push(temp.top());
		temp.pop();
	}
	while (index < data_size) {
		data.push_back(buffer[index++]);
	}
	delete[]buffer;
}
void Decoder::decompress() {
	while (!Keys.empty()) {
		Key key = Keys.top();
		Keys.pop();
		char pair_1 = key.pair_1;
		char pair_2 = key.pair_2;
		char token = key.token;
		for (unsigned int i = 0;i < data.size();i++) {
			if (data[i] == token) {
				data[i] = pair_1;
				data.insert(data.begin() + i + 1, pair_2);
				i++;
			}
		}
	}
}
void Decoder::SaveFile(const char *filename) {
	int data_size = data.size();
	ofstream file(filename, ios::out | ios::binary);
	char * buffer = new char[data_size];
	for (int i = 0;i < data_size;i++) {
		buffer[i] = data[i];
	}
	file.write(buffer, data_size);
	delete[]buffer;
}
void Decoder::printDecompressed() {
	cout << "decoded data:" << endl;
	for (unsigned int i = 0;i < data.size();i++) {
		cout << data[i];
	}
	cout << endl;
}


/////////////////////////////////////////////////////////
void compress_data(const char *file_from, const char*file_to) {
	Encoder encoder(file_from);
	encoder.Compress();
	encoder.SaveFile(file_to);
}
void decompress_data(const char *file_from, const char *file_to) {
	Decoder decoder(file_from);
	decoder.decompress();
	decoder.SaveFile(file_to);
}
