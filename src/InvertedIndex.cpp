#include "InvertedIndex.h"
#include <string>
#include "textEdit.h"
#include <thread>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs){
    for (int i = 0; i < input_docs.size(); i++){
        docs.push_back(input_docs[i]);
    }
    for (int i = 0; i < docs.size(); i++){
        std::vector<std::string> words;
        split_text(docs[i], std::ref(words));
        for (int j = 0; j < words.size(); j++){
            if (!freq_dictionary.contains(words[j])) {
                freq_dictionary[words[j]] = GetWordCount(words[j]);
            }
        }
    }
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs){
    for (int i = 0; i < input_docs.size(); i++){
        docs.push_back(input_docs[i]);
    }
    std::thread t[docs.size()];
    for (int i = 0; i < docs.size(); i++){
        std::vector<std::string> words;
        t[i] = std::thread(split_text,docs[i], std::ref(words));
        t[i].join();
        for (int j = 0; j < words.size(); j++){
            if (!freq_dictionary.contains(words[j])) {
                freq_dictionary[words[j]] = GetWordCount(words[j]);
            }
        }
    }
}

std::map<std::string, std::vector<Entry>> InvertedIndex::getFreqDictionary(){
    return freq_dictionary;
}
int InvertedIndex::docsCount(){
    return docs.size();
}
