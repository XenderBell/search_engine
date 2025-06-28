#pragma once
#include <string>

void split_text(std::string text, std::vector<std::string> &words);
void readFile(std::string way, std::string &text);
void RemoveSimilarWords (std::vector<std::string> &words);