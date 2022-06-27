
#ifndef CONSISTENCY_H
#define CONSISTENCY_H

#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <string>
#include <QString>

using namespace std;


/*  Function Description:
 *  Correction of mismatched/missing tags by appending the correct tag to a given string
 */
void lineTagCorrection(string *line, string correctTag);

/*  Function Description:
 *  Check if line consists of only spaces or tabs
 */
bool checkEmpty(string str);

/*  Function Description:
 *  Extract tag name from a given string without its atrributes
 */
string extractTagName(string tag, int idx);

/*  Function Description:
 *  Read a given xml file into a vector of strings
 */
vector<string> fileReader(bool *success, std::string folder);

/*  Function Description:
 *  Output the corrected xml file
 */
int correctFileOutput(vector<string> &xml_buffer);

/*  Function Description:
 *  Check the xml file for errors/tag mismatches then output the corrected xml file
 */
vector<QString> consistencyCheckCorrect(vector<string> &xml_buffer);



#endif //CONSISTENCY_H
