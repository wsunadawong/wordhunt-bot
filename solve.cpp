#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <algorithm>

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

char board[][4] = {
    {'E','A','N','E'},
    {'O','H','N','D'},
    {'L','R','R','O'},
    {'T','H','T','S'}
};

struct Found {
    string path;
    string word;
};

int visited[4][4] = {};
unordered_map<string, int> wordsFound;
string path;
string word;
int minDepth = 3;
vector<Found> findings;


void dfs(Trie* t, int r, int c, int depth) {
    if (t->wordEnd && depth >= minDepth && wordsFound.find(word)  == wordsFound.end()) {
        findings.push_back({path, word});
        wordsFound[word] = 1;
    }
    char ch = board[r][c];
    if (t->children.find(ch) == t->children.end()) {
        return;
    }
    visited[r][c] = 1;
    word.push_back(ch);
    path.push_back('0' + r);
    path.push_back('0' + c);

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (!(dr == 0 && dc == 0) && r+dr >= 0 && r+dr < 4 && c+dc >= 0 && c+dc <4 && !visited[r+dr][c+dc]) {
                dfs(t->children[ch], r+dr, c+dc, depth+1);
            }
        }
    }
    visited[r][c] = 0;
    word.pop_back();
    path.pop_back();
    path.pop_back();
}

int compareFound(Found a, Found b) {
    return a.word.size() > b.word.size();
}

int main(int argc, char *args[]) {
    if (argc == 2) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                board[r][c] = args[1][4*r+c];
            }
        }
    }

    Trie* dict = new Trie;
    ifstream fin("collins.txt");
    while (!fin.eof()) {
        string word;
        getline(fin, word);
        insert(dict, word);
    }
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            dfs(dict, r, c, 0);
        }
    }
    sort(findings.begin(), findings.end(), compareFound);
    for (vector<Found>::iterator i = findings.begin(); i != findings.end(); i++) {
        cout << i->path << ' ' << i-> word << endl;
    }
}