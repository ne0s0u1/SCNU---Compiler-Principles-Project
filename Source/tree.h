#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "JuceHeader.h"
using namespace std;
#define EMPTY "@"
#define END "$"
#define ACCEPT "acc"
inline set<string> mainCd;  // 大类编码
inline map<string, string> idMap;
inline map<string, string> tkMap{ {"(", "()"}, {")", "()"}, {"[", "[]"}, {"]", "[]"}, {"{", "{}"}, {"}", "{}"} };



inline string YijinStr = juce::CharPointer_UTF8("\xe7\xa7\xbb\xe8\xbf\x9b\xe7\x8a\xb6\xe6\x80\x81");

struct Pair {
    int x, y;  // x: 父节点编号 y: 子节点编号
};

struct TreeEdge {
    int root;
    vector<Pair> pairs;
};

struct Node {
    string token;
    vector<Node*> kids;
    Node(string str = "") : token(tkMap.find(str) != tkMap.end() ? tkMap[str] : str) {}
};

struct Gram {
    string l;
    vector<string> r;
    string str() {
        string s = l + " ->";
        for (auto& x : r)
            s += ' ' + x;
        return s;
    }
};

struct Lex {  // 词法
    string k, v;
    string token() {  // 在LALR1分析时的接收token
        return mainCd.find(k) != mainCd.end() ? idMap[k] : v;
    }  // 大类编码的v是单词内容，需要取其单词名称
    string str(bool full) {
        string s = k == juce::CharPointer_UTF8("\xe7\x8a\xb6\xe6\x80\x81") ? '[' + v + ']' : v;
        return full ? '<' + k + ' ' + s + '>' : s;
    }  // 打印所用的字符串
};

struct TreeState {
    string id, token;
    // 重载 operator< 来进行Hash比较
    bool operator<(const TreeState& other) const {
        return id != other.id ? stoi(id) < stoi(other.id) : token < other.token;
    }
};

inline bool isStrEmpty;  // 是否接收空串
inline int lexi, step;  // 当前读取的词法下标 当前语法分析的步骤数
inline string id, in, mv;  // 状态编号，输入token，动作
inline int Gx, Gn;  // 当前归约要用的文法产生式下标、文法右部token数
inline Gram Gy;  // 当前归约要用的文法产生式
inline Node* Root;  // 语法树根节点
inline vector<Node*> Na, Nb, Nc;  // 语法树节点记录
inline vector<Lex> Lc;  // 词组分析栈
inline vector<Lex> Lexs;  // 读取的Lex文件中的所有词组
inline vector<Gram> TreeGrams;  // 所有文法
inline vector<TreeEdge> Edges;  // 语法树的边
inline map<TreeState, string> Table;  // 分析表

/**
 * @brief 重置所有与语法分析和语法树构建相关的全局状态。
 *        必须在每次开始新的语法分析之前调用，以防止旧数据干扰。
 */
inline void resetParserAndTree()
{
    // --- 1. 清空所有核心数据容器 ---
    mainCd.clear();
    idMap.clear();
    // tkMap 是 const 初始化，不需要重置

    Lc.clear();         // 清空分析栈
    Lexs.clear();       // 清空输入的Token流
    TreeGrams.clear();  // 清空文法规则
    Edges.clear();      // 清空语法树构建规则
    Table.clear();      // 清空分析表

    // --- 2. 释放并清空节点相关的容器 ---
    // 先释放所有已创建节点的内存
    for (auto* node : Na) {
        delete node;
    }
    // 然后清空所有节点存储向量
    Na.clear();
    Nb.clear();
    Nc.clear();

    // --- 3. 重置所有状态变量和计数器 ---
    isStrEmpty = false;
    lexi = 0;
    step = 0;

    id.clear();
    in.clear();
    mv.clear();

    Gx = 0;
    Gn = 0;
    // Gy (Gram对象) 不需要手动清空，它在每次reduce时都会被完整地重新赋值

    Root = nullptr; // 将树的根指针置空
}

