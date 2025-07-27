#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "JuceHeader.h"
using namespace std;

/*================================== 核心定义与全局变量 ==================================*/

// --- 特殊字符宏定义 ---
#define EMPTY char(0)   // 空字符ε的内部表示
#define ANY   '~'     // 任意字符的内部表示
#define END   '$'     // 输入结束符的表示
#define EXIT  char(-1)  // 正则表达式中用于标记单词出口的内部符号
#define RANGE '-'     // 字符集合中范围操作符的表示, e.g., [a-z]

// --- 工具宏 ---
#define STR(x) string(1, x) // 将单个字符快速转换为字符串

// --- 数据结构：正则表达式解析与Token管理 ---
inline map<string, set<char>> CharSet;         // 存储字符集定义, e.g., "letter" -> {'a', 'b', ..., 'Z'}
inline set<char> CharSetSymbol;                   // 存储代表字符集的内部符号
inline vector<string> MapStr{ "\\+", "\\|", "\\(", "\\)", "\\*", "\\?", "\\n", "\\~" }; // 需要从字符串映射到特殊字符的列表
inline map<char, string> Code2Str;                  // 特殊字符到原字符串的映射 (用于输出)
inline map<string, char> Str2Code;                  // 原字符串到特殊字符的映射 (用于内部处理)
inline map<int, string> Code2Token{ {0, STR(END)} };// 单词编码(Code)到其原始Token字符串的映射
inline vector<int> CodeMain[2];                  // 存储大类编码: 0为有实义, 1为无实义
inline map<int, int> PriorityOfCode;                   // 单词编码的优先级
inline map<char, int> OpPrec{ {'|', 1}, {'@', 2}, {'*', 3}, {'?', 3} }; // 正则表达式中运算符的优先级

// --- 状态与计数器 ---
inline int NfaCnt = 0;   // NFA节点ID的全局计数器
inline int CdCnt = 1;    // 单词编码(Token Code)的全局计数器 (0被END占用)
inline set<char> Chars;  // 从正则表达式中收集到的所有有效字符集合

// --- 核心数据存储 ---
inline string InRegex;    // 用户输入的原始正则表达式字符串
inline string OutRegex;   // 经过预处理和符号映射后的正则表达式，用于生成NFA
inline string LexCode;    // 最终生成的C语言词法分析器源码
inline vector<vector<string>> NFAGraph, DFAGraph, MDFAGraph; // 用于UI表格展示的自动机状态转换图


/*================================== NFA (非确定有限自动机) 相关结构 ==================================*/
struct NFA; // 前向声明，因为Edge结构需要使用NFA*

// NFA的边结构
struct Edge {
    char c;     // 边的转移条件字符
    NFA* next;  // 边指向的下一个NFA节点
};

// NFA的节点结构
struct NFA {
    int id = -1;             // 节点唯一ID，由NfaCnt分配
    int cd = 0;              // 如果是终态，表示识别出的单词编码(Code)
    bool isStart = false;    // 是否为初态
    bool isEnd = false;      // 是否为终态
    bool isExit = false;     // 是否为单词的出口状态（附带单词编码）
    vector<Edge> edges;      // 从该节点出发的所有边
    NFA() : id(NfaCnt++) {}  // 构造时自动分配ID
};

// NFA片段结构，用于在Thompson算法中表示一个NFA的起始和结束节点
struct NFAP {
    NFA* start;
    NFA* end;
    NFAP(NFA* s = nullptr, NFA* e = nullptr) :start(s), end(e) {}
};

// --- NFA构建过程中的全局变量 ---
inline stack<NFA*> NFAs;          // 存储所有动态分配的NFA节点，便于后续释放内存
inline NFAP NFAp;                 // 最终构建完成的完整NFA图
inline set<int> StartNFA;         // NFA的初态集 (通常只有一个)
inline set<int> EndNFA;           // NFA的终态集
inline map<int, int> ExitNFA;      // NFA的出口状态ID到单词编码(cd)的映射

// --- NFA状态转换表相关 ---
// 用于表示状态转换表中的一个状态（行）
struct nodeState {
    int id = -1;
    string flag;                    // 标记: "-"为初态, "+"为终态
    map<char, set<int>> m;          // 状态转移映射: char -> set of next_state_ids
};
inline map<int, nodeState> StateTable;  // 完整的状态转换表: nfa_state_id -> nodeState
inline vector<int> InsertOrder;       // 记录节点插入顺序，保证表格输出时初态在前，终态在后


/*================================== DFA (确定有限自动机) 相关结构 ==================================*/

// DFA节点/状态结构
struct DFA {
    int id = -1;               // DFA状态的唯一ID
    int cd = 0;                // 如果是终态，表示识别出的单词编码
    string flag;               // 标记: "-"为初态, "+"为终态
    set<int> states;           // 该DFA状态所包含的NFA状态集合 (ε-closure的结果)
    map<char, set<int>> trans; // DFA的状态转移函数: char -> set of NFA states
};

// --- DFA构建与最小化过程中的全局变量 ---
inline int CountDFA;                          // DFA状态ID的全局计数器
inline vector<DFA> DFAList;                 // 存储所有DFA状态的列表
inline set<set<int>> DFAStateSet;           // 用于在子集构造法中对DFA状态进行去重
inline map<set<int>, int> DFASetMap;         // NFA状态集到DFA状态ID的映射
inline set<int> DFAEndState;                // DFA的终态集合 (用于最小化)
inline set<int> DFANotEndState;             // DFA的非终态集合 (用于最小化)


/*================================== 最小化DFA (MDFA) 相关结构 ==================================*/

