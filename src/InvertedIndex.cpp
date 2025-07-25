#include "InvertedIndex.h"
#include <string>
#include "textEdit.h"
#include <thread>
#include <algorithm>

std::mutex mtx;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs){
    for (int i = 0; i < input_docs.size(); i++){
        docs.push_back(input_docs[i]);
    }
    std::vector<std::thread> threads;
    for (int i = 0; i < docs.size(); i++){
        threads.push_back(std::thread([&, i]() {
            mtx.lock();
            std::vector<std::string> words;
            split_text(docs[i], std::ref(words));
            for (int j = 0; j < words.size(); j++) {
                if (!freq_dictionary.contains(words[j])) {
                    freq_dictionary[words[j]] = GetWordCount(words[j]);
                }
            }
            mtx.unlock();
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
