#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

struct Trie {
    unordered_map<char, Trie*> children;
    bool wordEnd = false;
};

void insert(Trie* t, const string& val) {
    for(char c: val) {
        if (t->children.find(c) == t->children.end()) {
            t->children[c] = new Trie;
        }
        t = t->children[c];
    }
    t->wordEnd = true;
}

bool contains(Trie* t, const string& val) {
    for (const char& c : val) {
        if (t->children.find(c) == t->children.end()) {
            return false;
        }
        t = t->children[c];
    }
    return t->wordEnd;
}


struct Found {
    string path;
    string word;
};

unordered_map<string, int> wordsFound;
string path;
string word;
vector<Found> findings;

#define maxN 20
char scramble[maxN];
int visited[maxN];
char guess[maxN];
int n;

void dfs(Trie* t, int depth) {
    if (depth == n && t->wordEnd && wordsFound.find(word)  == wordsFound.end()) {
        wordsFound.push_back(word);
    }
}

int compareFound(Found a, Found b) {
    return a.word.size() > b.word.size();
}

int main(int argc, char *args[]) {
    if (argc != 2) {
        return;
    }
    sprintf(scramble, "%s", args[2]);
    n = strlen(scramble);
    Trie* dict = new Trie;
    ifstream fin("collins.txt");
    while (!fin.eof()) {
        string word;
        getline(fin, word);
        insert(dict, word);
    }
    dfs(dict, 0);
    sort(findings.begin(), findings.end(), compareFound);
    for (vector<Found>::iterator i = findings.begin(); i != findings.end(); i++) {
        cout << i-> word << endl;
    }
}