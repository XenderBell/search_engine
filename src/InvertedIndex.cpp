#include "InvertedIndex.h"
#include <string>
#include "textEdit.h"
#include <thread>
#include <algorithm>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs){
    for (int i = 0; i < input_docs.size(); i++){
        docs.push_back(input_docs[i]);
    }
    std::vector<std::thread> threads;
    for (int i = 0; i < docs.size(); i++){
        threads.push_back(std::thread([&, i]() {
            std::vector<std::string> words;
            split_text(docs[i], std::ref(words));
            for (int j = 0; j < words.size(); j++) {
                if (!freq_dictionary.contains(words[j])) {
                    freq_dictionary[words[j]] = GetWordCount(words[j]);
                }
            }
        }));
    }
    std::for_each(threads.begin(), threads.end(), [](auto &it){it.join();});
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word){
    std::vector<Entry> entries;
    for (int i = 0; i < docs.size(); i++){
        Entry newEntry;
        newEntry.doc_id = i;
        int wordEntries = 0;
        size_t index{};
        while ((index = docs[i].find(word, index)) != std::string::npos){
            ++wordEntries;
            index += word.length();
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