// 最小化DFA节点/状态结构
struct MDFA {
    int id = -1;              // MDFA状态的唯一ID
    int cd = 0;               // 如果是终态，表示识别出的单词编码
    string flag;              // 标记: "-"为初态, "+"为终态
    map<char, int> trans;     // MDFA的状态转移函数: char -> next_mdfa_state_id
};

// --- MDFA构建过程中的全局变量 ---
inline int MDFAStart;                   // 最小化DFA的初态ID
inline int MDFACount;                     // MDFA状态ID的全局计数器
inline vector<MDFA> MDFAList;           // 存储所有MDFA状态的列表
inline vector<set<int>> DivideSet;       // 在DFA最小化（Hopcroft算法）中用于存储划分的状态集合
inline map<int, int> DFA2MDFA;          // DFA状态ID到其所属的MDFA分组ID的映射


/*============================通用函数方法==================================*/

//初始化函数
inline void resetLexerAnalysis()
{
    // --- 清空所有 map 和 set ---
    CharSet.clear();
    CharSetSymbol.clear();
    Code2Str.clear();
    Str2Code.clear();
    Code2Token.clear();
    PriorityOfCode.clear();
    Chars.clear();
    StateTable.clear();
    StartNFA.clear();
    EndNFA.clear();
    ExitNFA.clear();
    DFAStateSet.clear();
    DFASetMap.clear();
    DFAEndState.clear();
    DFANotEndState.clear();
    DFA2MDFA.clear();

    // --- 清空所有 vector ---
    CodeMain[0].clear();
    CodeMain[1].clear();
    NFAGraph.clear();
    DFAGraph.clear();
    MDFAGraph.clear();
    InsertOrder.clear();
    DFAList.clear();
    MDFAList.clear();
    DivideSet.clear();

    // --- 释放 NFA 节点的内存并清空栈 ---
    while (!NFAs.empty()) {
        delete NFAs.top();
        NFAs.pop();
    }

    // --- 重置所有字符串 ---
    InRegex.clear();
    OutRegex.clear();
    LexCode.clear();

    // --- 重置所有计数器和基本类型变量 ---
    NfaCnt = 0;
    CdCnt = 1;
    CountDFA = 0;
    MDFAStart = 0;
    MDFACount = 0;

    // --- 重置 NFAp 结构体 ---
    NFAp.start = nullptr;
    NFAp.end = nullptr;

    // --- 重新初始化必要的全局数据 ---
    Code2Token[0] = STR(END); // 重新设置终结符$的Token

    // 重新填充 Str2Code 和 Code2Str 映射表
    for (int i = 0; i < MapStr.size(); ++i) {
        char c = char(EXIT - 1 - i);
        Str2Code[MapStr[i]] = c;
        Code2Str[c] = MapStr[i];
    }
}

// 按字符切分字符串
inline vector<string> split(const string& str, const char& delimiter) {
    vector<string> result;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter))
        result.push_back(token);
    return result;
}

//去除前后置的转义符'\'，用于结果展示
inline string trim(string str) {
    if (str.empty() || str == "\\n") return str;
    str.erase(0, str.find_first_not_of('\\'));
    str.erase(str.find_last_not_of('\\') + 1);
    return str;
}

//set转string，用于结果展示
inline string set2string(const set<int>& s) {
    string result;
    for (const auto& i : s) {
        result.append(to_string(i));
        result += ',';
    }
    if (result.size() != 0)
        result.pop_back(); //弹出最后一个逗号
    return result;
}

inline string escape(const char& c) {
    switch (c) {
    case('\n'): return "\\n";
    case('\\'): return "\\\\";
    case('\''): return "\\\'";
    case('\"'): return "\\\"";
    default: return STR(c);
    }
}

// 字符串子串替换
inline void replace(string& str, const string& x, const string& y) {
    int i = 0;
    while ((i = str.find(x, i)) != -1) {
        str.replace(i, x.size(), y);
        i += y.size();
    }
}

//判断是不是字符，由于前面已经对出现在下面的字符进行了希腊字母转换，所以一定不会出现下面的字符
inline bool isChar(const char& c) {
    return !(c == '+' || c == '|' || c == '*' || c == '('
        || c == ')' || c == '?' || c == '@' || c == EXIT);
}

// 分类连续区间字符和非连续区间字符
inline vector<vector<char>> sortCharSet(const set<char>& s) {
    char l = *s.begin(), r = l - 1;
    vector<vector<char>> v(2);
    for (const auto& c : s) {
        if (c == r + 1) ++r;
        else {
            if (l == r) v[1].push_back(l);
            else v[0].push_back(l), v[0].push_back(r);
            l = r = c;
        }
    }
    if (l == r) v[1].push_back(l);
    else v[0].push_back(l), v[0].push_back(r);
    return v;
}


// 创建并记录NFA节点
inline NFA* newNFA() {
    NFA* x = new NFA;
    NFAs.push(x);
    return x;
}

// 释放所有NFA节点
inline void delNFAs() {
    while (!NFAs.empty()) {
        delete NFAs.top();
        NFAs.pop();
    }
}

//创建基本字符NFA，只包含一个字符的NFA图
inline NFAP basicNFA(const char& c) {
    NFA* S = newNFA();
    NFA* E = newNFA();
    S->isStart = true;
    E->isEnd = true;
    Edge edge;
    edge.c = c;
    edge.next = E;
    S->edges.push_back(edge);
    // 存入全局字符set
    Chars.insert(c);
    return NFAP(S, E);
}

