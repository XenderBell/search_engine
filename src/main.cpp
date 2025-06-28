#include "gtest/gtest.h"
#include "../include/ConverterJSON.h"
#include "../include/InvertedIndex.h"
#include "../include/SearchServer.h"
#include "../include/launchExceptions.h"
#include "../include/textEdit.h"

using namespace std;
void TestInvertedIndexFunctionality(
        const vector<string>& docs,
        const vector<string>& requests,
        const std::vector<vector<Entry>>& expected){
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for(auto& request : requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}
TEST(TestCaseInvertedIndex, TestBasic) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock"
    };
    const vector<string> requests = {"london", "the"};
    const vector<vector<Entry>> expected = {
            {
                    {0, 1}
            }, {
                    {0, 1}, {1, 3}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2) {
    const vector<string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water", "americano cappuccino"
    };
    const vector<string> requests = {"milk", "water", "cappuchino"};
    const vector<vector<Entry>> expected = {
            {
                    {0, 4}, {1, 1}, {2, 5}
            }, {
                    {0, 2}, {1, 2}, {2, 5}
            },
            {
                    {3, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseSearchServer, TestSimple) { const vector<string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water", "americano cappuccino"
    };
    const vector<string> request = {"milk water", "sugar"}; const std::vector<vector<RelativeIndex>> expected = {
            {
                {2, 1}, {0, 0.7}, {1, 0.3}
            },
            {
            }

    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected); }

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