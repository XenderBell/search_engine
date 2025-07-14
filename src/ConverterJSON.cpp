#include "ConverterJSON.h"
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"
#include "launchExceptions.h"
#include "textEdit.h"
#include <iostream>

std::vector<std::string> ConverterJSON::GetTextDocuments(){
    std::ifstream config(configWay);
    nlohmann::json configRecord;
    config >> configRecord;
    std::vector<std::string> readedFiles;
    for (int i = 0; i < configRecord["files"].size(); i++){
        std::string text;
        readFile(configRecord["files"][i], text);
        readedFiles.push_back(text);
    }
    return readedFiles;
}

int ConverterJSON::GetResponsesLimit(){
    std::ifstream config(configWay);
    nlohmann::json configRecord;
    config >> configRecord;
    return configRecord["config"]["max_responses"];
}

std::vector<std::string> ConverterJSON::GetRequests(){
    std::ifstream requests(requestsWay);
    nlohmann::json requestsRecord;
    requests >> requestsRecord;
    return requestsRecord["requests"];
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers){
    std::ofstream answerFile(answersWay);
    if (!answerFile.is_open()){
        std::cerr << "Opening file error!" << std::endl;
    }
    else {
        std::string requestIDType = "request00";
        nlohmann::json list;
        for (int i = 0; i < answers.size(); i++) {
            std::string requestID = requestIDType + std::to_string(i);
            auto requestRel = answers[i];
            float summaryRel = 0.0;
            bool result = true;
            std::vector<float> ranks;
            for (int j = 0; j < requestRel.size(); j++) {
                summaryRel += requestRel[j].rank;
                ranks.push_back(requestRel[j].rank);
            }
            if (summaryRel == 0)
                result = false;
            list["answers"][requestID]["result"] = result;
            if (result) {
                std::sort(ranks.begin(), ranks.end());
                for (auto it = requestRel.begin(); it != requestRel.end(); ++it) {
                    list["answers"][requestID]["relevance"].push_back(
                            {
                                    {"docid", it->doc_id},
                                    {"rank",  it->rank}
                            }
                    );
                }
            }
        }
        answerFile << std::setw(3) << list;
    }
}

void ConverterJSON::launch(){
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

bool ConverterJSON::isExist(std::string way) {
    std::fstream file(way);
    if (file.is_open())
        return true;
    else
        return false;
}

std::string ConverterJSON::getWay(int number) {
    switch(number){
        case 1:
            return configWay;
            break;
        case 2:
            return requestsWay;
            break;
        case 3:
            return answersWay;
            break;
    }
    return {};
}
