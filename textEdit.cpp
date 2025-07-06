#include "textEdit.h"
#include <sstream>
#include <fstream>
#include <vector>

void split_text(std::string text, std::vector<std::string> &words){
    std::stringstream wordStream(text);
    std::string word;
    while (wordStream >> word){
        words.push_back(word);
    }
}
void readFile(std::string way, std::string &text){
    std::ifstream textFile(way);
    std::string line;
    while (std::getline(textFile, line)) {
        text += line;
    }
}
void RemoveSimilarWords (std::vector<std::string> &words){
    std::vector<std::string> newList;
    newList.push_back(words[0]);
    for (int i = 1; i < words.size(); i++){
        if (std::count(newList.begin(), newList.end(), words[i]) == 0){
            newList.push_back(words[i]);
        }
    }
    words = newList;
}