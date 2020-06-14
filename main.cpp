#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <codecvt>
#include <locale>

int main(int argc, char** argv) {
	std::setlocale(LC_ALL, "en_US.UTF8");

	std::wcout << L"This is word-counter by Antoni Kiszka - a high-speed, high-capacity, low-memory text analyser." << std::endl;
	std::wcout << L"Please enter the files you wish to analyse followed by an EOF." << std::endl;

	std::vector<std::wstring> filenames;

	{ // read the filenames
		std::wstring filename;
		while(std::wcin >> filename)
			filenames.push_back(filename);
	}

	std::wcin.clear();
	std::wcout.clear();

	std::wcout << "Processing..." << std::endl;

	std::unordered_map<std::wstring, std::vector<uint_fast64_t>> word_pos;
	uint_fast64_t unique_word_count = 0;
	uint_fast64_t all_word_count = 0;

	for(const auto& filename : filenames) { // read all files into word_pos
		std::wifstream file(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(filename));
		file.imbue(std::locale(std::locale("pl_PL.UTF8"), new std::codecvt_utf8<wchar_t>));
		std::wstringstream wss;
		wss << file.rdbuf();

		std::wstring word;
		uint_fast64_t offset = 0;
		while(wss >> word) {
			++offset;
			++all_word_count;
			word.erase(std::remove_if(word.begin(), word.end(), [](const wchar_t& c){ return !std::iswalpha(c); }), word.end());
			std::transform(word.begin(), word.end(), word.begin(), [](const wchar_t& c) { return std::towlower(c); });
			word_pos[word].push_back(offset);
		}
	}

	word_pos.erase(L"");

	std::vector<const std::wstring*> wordlist;
	uint_fast32_t nth = std::min<size_t>(40, word_pos.size());


	{ // generate the top list & count the uniqe words
		wordlist.reserve(word_pos.size());
		for(const auto& word : word_pos) {
			++unique_word_count;
			wordlist.push_back(&word.first);
		}

		auto compare = [&](const std::wstring* i, const std::wstring* j){ return word_pos.at(*i).size() > word_pos.at(*j).size(); };

		std::nth_element(wordlist.begin(), wordlist.begin()+nth, wordlist.end(), compare);
		std::sort(wordlist.begin(), wordlist.begin()+nth, compare);
	}

	std::wcout << "Processed " << all_word_count << " words, " << unique_word_count << " of which are uniqie. These are the results: " << std::endl;

	for(uint_fast8_t i = 0; i <= nth; ++i) { // print the top list
		uint_fast32_t word_count = word_pos[*wordlist[i]].size();
		double percentage = static_cast<double>(word_count) / static_cast<double>(all_word_count) * 100.0;
		std::wcout << *wordlist[i] << ": " << word_count << " (" << percentage << "%)" << std::endl;
	}
}