//创建连接运算符的NFA图
inline NFAP concatNFA(NFAP& x, NFAP& y) {
    // 把nfa1的终止状态与nfa2的起始状态连接起来
    x.end->isEnd = false;
    y.start->isStart = false;
    Edge edge;
    edge.c = EMPTY; // 这里用EMPTY表示空边
    edge.next = y.start;
    x.end->edges.push_back(edge);
    return NFAP(x.start, y.end);
}

//创建选择运算符的NFA图
inline NFAP unionNFA(NFAP& x, NFAP& y) {
    NFA* S = newNFA();
    NFA* E = newNFA();
    S->isStart = true;
    E->isEnd = true;
    // 把新的初态与nfa1和nfa2的初态连接起来
    Edge edge1;
    edge1.c = EMPTY;
    edge1.next = x.start;
    S->edges.push_back(edge1);
    x.start->isStart = false;    // 初态结束
    Edge edge2;
    edge2.c = EMPTY;
    edge2.next = y.start;
    S->edges.push_back(edge2);
    y.start->isStart = false;    // 初态结束
    // 把nfa1和nfa2的终止状态与新的终止状态连接起来
    x.end->isEnd = false;
    y.end->isEnd = false;
    Edge edge3;
    edge3.c = EMPTY;
    edge3.next = E;
    x.end->edges.push_back(edge3);
    Edge edge4;
    edge4.c = EMPTY;
    edge4.next = E;
    y.end->edges.push_back(edge4);
    return NFAP(S, E);
}

//创建*运算符的NFA图
inline NFAP repeatNFA(NFAP& x) {
    NFA* S = newNFA();
    NFA* E = newNFA();
    S->isStart = true;
    E->isEnd = true;
    // 把新的初态与nfa1的初态连接起来
    Edge edge1;
    edge1.c = EMPTY;
    edge1.next = x.start;
    S->edges.push_back(edge1);
    x.start->isStart = false;    // 初态结束
    // 把新的初态与新的终止状态连接起来
    Edge edge2;
    edge2.c = EMPTY;
    edge2.next = E;
    S->edges.push_back(edge2);
    // 把nfa1的终止状态与新的终止状态连接起来
    x.end->isEnd = false;
    Edge edge3;
    edge3.c = EMPTY;
    edge3.next = x.start;
    x.end->edges.push_back(edge3);
    Edge edge4;
    edge4.c = EMPTY;
    edge4.next = E;
    x.end->edges.push_back(edge4);
    return NFAP(S, E);
}

//创建？运算符的NFA图
inline NFAP optionNFA(NFAP& x) {
    NFA* S = newNFA();
    NFA* E = newNFA();
    S->isStart = true;
    E->isEnd = true;
    // 把新的初态与nfa1的初态连接起来
    Edge edge1;
    edge1.c = EMPTY;
    edge1.next = x.start;
    S->edges.push_back(edge1);
    x.start->isStart = false;    // 初态结束
    // 把新的初态与新的终止状态连接起来
    Edge edge2;
    edge2.c = EMPTY;
    edge2.next = E;
    S->edges.push_back(edge2);
    // 把nfa1的终止状态与新的终止状态连接起来
    x.end->isEnd = false;
    Edge edge3;
    edge3.c = EMPTY;
    edge3.next = E;
    x.end->edges.push_back(edge3);
    return NFAP(S, E);
}

//使用DFS算法生成状态转换表
inline void buildNFAStateTable() {
    stack<NFA*> nfaStack;
    set<NFA*> visited;
    // 初态
    NFA* S = NFAp.start;
    nodeState startState;
    startState.flag = '-'; // -表示初态
    startState.id = S->id;
    StateTable[S->id] = startState;
    InsertOrder.push_back(S->id);
    StartNFA.insert(S->id);
    nfaStack.push(S);
    while (!nfaStack.empty()) {
        NFA* x = nfaStack.top();
        nfaStack.pop();
        visited.insert(x);
        for (const Edge& edge : x->edges) {
            char trChar = edge.c;
            NFA* y = edge.next;
            // 记录状态转换信息
            StateTable[x->id].m[trChar].insert(y->id);
            // 如果下一个状态未被访问，将其加入堆栈
            if (visited.find(y) == visited.end()) {
                nfaStack.push(y);
                // 记录状态信息
                nodeState nextState;
                nextState.id = y->id;
                if (y->isStart) {
                    nextState.flag = "-"; // -表示初态
                    StartNFA.insert(nextState.id);
                }
                if (y->isEnd) {
                    nextState.flag = "+"; // +表示终态
                    EndNFA.insert(nextState.id);
                }
                if (y->isExit)
                    ExitNFA[nextState.id] = y->cd;
                StateTable[y->id] = nextState;
                // 记录插入顺序（排除终态）
                if (!y->isEnd)
                    InsertOrder.push_back(y->id);
            }
        }
    }
    // 顺序表才插入终态
    NFA* endNode = NFAp.end;
    InsertOrder.push_back(endNode->id);
}