inline void loadGrams(const string& path) {
    string line, token;
    ifstream file(path);

    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> v;
        stringstream ss(line);
        while (getline(ss, token, ' '))  // 按'\t'分割
            v.push_back(token);
        TreeGrams.push_back({ v[0], vector<string>(v.begin() + 1, v.end()) });
    }
    file.close();
}

inline void loadTable(const string& path) {
    string line;
    ifstream file(path);
 
    while (getline(file, line)) {
        if (line.empty()) continue;
        int i = line.find(' '), j = line.rfind(' ');
        string a = line.substr(0, i), b = line.substr(i + 1, j - i - 1), c = line.substr(j + 1);
        Table[{a, b}] = c;
    }
    file.close();
}

inline void loadMap(const string& path) {
    string line, cd;
    ifstream file(path);

    getline(file, line);
    istringstream ss(line);
    while (ss >> cd)
        mainCd.insert(cd);
    while (getline(file, line)) {
        if (line.empty()) continue;
        int i = line.find('\t');
        string k = line.substr(0, i), v = line.substr(i + 1);
        idMap[k] = v;
    }
    file.close();
}

inline void loadLex(const string& path) {
    string line;
    ifstream file(path);

    while (getline(file, line)) {
        if (line.empty()) continue;
        int i = line.find('\t');
        if (i == -1)
            Lexs.push_back(Lex{ line, idMap[line] });
        else
            Lexs.push_back(Lex{ line.substr(0, i), line.substr(i + 1) });
    }
    file.close();
}

inline void getPairs(const string& str, TreeEdge& edge) {
    int k, x, y;
    string pairStr;
    istringstream iss(str);  // 使用 stringstream 来逐个处理字符串
    while (iss >> pairStr) {  // 按空格分隔每一对数字
        int k = pairStr.find('-');  // 查找横杆的位置
        if (k != -1) {
            x = stoi(pairStr.substr(0, k));
            y = stoi(pairStr.substr(k + 1));
            edge.pairs.push_back({ x, y });  // 将数字对添加到结果中
        }
    }
}

inline void loadTree(const string& path) {
    string line;
    ifstream file(path);

    int i, j, k = 0, n = TreeGrams.size();
    Edges.resize(n);
    while (getline(file, line)) {
        if (k >= n) break;
        if (line.empty()) continue;
        i = line.find(' ');
        j = stoi(line.substr(0, i));
        Edges[k].root = j;
        getPairs(line.substr(i + 1), Edges[k++]);
    }
    while (k < TreeGrams.size())
        Edges[k++].root = 0;
    file.close();
}


inline Node* newNode(string str = "") {
    Node* x = new Node(str);
    Na.push_back(x);
    return x;
}

inline void delNodes() {
    for (auto& x : Na)
        delete x;
}

inline void trim() {
    Nc = { newNode() };
    Nc.insert(Nc.end(), Nb.end() - Gn, Nb.end());  // 反转节点栈顺序
    Nb.erase(Nb.end() - Gn, Nb.end());
}

inline void merge(Node* x, Node* y) {  // 合并节点
    if (y->token.empty()) x->kids.insert(x->kids.end(), y->kids.begin(), y->kids.end());
    else x->kids.push_back(y);
}

inline void genNode() {
    trim();
    for (const auto& p : Edges[Gx].pairs)
        merge(Nc[p.x], Nc[p.y]);
    Nb.push_back(Nc[Edges[Gx].root]);
}
inline void _printTree(int n, Node* x, ofstream& Tofs) {
    Tofs << string(n, ' ') << x->token << '\n';
    for (auto& y : x->kids)
        _printTree(n + 1, y, Tofs);
}
inline void printTree(ofstream &Tofs) {
    if (Root != NULL)
        _printTree(0, Root,Tofs);
    else
        Tofs << 111;
}

// 查询分析表
inline string Lookup(TreeState key) {
    if (Table.find(key) != Table.end()) return Table[key];
    key.token = EMPTY;  // 查询能否接收空串
    return 'e' + Table[key];
}

