#pragma once
#include <vector>
#include "index.h"
#include <string>

class ConverterJSON {
    std::string configWay;
    std::string requestsWay;
    std::string answersWay;
public:

    ConverterJSON(const std::string& config = "config.json",
                  const std::string& requests = "requests.json",
                  const std::string& answers = "./../../config/answers.json"):
                  configWay(config),
                  requestsWay(requests),
                  answersWay(answers) {};

    std::vector <std::string> GetTextDocuments();

    int GetResponsesLimit();

    std::vector <std::string> GetRequests();

    void putAnswers(std::vector <std::vector<RelativeIndex>> answers, int max_responses);

    void launch();

};