//正则表达式转NFA入口
inline void regex2NFA() {
    // 双栈法，创建两个栈opStack（运算符栈）,nfaStack（nfa图栈）
    stack<char> opStack;
    stack<NFAP> nfaStack;
    // 对表达式进行类似于逆波兰表达式处理
    // 运算符：| @（） ？ +  *
    for (const char& c : OutRegex) {
        switch (c)
        {
        case ' ': // 空格跳过
            break;
        case '(':
            opStack.push(c);
            break;
        case ')':
            while (!opStack.empty() && opStack.top() != '(') {
                // 处理栈顶运算符，构建NFA图，并将结果入栈
                char op = opStack.top();
                opStack.pop();
                NFAP y = nfaStack.top();
                nfaStack.pop();
                NFAP x = nfaStack.top();
                nfaStack.pop();
                if (op == '|' || op == '@')
                    nfaStack.push(op == '|' ? unionNFA(x, y) : concatNFA(x, y));
            }
            if (opStack.empty()) {
                //cout << "括号未闭合，请检查正则表达式！";
                exit(2);
            }
            else
                opStack.pop(); // 弹出(
            break;
        case '|':
        case '@':
            // 处理运算符 | 和 @
            while (!opStack.empty() && (opStack.top() == '|' || opStack.top() == '@')
                && OpPrec[opStack.top()] >= OpPrec[c]) {
                char op = opStack.top();
                opStack.pop();
                NFAP y = nfaStack.top();
                nfaStack.pop();
                NFAP x = nfaStack.top();
                nfaStack.pop();
                // 处理栈顶运算符，构建NFA图，并将结果入栈
                nfaStack.push(op == '|' ? unionNFA(x, y) : concatNFA(x, y));
            }
            opStack.push(c);
            break;
        case '?':
        case '*':
            // 处理闭包运算符 ? + *
            // 从nfaStack弹出NFA，应用相应的闭包操作，并将结果入栈
            if (!nfaStack.empty()) {
                NFAP t = nfaStack.top();
                nfaStack.pop();
                nfaStack.push(c == '?' ? optionNFA(t) : repeatNFA(t));
            }
            else {
                //cout << "正则表达式语法错误：闭包操作没有NFA可用！";
                exit(2);
            }
            break;
        case EXIT:
            if (!nfaStack.empty()) {  // 标记单词编码cd
                NFAP t = nfaStack.top();
                nfaStack.pop();
                t.end->cd = CdCnt++;
                t.end->isExit = true;
                nfaStack.push(t);
            }
            break;
        default:  // 处理字母字符
            nfaStack.push(basicNFA(c));
        }
    }
    // 处理栈中剩余的运算符
    while (!opStack.empty()) {
        char op = opStack.top();
        opStack.pop();
        if (op == '|' || op == '@') {
            // 处理并构建运算符 | 和 .
            if (nfaStack.size() < 2) {
                //cout << "正则表达式语法错误：不足以处理运算符 " << op << "！";
                exit(2);
            }
            NFAP y = nfaStack.top();
            nfaStack.pop();
            NFAP x = nfaStack.top();
            nfaStack.pop();
            nfaStack.push(op == '|' ? unionNFA(x, y) : concatNFA(x, y));
        }
        else {
            //cout << "正则表达式语法错误：未知的运算符 " << op << "！";
            exit(2);
        }
    }
    // 最终的NFA图在nfaStack的顶部
    NFAp = nfaStack.top();
    //cout << "NFA图构建完毕" << endl;
    buildNFAStateTable();
    //cout << "状态转换表构建完毕" << endl;
}

// 判断是否含有初态终态，含有则返回对应字符串
inline string getDfaFlag(set<int>& stateSet) {
    for (auto& x : stateSet)
        if (StartNFA.find(x) != StartNFA.end())
            return "-";
        else if (EndNFA.find(x) != EndNFA.end() || ExitNFA.find(x) != ExitNFA.end())
            return "+";
    return "";
}

// 计算给定状态的ε-闭包
inline set<int> emptyClosure(int id) {
    set<int> result{ id };
    stack<int> stack;
    stack.push(id);  //将输入的状态id压入栈中
    while (!stack.empty()) {
        int current = stack.top();  //取出栈顶元素
        stack.pop();
        //获取当前状态的ε-转换集合
        set<int> eClosure = StateTable[current].m[EMPTY];
        //遍历ε-转换集合中的每一个状态
        for (const auto& t : eClosure) {
            //如果结果集合中不包含该状态
            if (result.find(t) == result.end()) {
                result.insert(t);  //将该状态加入结果集合
                stack.push(t);  //将该状态压入栈中以便后续处理
            }
        }
    }
    return result;
}


//计算给定状态和字符的闭包
inline set<int> charClosure(int id, char ch) {
    set<int> result;
    set<int> processed;
    stack<int> stack;
    stack.push(id);
    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();
        //如果当前状态已经被处理过，则跳过
        if (processed.find(current) != processed.end())
            continue;
        processed.insert(current);
        set<int> otherClosure = StateTable[current].m[ch];
        for (const auto& o : otherClosure) {
            set<int> tmp = emptyClosure(o);
            result.insert(tmp.begin(), tmp.end());
            stack.push(o);
        }
    }
    return result;
}

inline void markDFAcd() {
    for (auto& x : DFAList)
        for (auto& y : x.states)
            if (ExitNFA.find(y) != ExitNFA.end())
                if (x.cd > 0) exit(3);  // 单状态多出口
                else x.cd = ExitNFA[y];
}

