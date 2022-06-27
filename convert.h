#ifndef CONVERT_H_
#define CONVERT_H_

using namespace std;

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

//string json_text;

class Node {
public:
	string TagName;
	string TagValue;
	Node* parent;
	vector<Node*> children;

	Node(string tagval = "", string textval = "", Node* p = NULL) :TagName{ tagval }, TagValue{ textval }, parent{ p } {
		//cout << "default constructor";
	}
	~Node() {
		// just was making sure that the allocated nodes are being destructed correctly 
		// cout << "Calling destructor"<<endl;
	}
};


void tab_spacing(int level);
string readFileIntoString(const string& path);
Node* Parse_XML(const string XML_content);
void Free_XML(Node* root);
void DFS(Node* root);
void print_json(Node* root, int level);
string convert_json(Node* root, int level);
void tab_spacing(int level);


#endif // !
