#include "convert.h"
#include <QString>

using namespace std;

string json_text;

string readFileIntoString(const string& path) {
	ifstream input_file(path);
	if (!input_file.is_open()) {
		cerr << "Could not open the file - '"
			<< path << "'" << endl;
		return "";
	}
	return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

Node* Parse_XML(const string XML_content) {
	// store the root node in order to delete all the nodes after parsing 
	Node* root = new Node();
	Node* curr_node = root;
	Node* prev_node = root;

	int i = 0;
	string curr_tag, curr_text{};

	// Parsing loop and creating XML tree structure
	while (XML_content[i] != '\0') {
		if (XML_content[i] == '<') {
			if (XML_content[i + 1] == '/')
				goto up;
			i++;
			//obtaining the tag
			while (XML_content[i] != '>')
			{
				curr_tag += XML_content[i];
				i++;
				// neglecting the tag attributes
				if (XML_content[i] == ' ')
					break;
			}
			i++;
			// obtaining the text
			while (XML_content[i] != '<')
			{
				curr_text += XML_content[i];
				i++;
			}
			// deleting any trailing spaces or new-line chars
			while (curr_text.size() && (curr_text[0] == ' ' || curr_text[0] == '\n' || curr_text[curr_text.size() - 1] == ' ' || curr_text[curr_text.size() - 1] == '\n'))
			{
				if (curr_text.size() && (curr_text[0] == ' ' || curr_text[0] == '\n'))
					curr_text.erase(0, 1);
				if (curr_text.size() && (curr_text[curr_text.size() - 1] == ' ' || curr_text[curr_text.size() - 1] == '\n'))
					curr_text.erase(curr_text.size() - 1, 1);
			}
			// we haven't yet created our root so we will create it
			if (root->TagName == "") {
				root->TagName = curr_tag;
				root->TagValue = curr_text;
			}
			// if the root is created then we should create child node to that root
			else {
				prev_node = curr_node;
				curr_node = new Node(curr_tag, curr_text, prev_node);
				prev_node->children.push_back(curr_node);
			}
			curr_tag.clear();
			curr_text.clear();

			// after obtaining the text and creating our node, we either gonna hit a closing tag for the our opened tag or another new tag
			// checking if we hit the closing tag for our opened tag and we are not at the root cause the root has no parent
		up:
			if (XML_content[i + 1] == '/')
			{
				i++;
				while (XML_content[i] != '<')
				{
					i++;
					if (i == XML_content.size())
						break;
				}
				if (curr_node != root)
					curr_node = curr_node->parent;
			}
			continue;
		}
		i++;
	}
	// returning the root node of the constructed tree
	return root;
}

void Free_XML(Node* root) {
	if (root) {
		for (auto& child : root->children) {
			Free_XML(child);
			delete child;
		}
		if (root->parent == NULL)
			delete root;
	}
}


void DFS(Node* root) {
	if (root) {
		cout << "TagName: " << root->TagName;
		if (root->TagValue.size())
			//setw is set width for making a limit for number of characters by size of Tagname
			cout << setw(30 - (root->TagName.size())) << "TagValue: " << root->TagValue << endl;
		else
			cout << endl;
		// range based for loop executes for loop over a range
		for (auto& adj : root->children) {
			DFS(adj);
		}
	}
}

void print_json(Node* root, int level) {
	vector<vector<Node*>> s1;
	//arranging
	for (int i = 0; i < root->children.size(); i++) {
		bool flag = false;
		for (int j = 0; j < s1.size(); j++) {
			if (s1[j][0]->TagName == root->children[i]->TagName) {
				s1[j].push_back(root->children[i]);
				flag = true;
				break;
			}
		}
		if (flag == false) {
			vector<Node*>n;
			s1.push_back(n);
			s1[s1.size() - 1].push_back(root->children[i]);
		}
	}
	//printing
	for (int i = 0; i < s1.size(); i++) {
		if (s1[i].size() > 1) {
			tab_spacing(level);
			json_text.append("\""); json_text.append(s1[i][0]->TagName); json_text.append("\": [\n");//cout << "\"" << s1[i][0]->TagName << "\"" << ": [" << endl;
			for (int j = 0; j < s1[i].size(); j++) {

				if (s1[i][j]->TagValue.size() == 0) {
					tab_spacing(level);
					json_text.append("{\n");//cout << "{" << endl;
					print_json(s1[i][j], level + 1);
					tab_spacing(level);
					json_text.append("}\n");//cout << "}" << endl;
				}
				else {
					tab_spacing(level);
                    json_text.append("\""); json_text.append(s1[i][j]->TagValue); json_text.append("\"\n");//cout << "\"" << s1[i][j]->TagValue << "\"" << endl;
				}

				if (j != s1[i].size() - 1) {
					tab_spacing(level);
					json_text.append(",\n");//cout << "," << endl;
				}
			}
			tab_spacing(level);
			json_text.append("]\n");//cout << "]" << endl;
		}
		else {
			if (s1[i][0]->TagValue.size() == 0) {
				tab_spacing(level);
				json_text.append("\""); json_text.append(s1[i][0]->TagName); json_text.append("\": {\n"); //cout << "\"" << s1[i][0]->TagName << "\"" << ": {" << endl;
				print_json(s1[i][0], level + 1);
				tab_spacing(level);
				json_text.append("}\n");//cout << "}" << endl;
			}
			else {
				tab_spacing(level);
				json_text.append("\""); json_text.append(s1[i][0]->TagName); json_text.append(": \""); json_text.append(s1[i][0]->TagValue); json_text.append("\"\n"); //cout << "\"" << s1[i][0]->TagName + ": " << "\"" << s1[i][0]->TagValue << "\"" << endl;
			}
		}
	}
}

string convert_json(Node* root, int level) {
	json_text.append("{\n");//cout << "{" << endl;
	tab_spacing(level);
	json_text.append("\""); json_text.append(root->TagName); json_text.append("\"{\n");//cout << "\"" << root->TagName << "\"" << "{" << endl;
	print_json(root, 2);
	tab_spacing(level);
	json_text.append("}\n");//cout << "}" << endl;
	json_text.append("}\n");//cout << "}" << endl;
	return json_text;
}
void tab_spacing(int level) {
	for (int i = 0; i < level; i++) {
		json_text.append("\t");//cout << '\t';
	}
}