inline void NFA2DFA() {
    NFA* startNFA = NFAp.start; // 获得NFA图的起始位置
    int startId = startNFA->id;   // 获得起始编号
    DFA startDFA;
    startDFA.states = emptyClosure(startId); // 初始闭包
    startDFA.flag = getDfaFlag(startDFA.states); // 判断初态终态
    queue<set<int>> newState;
    DFASetMap[startDFA.states] = CountDFA;
    if (startDFA.flag == "+")
        DFAEndState.insert(CountDFA++);
    else
        DFANotEndState.insert(CountDFA++);
    // 对每个字符进行遍历
    for (const char& ch : Chars) {
        set<int> chClosure;
        for (const int& id : startDFA.states) {
            set<int> tmp = charClosure(id, ch);
            chClosure.insert(tmp.begin(), tmp.end());
        }
        if (chClosure.empty())  // 如果这个闭包是空集没必要继续下去了
            continue;
        int presize = DFAStateSet.size();
        DFAStateSet.insert(chClosure);
        int lastsize = DFAStateSet.size();
        // 不管一不一样都是该节点这个字符的状态
        startDFA.trans[ch] = chClosure;
        // 如果大小不一样，证明是新状态
        if (lastsize > presize) {
            DFASetMap[chClosure] = CountDFA;
            newState.push(chClosure);
            if (getDfaFlag(chClosure) == "+")
                DFAEndState.insert(CountDFA++);
            else
                DFANotEndState.insert(CountDFA++);
        }
    }
    DFAList.push_back(startDFA);
    // 对后面的新状态进行不停遍历
    while (!newState.empty()) {
        // 拿出一个新状态
        set<int> ns = newState.front();
        newState.pop();
        DFA x;
        x.states = ns;  // 该节点状态集合
        x.flag = getDfaFlag(ns);
        for (const char& ch : Chars) {
            set<int> chClosure;
            for (const int& id : ns) {
                set<int> tmp = charClosure(id, ch);
                chClosure.insert(tmp.begin(), tmp.end());
            }
            if (chClosure.empty()) continue; // 如果这个闭包是空集没必要继续下去了
            int presize = DFAStateSet.size();
            DFAStateSet.insert(chClosure);
            int lastsize = DFAStateSet.size();
            // 不管一不一样都是该节点这个字符的状态
            x.trans[ch] = chClosure;
            // 如果大小不一样，证明是新状态
            if (lastsize > presize) {
                DFASetMap[chClosure] = CountDFA;
                newState.push(chClosure);
                if (getDfaFlag(chClosure) == "+")
                    DFAEndState.insert(CountDFA++);
                else
                    DFANotEndState.insert(CountDFA++);
            }
        }
        DFAList.push_back(x);
    }
    // 统一编号
    for (int i = 0; i < DFAList.size(); ++i)
        DFAList[i].id = i;
    markDFAcd();  // 标记DFA的cd
}


/*---------------------------------DFA最小化--------------------------------------*/
// 判断是否含有初态终态，含有则返回对应字符串
inline string getMinDfaFlag(const set<int>& stateSet) {
    if (stateSet.find(0) != stateSet.end())
        return "-";
    for (const int& element : DFAEndState)
        if (stateSet.find(element) != stateSet.end())
            return "+";  // 可能会有多个终态同时包含，我们只要一个
    return "";
}

// 根据字符 ch 将状态集合 node 分成两个子集合
inline void splitSet(int i, char ch) {
    set<int> result;
    auto& node = DivideSet[i];
    int s = -2, cs;
    for (const auto& state : node) {
        if (DFAList[state].trans.find(ch) == DFAList[state].trans.end())
            cs = -1; // 空集
        else {
            // 根据字符 ch 找到下一个状态
            int next_state = DFASetMap[DFAList[state].trans[ch]];
            cs = DFA2MDFA[next_state];    // 这个状态的下标是多少
        }
        if (s == -2) s = cs;   // 初始下标
        else if (cs != s)   // 如果下标不同，就是有问题，需要分出来
            result.insert(state);
    }
    // 删除要删除的元素
    for (int state : result)
        node.erase(state);
    // 都遍历完了，如果result不是空，证明有新的，加入vector中
    if (!result.empty()) {
        DivideSet.push_back(result);
        // 同时更新下标
        for (const auto& a : result)
            DFA2MDFA[a] = DivideSet.size() - 1;
    }
}

// 划分初始状态为MaxCd+1个集合(cd=0划分为初态，其余划为MaxCd个独立的终态)
inline void divideState() {
    map<int, set<int>> m;
    for (const auto& x : DFAList)
        m[x.cd].insert(x.id);
    int id = 0;
    for (const auto& p : m) {
        DivideSet.push_back(p.second);
        for (const auto& x : p.second)
            DFA2MDFA[x] = id;
        ++id;
    }
}

// 标记最小DFA的cd
inline void markMinDFAcd() {
    for (const auto& p : DFA2MDFA)
        MDFAList[p.second].cd = DFAList[p.first].cd;
}

// 最小化DFA
inline void minimizeDFA() {
    divideState();
    // 当flag为1时，一直循环
    int continueFlag = 1;
    while (continueFlag) {
        continueFlag = 0;
        int size1 = DivideSet.size();
        for (int i = 0; i < size1; i++) // 逐个字符尝试分割状态集合
            for (char ch : Chars)
                splitSet(i, ch);
        int size2 = DivideSet.size();
        if (size2 > size1) continueFlag = 1;
    }
    for (MDFACount = 0; MDFACount < DivideSet.size(); MDFACount++) {
        const auto& v = DivideSet[MDFACount];
        MDFA x;
        x.flag = getMinDfaFlag(v);
        x.id = MDFACount;
        if (x.flag == "-") MDFAStart = x.id;
        // 逐个字符
        for (const char& ch : Chars) {
            if (v.empty()) continue;
            int i = *(v.begin()); // 拿一个出来
            if (DFAList[i].trans.find(ch) == DFAList[i].trans.end()) continue;
            int next_state = DFASetMap[DFAList[i].trans[ch]];
            int thisNum = DFA2MDFA[next_state];    // 这个状态下标
            x.trans[ch] = thisNum;
        }
        MDFAList.push_back(x);
    }
    markMinDFAcd();
}

