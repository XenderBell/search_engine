#pragma once
#include <cstring>

struct RelativeIndex{
    size_t doc_id; float rank;
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

struct Entry {
    int doc_id, count;
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};