// 当前所在状态, LALR1接收的token, 动作+id
inline void shift(ofstream &Pofs) {  // 移进至状态[id](直接接收新输入)
    isStrEmpty = mv[0] == 'e';
    Lex lex = isStrEmpty ? Lex{ juce::CharPointer_UTF8("\xe7\xa9\xba\xe4\xb8\xb2"), EMPTY } : Lexs[lexi++];  // 特判能接收空串的情况
    string s = mv.substr(1 + isStrEmpty);
    Lc.push_back(lex);
    Lc.push_back(Lex{ juce::CharPointer_UTF8("\xe7\x8a\xb6\xe6\x80\x81"), s });
    Node* x = newNode(lex.str(false));
    Nb.push_back(x);  // 生成树节点
    Pofs << YijinStr << s << '\n';//移进状态
}

// 用文法[id]归约(归约跳转)
inline void reduce(ofstream& Pofs) {
    Gx = stoi(mv.substr(1)), Gy = TreeGrams[Gx], Gn = Gy.r.size();
    Lc.erase(Lc.end() - 2 * Gn, Lc.end());
    id = Lc.back().token();
    mv = Lookup(TreeState{ id, Gy.l });
    Lc.push_back(Lex{ juce::CharPointer_UTF8("\xe4\xba\xa7\xe7\x94\x9f\xe5\xbc\x8f"), Gy.l });
    Lc.push_back(Lex{ juce::CharPointer_UTF8("\xe7\xa9\xba\xe4\xb8\xb2"), mv });  // 跳转至新状态;
    genNode();
    Pofs << string(juce::CharPointer_UTF8("\xe7\x94\xa8")) << Gy.str() << string(juce::CharPointer_UTF8("\xe5\xbd\x92\xe7\xba\xa6")) << endl;
}

// 用文法[0]归约(归约接受)
inline void accept(ofstream& Pofs) {
    Gx = 0, Gy = TreeGrams[Gx], Gn = Gy.r.size();
    if (Nb[0]->token == "") {
        Root = Nb[0];
        Root->token = juce::CharPointer_UTF8("\xe5\xbc\x80\xe5\xa7\x8b");
    }
    else {
        string nodeS = juce::CharPointer_UTF8("\xe5\xbc\x80\xe5\xa7\x8b");
        Root = newNode(nodeS);
        Root->kids.push_back(Nb[0]);
    }
    ++lexi;
    Pofs << string(juce::CharPointer_UTF8("\xe7\x94\xa8")) << Gy.str() << string(juce::CharPointer_UTF8("\xe5\xbd\x92\xe7\xba\xa6(\xe6\x8e\xa5\xe5\x8f\x97)")) << endl;
}

inline void parse(ofstream& Pofs) {
    Lc.push_back(Lex{ juce::CharPointer_UTF8("\xe7\xa9\xba\xe4\xb8\xb2"), "0" });  // 初始状态
    int n = Lexs.size();
    
    while (lexi < n) {
        id = Lc.back().token();
        in = Lexs[lexi].token();
        mv = Lookup(TreeState{ id, in });
        Pofs << string(juce::CharPointer_UTF8("[\xe6\xad\xa5\xe9\xaa\xa4")) << ++step << "]\n";
        Pofs << string(juce::CharPointer_UTF8("\xe5\x88\x86\xe6\x9e\x90\xe6\xa0\x88"));
        for (auto& x : Lc)
            Pofs << ' ' << x.str(false);
        Pofs << '\n';
        Pofs << string(juce::CharPointer_UTF8("\xe8\xbe\x93\xe5\x85\xa5")) << Lexs[lexi].str(true) << '\n';
        switch (mv[0]) {
        case 'e':
        case 's': shift(Pofs); break;  // 移进
        case 'r': reduce(Pofs); break;  // 归约
        case 'a': accept(Pofs); break;  // 接受 
        default:
            Pofs << string(juce::CharPointer_UTF8("\xe6\x9c\xaa\xe7\x9f\xa5\xe5\x8a\xa8\xe4\xbd\x9c")) << mv << '\n';
            exit(1);
        }
    }
}

