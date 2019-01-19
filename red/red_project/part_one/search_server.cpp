#include "search_server.h"
#include "iterator_range.h"
//#include "duration.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <vector>

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }

  index = move(new_index);
}

void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output ) {
  const size_t MAX_DOCS = 50000;
  const size_t MAX_RES = 5;
  //TotalDuration split("split");
  //TotalDuration lookup("lookup");
  //TotalDuration process("process");
  //TotalDuration output("output");

  vector<size_t> docid_count;
  docid_count.resize(MAX_DOCS);

  for (string current_query; getline(query_input, current_query); ) {
    vector<string> words;
    {
      //ADD_DURATION(split);
      words = SplitIntoWords(current_query); 
    }
  
    docid_count.assign(docid_count.size(), 0);
    {
      //ADD_DURATION(lookup);
      for (const auto& word : words) {
        for (const size_t docid : index.Lookup(word)) {
          docid_count[docid]++;
        }
      }
    }

    vector<pair<size_t, size_t>> search_results;
    search_results.resize(docid_count.size());
    for (size_t i = 0; i < docid_count.size(); ++i) {
      search_results[i] = make_pair(i, docid_count[i]);
    }

    {
      //ADD_DURATION(process);
      partial_sort(search_results.begin(),
        search_results.begin() + MAX_RES, search_results.end(),
        [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
        int64_t lhs_docid = lhs.first;
        auto lhs_hit_count = lhs.second;
        int64_t rhs_docid = rhs.first;
        auto rhs_hit_count = rhs.second;
        return make_pair(lhs_hit_count, -lhs_docid) 
                          > make_pair(rhs_hit_count, -rhs_docid);
      }); 
    }
    
    { 
      //ADD_DURATION(output);
      search_results_output << current_query << ':';
      for (auto[docid, hitcount] : Head(search_results, MAX_RES)) {
        if (hitcount == 0) break;
        search_results_output << " {"
          << "docid: " << docid << ", "
          << "hitcount: " << hitcount << '}';
      }
      search_results_output << endl;
    }
  }
}

void InvertedIndex::Add(const string& document) {
  docs.push_back(document);

  const size_t docid = docs.size() - 1;
  for (const auto& word : SplitIntoWords(document)) {
    index[word].push_back(docid);
  }
}

list<size_t> InvertedIndex::Lookup(const string& word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return {};
  }
}
