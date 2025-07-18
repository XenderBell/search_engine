#include "InvertedIndex.h"
#include <string>
#include "textEdit.h"
#include <thread>
#include <algorithm>

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word){
    std::vector<Entry> entries;
    for (int i = 0; i < docs.size(); i++){
        Entry newEntry;
        newEntry.doc_id = i;
        int wordEntries = 0;
        std::vector<std::string> splited;
        split_text(docs[i], splited);
        for (int j = 0; j < splited.size(); j++){
            if (splited[j] == word){
                wordEntries += 1;
            }
        }
        newEntry.count = wordEntries;
        if (newEntry.count > 0)
            entries.push_back(newEntry);
    }
    return entries;
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word){
    std::vector<Entry> entries;
    for (int i = 0; i < docs.size(); i++){
        Entry newEntry;
        newEntry.doc_id = i;
        int wordEntries = 0;
        std::vector<std::string> splited;
        split_text(docs[i], splited);
        for (int j = 0; j < splited.size(); j++){
            if (splited[j] == word){
                wordEntries += 1;
            }
        }
        newEntry.count = wordEntries;
        if (newEntry.count > 0)
            entries.push_back(newEntry);
    }
    return entries;
}

std::map<std::string, std::vector<Entry>> InvertedIndex::getFreqDictionary(){
    return freq_dictionary;
}
int InvertedIndex::docsCount(){
    return docs.size();
}