// 生成NFA表格
inline void genNFA() {
    vector<string> headers{ "Flag", "ID", "ε" };  // 设置表头
    int colCnt = headers.size();
    map<char, int> charCol{ {EMPTY, colCnt - 1} };  // 字符和第X列存起来对应
    for (const auto& c : Chars) {
        if (Code2Str.find(c) != Code2Str.end())
            headers.push_back(trim(Code2Str[c]));
        else
            headers.push_back(c == ANY ? "ANY" : STR(c));
        charCol[c] = colCnt++;
    }
    int rowCnt = StateTable.size() + 1;
    NFAGraph.resize(rowCnt, vector<string>(colCnt));
    NFAGraph[0] = headers;
    int row = 1;
    for (const auto& id : InsertOrder) {
        const nodeState& node = StateTable[id];
        NFAGraph[row][0] = node.flag;  // Flag 列
        NFAGraph[row][1] = to_string(node.id);  // ID 列
        for (const auto& entry : node.m)
            NFAGraph[row][charCol[entry.first]] = set2string(entry.second);
        ++row;
    }
}

// 生成DFA表格
inline void genDFA() {
    vector<string> headers{ "Flag", "ID", "State set" };
    int colCnt = headers.size();
    map<char, int> charCol;
    for (const auto& c : Chars) {
        if (Code2Str.find(c) != Code2Str.end())
            headers.push_back(trim(Code2Str[c]));
        else
            headers.push_back(c == ANY ? "ANY" : STR(c));
        charCol[c] = colCnt++;
    }
    int rowCnt = DFAList.size() + 1;
    DFAGraph.resize(rowCnt, vector<string>(colCnt));
    DFAGraph[0] = headers;
    int row = 1;
    for (const auto& x : DFAList) {
        DFAGraph[row][0] = x.flag;  // Flag 列
        DFAGraph[row][1] = to_string(x.id);  // 编号
        DFAGraph[row][2] = "{" + set2string(x.states) + "}";  // 状态集合
        for (const auto& entry : x.trans)  // 状态转换
            DFAGraph[row][charCol[entry.first]] = to_string(DFASetMap[entry.second]);
        ++row;
    }
}

// 生成最小DFA表格
inline void genMinDFA() {
    vector<string> headers{ "Flag", "ID", "Code", "Content" };
    int colCnt = headers.size();
    map<char, int> charCol;
    for (const auto& c : Chars) {
        if (Code2Str.find(c) != Code2Str.end())
            headers.push_back(trim(Code2Str[c]));
        else
            headers.push_back(c == ANY ? "ANY" : STR(c));
        charCol[c] = colCnt++;
    }
    int rowCnt = MDFAList.size() + 1;
    MDFAGraph.resize(rowCnt, vector<string>(colCnt));
    MDFAGraph[0] = headers;
    int row = 1;
    for (auto& x : MDFAList) {
        MDFAGraph[row][0] = x.flag;
        MDFAGraph[row][1] = to_string(x.id);
        MDFAGraph[row][2] = x.cd > 0 ? to_string(x.cd) : "";
        MDFAGraph[row][3] = x.cd > 0 ? Code2Token[x.cd] : "";
        for (const auto& entry : x.trans)
            MDFAGraph[row][charCol[entry.first]] = to_string(entry.second);
        ++row;
    }
}

// 生成子类编码
inline void genSubCode(string& r) {
    if (r.empty()) return;
    for (auto& x : split(r, '|')) {
        OutRegex += '(' + x + ')' + EXIT + '|';
        replace(x, "\\", "");  // 去除反斜杠转义符
        int k = Code2Token.size();
        PriorityOfCode[k] = 2;
        Code2Token[k] = x;
    }
}

// 生成大类编码(实义/无实义)
inline void genMainCode(string& l, string& r) {
    if (r.empty()) return;
    OutRegex += '(' + r + ')' + EXIT + '|';  //拼接到存储式子的字符串后，用|将式子连接
    bool flag = l[1] == '!';
    int k = Code2Token.size();
    PriorityOfCode[k] = 1 - flag;
    Code2Token[k] = l.substr(1 + flag);
    CodeMain[flag].push_back(k);
}

// 生成字符集
inline void genCharSet(string& l, string& r) {
    if (r.size() < 3) return;
    MapStr.push_back(l);
    r = r.substr(1, r.size() - 2);  // 去掉中括号
    replace(r, "\\-", STR(char(-2)));  // 替换转义减号\-
    replace(r, STR(RANGE), STR(char(-1)));  // 替换范围运算符
    replace(r, STR(char(-2)), "-");  // 转义减号\-转为减号-
    int n = r.size(), i = -1;
    vector<bool> v(n);
    // 参与范围运算的字符
    while ((i = r.find(char(-1), i + 1)) != -1) {
        v[i - 1] = v[i] = v[i + 1] = true;
        for (char c = r[i - 1]; c <= r[i + 1]; ++c)
            CharSet[l].insert(c);
    }
    // 补充剩余的字符
    for (i = 0; i < n; ++i)
        if (!v[i]) CharSet[l].insert(r[i]);
}

