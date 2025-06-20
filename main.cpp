#include <iostream>
#include <vector>
#include "nlohmann/json.hpp"
#include <fstream>
#include <exception>
#include <string>
#include <map>
#include <thread>
#include <sstream>


class NoConfigException: public std::exception{
    const char* what() const noexcept override{
        return "config file is empty.";
    }
};
class NoConfigFileException: public std::exception{
    const char* what() const noexcept override{
        return "config file is missing.";
    }
};

struct RelativeIndex{
    size_t doc_id; float rank;
};


void split_text(std::string text, std::vector<std::string> &words){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::stringstream wordStream(text);
    std::string word;
    while (wordStream >> word){
        words.push_back(word);
    }
}
//Получения текста из файла по известному пути
void readFile(std::string way, std::string &text){
    std::this_thread::sleep_for(std::chrono::seconds(1));
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


class ConverterJSON {
public:

    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments(std::string configWay){
        std::ifstream config(configWay);
        nlohmann::json configRecord;
        config >> configRecord;
        return configRecord["files"];
    }

    int GetResponsesLimit(std::string configWay){
        std::ifstream config(configWay);
        nlohmann::json configRecord;
        config >> configRecord;
        return configRecord["config"]["max_responses"];
    }

    std::vector<std::string> GetRequests(std::string requestsWay){
        std::ifstream requests("../requests.json");
        nlohmann::json requestsRecord;
        requests >> requestsRecord;
        return requestsRecord["requests"];
    }

    void putAnswers(std::vector<std::vector<RelativeIndex>> answers, std::string answersWay, int max_responses){
        std::ofstream answerFile(answersWay);
        std::string requestIDType = "request00";
        nlohmann::json list;
        for (int i = 0; i < answers.size(); i ++){
            std::string requestID = requestIDType + std::to_string(i);
            auto requestRel = answers[i];
            float summaryRel = 0.0;
            bool result = true;
            for (int j = 0; j < requestRel.size(); j++){
                summaryRel += requestRel[j].rank;
            }
            if (summaryRel == 0)
                result = false;
            list["answers"][requestID]["result"] = result;
            if (result){
                for (int j = 0; j < max_responses; j++) {
                    list["answers"][requestID]["relevance"]["docid"] = requestRel[j].doc_id;
                    list["answers"][requestID]["relevance"]["rank"] = requestRel[j].rank;
                }
            }
        }
    }

    void launch(std::string configWay){
        if (!std::filesystem::exists(configWay)){
            throw NoConfigFileException();
        }
        else {
            std::ifstream config(configWay);
            nlohmann::json configRecord;
            config >> configRecord;
            if (configRecord["config"] == nullptr){
                throw NoConfigException();
            }
            else {
                std::string name = configRecord["config"]["name"];
                std::string version = configRecord["config"]["version"];
                std::cout << name << " " << version << std::endl;
            }
        }
    }

};

struct Entry {
    size_t doc_id, count;
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex {

public:

    InvertedIndex() = default;

    void UpdateDocumentBase(std::vector<std::string> input_docs){
        for (int i = 0; i < input_docs.size(); i++){
            std::string text;
            std::thread split(readFile, input_docs[i], std::ref(text));
            docs.push_back(text);
        }
        for (int i = 0; i < docs.size(); i++){
            std::vector<std::string> words;
            std::thread split(split_text, docs[i], std::ref(words));
            split.join();
            for (int j = 0; j < words.size(); j++){
                if (!freq_dictionary.contains(words[j]))
                    freq_dictionary[words[j]] = GetWordCount(words[j]);
            }
        }
    }

    std::vector<Entry> GetWordCount(const std::string& word){
        std::vector<Entry> entries;
        for (int i = 0; i < docs.size(); i++){
            Entry newEntry;
            newEntry.doc_id = i;
            for (int j = 0; j <= docs[i].length() - word.length(); j++){
                size_t findedWord = docs[i].find(word, j);
                if (findedWord != std::string::npos)
                    newEntry.count += 1;
            }
            if (newEntry.count > 0)
                entries.push_back(newEntry);
        }
        return entries;
    }

    std::map<std::string, std::vector<Entry>> getFreqDictionary(){
        return freq_dictionary;
    }
    int docsCount(){
        return docs.size();
    }

private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;

};



class SearchServer{
public:
    SearchServer(InvertedIndex& idx) : _index(idx){};

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input){
        std::vector<std::vector<RelativeIndex>> searchingResult (queries_input.size());
        for (int i = 0; i < queries_input.size(); i++){
            std::vector<RelativeIndex> localSearch(_index.docsCount());
            for (int it = 0; it < localSearch.size(); it++){
                localSearch[it].doc_id = it;
                localSearch[it].rank = 0;
            }
            std::vector<std::string> requestsWords;
            std::thread split(split_text, queries_input[i], std::ref(requestsWords));
            split.join();
            RemoveSimilarWords(requestsWords);
            for (int j = 0; j < requestsWords.size(); j++){
                auto freq = _index.getFreqDictionary()[requestsWords[j]];
                for (int k = 0; k < freq.size(); k++){
                    localSearch[k].rank += freq[k].count;
                }
            }
            std::vector<float> ranks;
            for (int j = 0; j < localSearch.size(); j++){
                ranks.push_back(localSearch[j].rank);
            }
            std::sort(ranks.begin(), ranks.end());
            auto a = ranks.back();
            for (int j = 0; j < localSearch.size(); j++){
                localSearch[j].rank = localSearch[j].rank / ranks.back();
            }
            searchingResult.push_back(localSearch);
        }
    }

private:
    InvertedIndex _index;
};


int main(){

    ConverterJSON Converter;
    InvertedIndex Invert;

    std::string config = "../config.json";
    std::string requests = "../requests.json";
    std::string answers = "../answers.json";

    try{
        Converter.launch(config);
    }
    catch (const std::exception &x) {
        std::cerr << "Config error: " << x.what() << std::endl;
    }

    auto textDocs = Converter.GetTextDocuments(config);
    auto responsesLimit = Converter.GetResponsesLimit(config);
    auto request = Converter.GetRequests(requests);

    Invert.UpdateDocumentBase(textDocs);
    SearchServer Search (Invert);

    std::vector<std::vector<RelativeIndex>> searchResult = Search.search(request);

    Converter.putAnswers(searchResult, answers, responsesLimit);

}