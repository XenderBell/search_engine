#include "SearchServer.h"
#include "textEdit.h"
#include <vector>
#include <string>
#include <cmath>

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input){
    std::vector<std::vector<RelativeIndex>> searchingResult;
    for (int i = 0; i < queries_input.size(); i++){
        std::vector<RelativeIndex> localSearch(_index.docsCount());
        for (int it = 0; it < _index.docsCount(); it++){
            localSearch[it].doc_id = it;
            localSearch[it].rank = 0;
        }
        std::vector<std::string> requestsWords;
        split_text(queries_input[i], std::ref(requestsWords));
        RemoveSimilarWords(requestsWords);
        for (int j = 0; j < requestsWords.size(); j++){
            auto freq = _index.getFreqDictionary()[requestsWords[j]];
            if (!freq.empty()) {
                for (int k = 0; k < freq.size(); k++) {
                    localSearch[k].rank += freq[k].count;
                }
            }
        }
        std::vector<float> ranks;
        for (int j = 0; j < localSearch.size(); j++){
            ranks.push_back(localSearch[j].rank);
        }
        std::sort(ranks.begin(), ranks.end());
        for (int j = 0; j < localSearch.size(); j++){
            localSearch[j].rank = localSearch[j].rank / ranks.back();
            if (isnan(localSearch[j].rank))
                localSearch[j].rank = 0;
        }
        searchingResult.push_back(localSearch);
    }
    return searchingResult;
}