// 映射处理正则表达式
inline void mapRegex() {
    // 初始化映射表(-128~-1作为可用映射字符值, 避免冲突)
    for (int i = 0; i < MapStr.size(); ++i) {
        Code2Str[char(EXIT - 1 - i)] = MapStr[i];
        Str2Code[MapStr[i]] = char(EXIT - 1 - i);
    }
    for (const auto& x : CharSet)
        CharSetSymbol.insert(Str2Code[x.first]);
    // 使用map映射关系替换正则表达式中的符号
    for (const auto& pair : Str2Code)
        replace(OutRegex, pair.first, STR(pair.second));
    //先处理+号
    for (int i = 0; i < OutRegex.size(); i++) {
        if (OutRegex[i] == '+') {
            int k = 0, j = i;
            do {
                --j;
                if (OutRegex[j] == ')') k++;
                else if (OutRegex[j] == '(') k--;
            } while (k != 0);
            string str1 = OutRegex.substr(0, j);
            string kstr = OutRegex.substr(j, i - j);
            string str2 = OutRegex.substr(i + 1, (OutRegex.size() - i));
            OutRegex = str1 + kstr + kstr + '*' + str2;
        }
    }
    for (int i = 0; i < OutRegex.size() - 1; i++) {
        if ((isChar(OutRegex[i]) && isChar(OutRegex[i + 1]))
            || (isChar(OutRegex[i]) && OutRegex[i + 1] == '(')
            || (OutRegex[i] == ')' && (OutRegex[i + 1] == '(' || isChar(OutRegex[i + 1])))
            || (OutRegex[i] == '*' && (OutRegex[i + 1] == '(' || isChar(OutRegex[i + 1])))
            || (OutRegex[i] == '?' && (OutRegex[i + 1] == '(' || isChar(OutRegex[i + 1])))
            || (OutRegex[i] == '+' && (OutRegex[i + 1] == '(' || isChar(OutRegex[i + 1]))))
        {
            string str1 = OutRegex.substr(0, i + 1);
            string str2 = OutRegex.substr(i + 1, (OutRegex.size() - i));
            OutRegex = str1 + '@' + str2;
        }
    }
}

// 对正则表达式进行处理
inline void processRegex() {
    // 去除空格和制表符
    replace(InRegex, " ", ""), replace(InRegex, "\t", "");
    // 将文本内容按行分割成一个字符串列表
    vector<string> lines = split(InRegex, '\n');
    // 拼接生成NFA和DFA图的正则表达式
    for (const auto& line : lines) {
        if (line.empty()) continue;
        int k = line.find('=');
        string l = line.substr(0, k); // 式子名称
        string r = line.substr(k + 1);  // 式子内容
        if (l[0] == '_')  // 如果命名前有下划线，则代表该正则表达式需要生成NFA图和DFA图
            l[1] == '_' ? genSubCode(r) : genMainCode(l, r);
        else  // xxx=[a-zA-Z]格式
            genCharSet(l, r);
    }
    OutRegex.pop_back();  // 去除最后一个'|'
    //cout << "拼接后的正则表达式:\n" << outRegex << '\n';
    mapRegex();
    //cout << "处理后的正则表达式:\n" << outRegex << '\n';
}


