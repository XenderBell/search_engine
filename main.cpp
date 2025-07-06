#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include "launchExceptions.h"

int main(){

    ConverterJSON Converter;
    InvertedIndex Invert;

    try{
        Converter.launch();
    }
    catch (const std::exception &x) {
        std::cerr << "Config error: " << x.what() << std::endl;
    }

    auto textDocs = Converter.GetTextDocuments();
    auto responsesLimit = Converter.GetResponsesLimit();
    auto request = Converter.GetRequests();

    Invert.UpdateDocumentBase(textDocs);
    SearchServer Search (Invert);

    std::vector<std::vector<RelativeIndex>> searchResult = Search.search(request);

    Converter.putAnswers(searchResult, responsesLimit);

}