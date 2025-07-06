#pragma once
#include <vector>
#include "index.h"

class ConverterJSON {
public:

    ConverterJSON() = default;

    std::vector <std::string> GetTextDocuments(std::string configWay);

    int GetResponsesLimit(std::string configWay);

    std::vector <std::string> GetRequests(std::string requestsWay);

    void putAnswers(std::vector <std::vector<RelativeIndex>> answers, std::string answersWay, int max_responses);

    void launch(std::string configWay);

};
