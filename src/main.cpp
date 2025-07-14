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

    if (Converter.isExist(Converter.getWay(2))) {

        auto responsesLimit = Converter.GetResponsesLimit();
        auto request = Converter.GetRequests();

        Invert.UpdateDocumentBase(textDocs);
        SearchServer Search(Invert);

        std::vector<std::vector<RelativeIndex>> searchResult = Search.search(request, responsesLimit);

        Converter.putAnswers(searchResult);

    }
    else
        std::cerr << "Requests file does not exist." << std::endl;

}
