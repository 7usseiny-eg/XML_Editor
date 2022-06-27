#include "consistency.h"

using namespace std;


void lineTagCorrection(string *line, string correctTag)
{
    *line = (*line).append("<");
    *line = (*line).append("/");
    *line = (*line).append(correctTag);
    *line = (*line).append(">");
}

bool checkEmpty(string str)
{
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] != ' ' && str[i] != '\t') /* Check if line consists of only spaces or tabs */
        {
            return false;
        }
    }
    return true;
}

string extractTagName(string tag, int idx)
{
    string tagName;
    int i = idx;
    while (tag[i] != '>' && tag[i] != ' ' && i < tag.size())  /* Extract just the tag name without attibutes or closing tag */
    {
        tagName += tag[i];
        i++;
    }
    return tagName;
}

vector<string> fileReader(bool *success, std::string folder)
{
    vector<string> xml_buffer;    /* A vector of strings that would represent the xml file */
    fstream file;
    file.open(folder, ios::in);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            xml_buffer.push_back(line);
        }
        *success = 1;
        return xml_buffer;
    }
    else
    {
        *success = 0;
        return xml_buffer;
    }
}

int correctFileOutput(vector<string> &xml_buffer)
{
    fstream file;
    file.open("sample_corrected.xml", ios::out);
    if (file.is_open())
    {
        for (int i = 0; i < xml_buffer.size(); i++)
        {
            file << xml_buffer[i] << "\n";
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

vector<QString> consistencyCheckCorrect(vector<string> &xml_buffer)
{
    vector<QString> return_value;
    QString error_text = "";
    vector<QString> text = {};
    QString correct_text;
    vector<int> error_lines;
    stack<string> tagNames;      /* Stack for pushing the opening tag names */

    for (int i = 0; i < xml_buffer.size(); i++)   /* Iteration on strings in the buffer */
    {
        string line = xml_buffer[i];
        text.push_back(QString::fromStdString(line));
        if ((!(tagNames.empty())) && checkEmpty(line))        /* Missing closing tag */
        {                 /* Error message */
            error_lines.push_back(i);
            error_text.append("Error in line number ");  error_text.append(QString::number(i+1)); error_text.append("\n"); //cout << "Error in line number " << i + 1 << "\n";
            error_text.append("Missing closing tag");  error_text.append("\n"); //cout << "Missing closing tag" << "\n";
            error_text.append("Missing tag is : </");  error_text.append(QString::fromStdString(tagNames.top())); error_text.append(">\n\n"); //cout << "Missing Tag is : </" << tagNames.top() << ">\n\n";
            string correctTag = tagNames.top();
            lineTagCorrection(&xml_buffer[i], correctTag);   /* Append the missing closing tag to the string */
            tagNames.pop();                                 /* Pop tag name from stack after correction */
        }
        else
        {
            for (int j = 0; j < line.size(); j++)         /* Iteration on characters in each string */
            {

                if (line[j] == '<')
                {
                    if (line[j + 1] == '!' || line[j + 1] == '?')   /* Skip in case it's a comment */
                    {
                        //skip
                    }
                    else if (line[j + 1] != '/')       /* Push the tag name in the stack in case it's an opening tag (not closing) */
                    {
                        tagNames.push(extractTagName(line, j + 1));
                    }

                    else if (line[j + 1] == '/')    /*Check if it's a closing tag */
                    {
                        string tagName = extractTagName(line, j + 2); /* Extract the closing tag name */
                        if (tagNames.top() == tagName)
                        {
                            tagNames.pop();      /* If the closing tag matches the opening tag then pop opening tag from stack */
                        }

                        else     /* Closing tag mismatches the opening tag */
                        {
                                    /* Error message */
                            error_lines.push_back(i);
                            error_text.append("Error in line number "); error_text.append(QString::number(i + 1)); error_text.append("\n"); //cout << "Error in line number " << i + 1 << "\n";
                            error_text.append("Incorrect closing tag\n"); //cout << "Incorrect closing tag" << "\n";
                            error_text.append("Correct Tag is : </"); error_text.append(QString::fromStdString(tagNames.top())); error_text.append(">\n\n"); //cout << "Correct Tag is : </" << tagNames.top() << ">\n\n";
                            string correctTag = tagNames.top();
                            xml_buffer[i].erase(j, tagName.size() + 3);    /* Erase the mismatched closing tag */
                            lineTagCorrection(&xml_buffer[i], correctTag);  /* Append the correct tag */
                            tagNames.pop();                     /* Pop tag name from stack after correction */
                        }
                    }
                }
            }
        }

        if (i == xml_buffer.size() - 1)   /* Check for opening tags left in the stack */
        {
            if (!tagNames.empty())
            {                 /* Error message */
                error_lines.push_back(i);
                error_text.append("Error in line number ");  error_text.append(QString::number(i + 1)); error_text.append("\n"); //cout << "Error in line number " << i + 1 << "\n";
                error_text.append("Missing closing tag(s)\n");  //cout << "Missing closing tag(s)"<< "\n";
                error_text.append("Tag(s): \n"); //cout << "Tag(s): " << "\n";
                while (!tagNames.empty())
                {
                    string correctTag = tagNames.top();
                    error_text.append("</"); error_text.append(QString::fromStdString(correctTag)); error_text.append(">\n"); //cout << "</" << correctTag << ">" << "\n";     /* Display the correct tag */
                    lineTagCorrection(&xml_buffer[i], correctTag);  /*Append the missing tag */
                    tagNames.pop();                   /* Pop tag name from stack after correction */
                }
            }
        }
    }
    correctFileOutput(xml_buffer);   /* Output the corrected xml file */
    return_value.push_back(error_text);
    for(int i = 0; i < error_lines.size(); i++)
        text[error_lines[i]] =  "ERROR: " + text[error_lines[i]];
    for(int i = 0; i < text.size(); i++){
        correct_text.append(text[i] + QString("\n"));
    }
    return_value.push_back(correct_text);
    return return_value;
}
