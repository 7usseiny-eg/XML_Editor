#include "formatter.h"

using namespace std;

string xml_formatter(const char* s)
{
    string line;
    ifstream in(s);
    stack <string> st;
    vector <string>tokens;
    vector<string>v;
    const char* t = " \t\n\r\f\v";
    while (getline(in, line))
        tokens.push_back(line);
    for (unsigned int i = 0; i < tokens.size();i++) {
        std::istringstream ss(tokens[i]);
        std::string token;
        while (std::getline(ss, token, '>')) {
            token.erase(0, token.find_first_not_of(t));
            if (token.find('<') != string::npos)
                token = token + ">";
            v.push_back(token);
        }
    }

    for (int i = 0;i < v.size()-1;i++) {
        if (v[i + 1].find('<') != string::npos && v[i + 1].find_first_of('<') != 0)
        {
            v[i] = v[i] + v[i + 1];
            v[i + 1].erase();
        }

    }
    tokens.clear();
    for (int i = 0;i < v.size();i++)
        if (v[i] != "")
            tokens.push_back(v[i]);

    for (unsigned int index = 0; index < tokens.size();index++) {
        for (unsigned int i = 0;i < st.size();i++) {
            if (tokens[index].find_first_of('<') == tokens[index].find_last_of('<') && tokens[index].find('/') != string::npos)
                if (i == st.size() - 1)
                    break;
            tokens[index] = "    " + tokens[index];

        }

        int x = tokens[index].find_first_of('<');
        int y = tokens[index].find_first_of('>');
        int z = tokens[index].find_last_of('<');
        int e = tokens[index].find_last_of('>');
        if (x < 0 || y < 0)
            continue;

        if (tokens[index].substr(x, y - x + 1).find('/') != string::npos && st.empty() == 0)
            st.pop();

        else st.push(tokens[index].substr(x, y - x + 1));
        if (z < 0 || e < 0)
            continue;
        if (x != z) {
            if (tokens[index].substr(z, e - z + 1).find('/') != string::npos && st.empty() == 0)
                st.pop();
            else st.push(tokens[index].substr(z, e - z + 1)); 
        }
    }
    string output;
    for (unsigned int i = 0; i < tokens.size();i++)
        output = output + tokens[i] + "\n";
    return output;

}




