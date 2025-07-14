#include "SearchServer.h"
#include "textEdit.h"
#include <vector>
#include <string>
#include <cmath>

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input, int ResponceLimit){
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
        ranks.clear();
        for (int j = 0; j < localSearch.size(); j++){
            ranks.push_back(localSearch[j].rank);
        }
        std::sort(ranks.begin(), ranks.end());
        std::vector<RelativeIndex> SortedResult;
        for (int j = ranks.size() - 1; j >= 0; j--) {
            for (int k = 0; k < ranks.size(); k++) {
                if (ranks[j] == localSearch[k].rank) {
                    SortedResult.push_back(localSearch[k]);
                }
            }
        }
        if (SortedResult.size() > ResponceLimit){
            for (int j = 0; j < (SortedResult.size() - ResponceLimit); j++){
                SortedResult.pop_back();
            }
        }
        searchingResult.push_back(SortedResult);

    }
    return searchingResult;
}
