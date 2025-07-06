#pragma once
#include <vector>
#include "index.h"
#include <map>

class InvertedIndex {

public:

    InvertedIndex() = default;

    void UpdateDocumentBase(std::vector<std::string> input_docs);

    std::vector<Entry> GetWordCount(const std::string& word);

    std::map<std::string, std::vector<Entry>> getFreqDictionary();
    int docsCount();

private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;

};