inline void genHeader() {
    LexCode +=
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "#include <string.h>\n"
        "#include <stdbool.h>\n"
        "#define MaxCd " + to_string(CdCnt) + "\n"
        "int n, m;\n"
        "bool flag;\n"
        "char c, str[128];\n"
        "int curr[MaxCd], next[MaxCd];\n"
        "FILE* input, * output;\n";
}
inline void genState() {
    LexCode +=
        "struct State { int cd, pr; }\n"
        "S[] = {";
    int k = 0;
    for (auto& x : MDFAList) {
        if (k++ % 5 == 0) LexCode += "\n\t";
        LexCode += "{" + to_string(x.cd) + ", " + to_string(PriorityOfCode[x.cd]) + "}, ";
    }
    LexCode.erase(LexCode.end() - 2, LexCode.end());
    LexCode += "\n};\n";
}
inline void genTools() {
    LexCode +=
        "void append() {\n"
        "\tsize_t len = strlen(str);\n"
        "\tstr[len] = c, str[len + 1] = '\\0';\n"
        "}\n"
        "int get_cd() {\n"
        "\tint cd = 0, pr = -1;\n"
        "\tfor (int i = 0; i < n; ++i)\n"
        "\t\tif (S[curr[i]].pr > pr)\n"
        "\t\t\tcd = S[curr[i]].cd, pr = S[curr[i]].pr;\n"
        "\treturn cd;\n"
        "}\n"
        "bool has_end(int id) {\n"
        "\tfor (int i = 0; i < n; ++i)\n"
        "\t\tif (curr[i] != id && S[curr[i]].cd > 0)\n"
        "\t\t\treturn true;\n"
        "\treturn false;\n"
        "}\n";
}
inline void genIsCharSet() {
    for (const auto& p : CharSet) {
        auto v = sortCharSet(p.second);
        string code0, code1;
        LexCode +=
            "bool is_" + p.first + "() {\n"
            "\treturn";
        for (int i = 0; i < v[0].size(); i += 2)
            code0 += " (c >= \'" + escape(v[0][i]) + "\' && c <= \'" + escape(v[0][i + 1]) + "\') ||";
        if (!code0.empty()) code0.erase(code0.end() - 3, code0.end());
        if (!v[1].empty()) {  // 单字符
            for (const auto& c : v[1])
                code1 += "|| c == \'" + escape(c) + "\' ";
            code1.pop_back();
        }
        if (code0.empty()) code1.erase(code1.begin(), code1.begin() + 2);
        else if (!code1.empty()) code0 += "\n\t\t";
        LexCode += code0, LexCode += code1;
        LexCode +=
            ";\n"
            "}\n";
    }
}
inline void genCores() {
    LexCode +=
        "void new_state() {\n"
        "\tstr[0] = '\\0';\n"
        "\tn = 1, curr[0] = " + to_string(MDFAStart) + ";\n"
        "}\n"
        "void end_state() {\n"
        "\tint cd = get_cd();\n"
        "\tswitch (cd) {\n"
        "\tcase 0: return;\n";
    for (auto& x : CodeMain[1])
        LexCode += "\tcase " + to_string(x) + ": break;\n";
    for (auto& x : CodeMain[0])
        LexCode += "\tcase " + to_string(x) + ": fprintf(output, \"%d\\t\\t%s\\n\", cd, str); break;\n";
    LexCode +=
        "\tdefault: fprintf(output, \"%d\\n\", cd);\n"
        "\t}\n"
        "\tungetc(c, input);\n"
        "\tn = 0;\n"
        "}\n"
        "bool next_state(int id) {\n"
        "\tint k = m, x = -1;\n"
        "\tswitch (id) {\n";
    for (auto& x : MDFAList) {
        int k = 0;
        string anyCode, setCode, charCode;
        for (const auto& p : x.trans) {
            if (p.second == -1) continue;
            string m = to_string(p.second);
            if (p.first == ANY) {
                if (x.id == p.second) anyCode += "\t\tif (has_end(id)) return false;\n";
                anyCode += "\t\tnext[m++] = " + m + ";\n";
            }
            else if (CharSetSymbol.find(p.first) != CharSetSymbol.end())
                setCode += "\t\tif (is_" + Code2Str[p.first] + "()) next[m++] = " + m + ";\n";
            else {
                if (charCode.empty()) charCode += "\t\tswitch (c) {\n";
                string n = Code2Str.find(p.first) != Code2Str.end() ? trim(Code2Str[p.first]) : STR(p.first);
                charCode += "\t\tcase '" + n + "': next[m++] = " + m + "; break;\n";
            }
        }
        if (!anyCode.empty() || !setCode.empty() || !charCode.empty()) {
            if (!charCode.empty()) charCode += "\t\t}\n";
            LexCode += "\tcase " + to_string(x.id) + ":\n";
            LexCode += anyCode, LexCode += setCode, LexCode += charCode;
            LexCode += "\t\tbreak;\n";
        }
    }
    LexCode +=
        "\t}\n"
        "\treturn k != m;\n"
        "}\n";
}
inline void genMain() {
    LexCode +=
        "void coding() {\n"
        "\twhile (c = fgetc(input)) {\n"
        "\t\tif (n == 0) new_state();\n"
        "\t\tm = 0, flag = false;\n"
        "\t\tfor (int i = 0; i < n; ++i)\n"
        "\t\t\tflag |= next_state(curr[i]);\n"
        "\t\tflag ? append() : end_state();\n"
        "\t\tn = m;\n"
        "\t\tfor (int i = 0; i < n; ++i)\n"
        "\t\t\tcurr[i] = next[i];\n"
        "\t\tif (c == EOF) break;\n"
        "\t}\n"
        "\tfprintf(output, \"0\");\n"
        "}\n"
        "int main(int argc, char* argv[]) {\n"
        "\tif (argc < 3) exit(1);\n"
        "\tinput = fopen(argv[1], \"r\");\n"
        "\tif (input == NULL) {\n"
        "\t\tprintf(\"Failed to open input file\");\n"
        "\t\texit(1);\n"
        "\t}\n"
        "\toutput = fopen(argv[2], \"w\");\n"
        "\tif (output == NULL) {\n"
        "\t\tprintf(\"Failed to open output file\");\n"
        "\t\tfclose(input);\n"
        "\t\texit(1);\n"
        "\t}\n"
        "\tcoding();\n"
        "\tfclose(input);\n"
        "\tfclose(output);\n"
        "\treturn 0;\n"
        "}";
}
// 生成词法分析源程序
inline void genLexer() {
    genHeader(), genState(), genTools();
    genIsCharSet(), genCores(), genMain();
}

inline void Run() {
    processRegex();  // 处理正则表达式
    regex2NFA();  //正则表达式转NFA
    NFA2DFA();  // NFA转DFA
    minimizeDFA();  // 最小化DFA
    genNFA(), genDFA(), genMinDFA(), genLexer();
    delNFAs();
}
inline void Print() {
    string s;
    cout << "---NFA---\n";
    for (const auto& x : NFAGraph) {
        for (const auto& y : x)
            s += y + '\t';
        s.pop_back();
        s += '\n';
    }
    cout << s;
    s.clear();
    cout << "---DFA---\n";
    for (const auto& x : DFAGraph) {
        for (const auto& y : x)
            s += y + '\t';
        s.pop_back();
        s += '\n';
    }
    cout << s;
    s.clear();
    cout << "---MinDFA---\n";
    for (const auto& x : MDFAGraph) {
        for (const auto& y : x)
            s += y + '\t';
        s.pop_back();
        s += '\n';
    }
    cout << s;
}
// 导出词法分析源程序和单词编码映射表
inline void Export() {
    juce::File appDataDir = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("CompilationFinalData");
    if (!appDataDir.isDirectory()) appDataDir.createDirectory();
    juce::File lexerCFile = appDataDir.getChildFile("Lexer.c");
    juce::File mapFile = appDataDir.getChildFile("Map.txt");
    lexerCFile.replaceWithText(LexCode);
    juce::String mapContent;
    for (int i = 0; i < CodeMain[0].size(); ++i)
        mapContent << CodeMain[0][i] << (i < CodeMain[0].size() - 1 ? ' ' : '\n');
    for (const auto& p : Code2Token)
        mapContent << p.first << '\t' << p.second << '\n';
    mapFile.replaceWithText(mapContent);
}


