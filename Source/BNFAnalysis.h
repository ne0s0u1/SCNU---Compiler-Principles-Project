#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include <map>
#include <unordered_map>
#include "JuceHeader.h"
using namespace std;
#define umap unordered_map  // 定义别名
#define SUFFIX "'"  // 扩充文法的后缀名
#define EMPTY "@"  // 空串符
#define END "$"  // 终结符
#define ACCEPT "acc"  // 接受动作(这个在LALR(1)分析表的时候使用)
#define OR string(1, char(-2))  // 转义或'|'的映射标识
inline const string Blank = " \t\n";
inline vector<string> gramline;  // 原始输入文法内容，按行获取
// 单条文法结构
struct GRAM {
    string left;  // 文法规则的左部(都为非终结符)
    vector<string> right;  // 文法规则右部的组成tokens(有先后顺序，通过扫描处理，将右部拆解为若干个独立的tokens)
    // 扫描前 A->aB|c
    // 扫描后 GRAM gram = { "A", {"a", "B", "|", "c"} }

    // 用来判定两个文法是否是相同的(后续会用上)
    bool operator==(const GRAM& other) const {
        return left == other.left && right == other.right;
    }
    // 用来判定两个文法是否是不同的(后续会用上)
    bool operator!=(const GRAM& other) const {
        return left != other.left || right != other.right;
    }
};
// Grams中将消除|或符号和直接左递归
inline vector<GRAM> Grams;
// LR(1)项目集中的文法结构
struct LR1GRAM {
    GRAM gram;
    string term;  // 终结符
    vector<string> former;  // 已经接受过的token
    bool operator==(const LR1GRAM& other) const {
        return term == other.term && former == other.former && gram == other.gram;
    }
};
// LALR(1)项目集中的文法结构
struct LALR1GRAM {
    GRAM gram;
    set<string> terms;  // 前瞻终结符集合
    vector<string> former;  // 已经接受过的token
};
// LR(1)项目集结构
struct LR1ITEM {
    int id;
    vector<LR1GRAM> LRGrams;
    int size() { return LRGrams.size(); }
};
// LALR(1)项目集结构
struct LALR1ITEM {
    int id;
    vector<LALR1GRAM> LRGrams;
    int size() { return LRGrams.size(); }
};
// DFA的边信息
struct EDGE {
    int x, y;
    string s;
};
// 通过文法的左部来查找文法，不存在返回-1
inline int getGramID(const string& name) {
    for (int i = 0; i < Grams.size(); ++i)
        if (Grams[i].left == name)
            return i;
    return -1;
}
// 通过文法的左部和右部共同查找
inline int getGramID(const GRAM& gram) {
    for (int i = 0; i < Grams.size(); ++i)
        if (Grams[i] == gram)
            return i;
    return -1;
}
// 判断是终结符还是非终结符
inline bool isNonterm(const string& name) {
    return getGramID(name) != -1;
}

inline bool isTerminal(const string& name) {
    return getGramID(name) == -1;
}
// 判断空串
inline bool isEmpty(const string& name) {
    return name == EMPTY;
}
// 判断元素x是否在集合A中
inline bool isInSet(const string& x, const set<string>& A) {
    return A.find(x) != A.end();
}
inline bool isInVec(const string& x, const vector<string>& A) {
    for (auto& y : A)
        if (x == y) return true;
    return false;
}
// 合并两个集合，右集合的元素(除了except外)都添加到左集合中
inline void mergeSet(set<string>& A, const set<string>& B, string except = "") {
    for (auto& x : B)
        if (x != except) A.insert(x);
}
// 判断字符是否为' ', '\n', '\t'
inline bool isBlank(char c) {
    return Blank.find(c) != -1;
}
// 清除前后缀空格
inline string stripBlank(const string& s) {
    int i = s.find_first_not_of(Blank);
    int j = s.find_last_not_of(Blank);
    return i == -1 ? "" : s.substr(i, j - i + 1);
}
inline void replaceit(string& str, const string& x, const string& y) {
    int i = 0;
    while ((i = str.find(x, i)) != -1) {
        str.replace(i, x.size(), y);
        i += y.size();
    }
}



//scan.h

typedef enum { START, DONE, KEEP, NOTEND, ENDLINE, ENDLINES } State;
// START 开始接收字符
// DONE 完成一个Token的接收
// NONTERM 当前要接收的Token是非终结符
// TERMINAL 当前要接收的Token是终结符
// KEEP 持续接受字符，遇到空格停止(在第二种书写形式中使用)
// OR '|'或，特殊字符
// NOTEND 还没结束扫描的状态
// ENDLINE 扫描完当前行的状态
// ENDLINES 扫描完全部行lines的状态

inline State scanState = START, endState = NOTEND;
inline int lineID = 0, charID = 0;
inline GRAM newGram;


inline int nextChar() {
    if (charID >= gramline[lineID].size()) {
        endState = (lineID >= gramline.size() - 1 ? ENDLINES : ENDLINE);
        return EOF;
    }
    return gramline[lineID][charID++];
}
// 这里偷个懒，直接把"->"左边的非空格字符都当做文法的左部
inline void getLeft() {
    int i = gramline[lineID].find("->");
    charID = i + 2;
    newGram.left = stripBlank(gramline[lineID].substr(0, i));
}
// 获取文法右部
inline void getRight() {
    bool save;
    string token;
    scanState = START;  // 初始化扫描状态
    endState = NOTEND;  // 初始化结束状态
    while (scanState != DONE) {
        char c = nextChar();
        save = true;  // 保存字符，c追加到token后
        switch (scanState) {
        case START:
            if (isBlank(c))
                save = false;
            else if (c == '|')
                scanState = DONE;
            else if (c == EOF)  // 一开始就结束，说明是空串
                return;
            else
                scanState = KEEP;
            break;
        case KEEP:
            if (isBlank(c) || c == EOF)
                save = false, scanState = DONE;
            break;
        case DONE:
        default: /* should never happen */
            //cout << "Scanner Bug: state = " << scanState << '\n';
            scanState = DONE;
            break;
        }
        if (save) token += c;
    }
    // 更新文法右部
    newGram.right.push_back(token);
}

inline void scanGrammar() {
    for (auto& line : gramline)  // 替换转义或'|'
        replaceit(line, "\\|", OR);
    while (endState != ENDLINES) {
        endState = NOTEND;
        getLeft();
        while (endState == NOTEND)
            getRight();
        Grams.push_back(newGram);
        newGram.right.clear();
        ++lineID, charID = 0;
    }
}

// 按或符号划分文法
inline void splitGrammarRules() {
    vector<GRAM> grams = Grams;  // 临时变量
    Grams.clear();  // 清空原来的文法
    for (auto& gram : grams) {
        vector<string> tokens;  // 记录当前被拆分后获得的右部tokens
        for (auto& x : gram.right) {
            if (x != "|") {  // 没遇到"|"就继续添加右部
                tokens.push_back(x);
                continue;
            }
            Grams.push_back({ gram.left, tokens });  // 遇到"|"，新增文法
            tokens.clear();  // 清空当前右部记录，准备下一次的遍历
        }
        // (处理末尾数据)如果还有剩余的右部，直接加入到Grams中
        if (!tokens.empty()) Grams.push_back({ gram.left, tokens });
    }
    for (auto& gram : Grams)  // 替换回转义或'|'
        for (auto& x : gram.right)
            if (x == OR) x = "|";
}

// 扩充开始文法，如A->a，新增A'->A  (先消除左递归，再扩充文法)
inline void extendStartGram() {
    string newName = Grams[0].left;
    while (isNonterm(newName))  // 如果新名称已存在，就一直往后追加suf
        newName += SUFFIX;
    Grams.insert(Grams.begin(), { newName, {Grams[0].left} });
}


//fset.h

// First和Follow集合
inline umap<string, set<string>> First, Follow;

// 获取Fset集合的大小总和
inline int FsetAllSize(umap<string, set<string>>& fset) {
    int size = 0;
    for (auto& x : fset)
        size += x.second.size();
    return size;
}
// 迭代构建全部First集合
inline void computeAllFirstSets() {
    // 单独求终结符的First集合(这一步不可少，否则会有遗漏)
    for (auto& gram : Grams)
        for (auto& token : gram.right)
            if (isTerminal(token)) First[token] = { token };  // 终结符的First集合只有自身
    int lastSize = -1, newSize = 0;
    while (lastSize != newSize) {
        for (auto& gram : Grams) {
            bool flag = false;  // 标志符，判断当前右部的token的First(token)是否存在@，如果存在，就得继续往后遍历右部
            for (auto& token : gram.right) {  // 遍历文法的右部
                mergeSet(First[gram.left], First[token], EMPTY);  // 把First[token] - @合并到First[左部]
                flag = isInSet(EMPTY, First[token]);  // 如果@在First[token]，继续往后遍历
                if (!flag) break;  // 如果@不在First[token]，停止遍历
            }
            if (flag) First[gram.left].insert(EMPTY);  // 如果遍历完右部，flag还是true,说明@∈First[右部]
        }
        lastSize = newSize, newSize = FsetAllSize(First);  // 更新全部First集合的大小
    }
}
// 迭代构建全部Follow集合
inline void computeAllFollowSets() {
    Follow[Grams[0].left] = { END };  // 初始化(默认第一条文法的左部为终结符)
    int lastSize = -1, newSize = 1;
    while (lastSize != newSize) {  // 如果全部Follow集合的大小不再变化，就跳出计算
        for (auto& x : Grams) {  // 遍历每一条文法规则
            for (int i = 0; i < x.right.size(); ++i) {  // 遍历右部的每一个token
                if (isTerminal(x.right[i])) continue;  // 只有非终结符才有Follow集合
                // 判断@是否属于First(Xi+1Xi+2...Xn)
                // 这里要对x.tokens[i]是否为最右端的token做特判
                bool flag = (i == x.right.size() - 1);
                for (int j = i + 1; j < x.right.size(); ++j) {  // 遍历token[i]后的tokens
                    mergeSet(Follow[x.right[i]], First[x.right[j]], EMPTY);
                    flag = isInSet(EMPTY, First[x.right[j]]);  // 判断@是否在First[Xj]中
                    if (!flag) break;
                }
                if (flag) mergeSet(Follow[x.right[i]], Follow[x.left]);  // 将Follow[A]合并到Follow[Xi]中
            }
        }
        lastSize = newSize, newSize = FsetAllSize(Follow);  // 更新新旧Follow集合大小的总和
    }
}

//analysis.h

// LR(1)构建顺序：
// 构建项目集→构建DFA→构建分析表
inline vector<LR1ITEM> LR1Items;
inline vector<LALR1ITEM> LALR1Items;
// LR1的id映射LALR1的id
inline map<int, int> LR2LA;
// LALR1的项目数量
inline int LALR1Cnt;

inline vector<EDGE> LR1Edges;
inline vector<EDGE> LALR1Edges;

// 在go函数中使用
inline vector<string> nextTokens;

// 在mergeCores函数中使用
inline vector<int> sameCores;
inline vector<set<string>> cores;

//map<pair<int, string>, string> LR1ACTION, LR1GOTO;
inline map<pair<int, string>, string> ACTION, GOTO;


/**
 * @brief 重置所有与BNF分析相关的全局变量，用于支持多次分析，避免数据残留。
 */
inline void resetAnalysis() {
    // 扫描器状态
    gramline.clear();
    scanState = START;
    endState = NOTEND;
    lineID = 0;
    charID = 0;

    // 文法和集合
    Grams.clear();
    First.clear();
    Follow.clear();

    // LR/LALR 状态机
    LR1Items.clear();
    LALR1Items.clear();
    LR1Edges.clear();
    LALR1Edges.clear();
    LR2LA.clear();
    LALR1Cnt = 0;

    // LR/LALR 辅助数据
    nextTokens.clear();
    sameCores.clear();
    cores.clear();

    // 最终分析表
    ACTION.clear();
    GOTO.clear();
}

inline bool isLR1InItem(int id, const LR1GRAM& A) {
    for (auto& x : LR1Items[id].LRGrams)
        if (x == A) return true;
    return false;
}

inline int getItemID(const LR1ITEM& A) {
    for (auto& item : LR1Items)
        if (item.LRGrams == A.LRGrams)
            return item.id;
    return -1;
}
// 求CLOSURE(I)
inline void closure(int id) {
    int lastSize = -1, newSize = LR1Items[id].size();
    while (lastSize != newSize) {  // 如果该项目的文法数量不再变化，就跳出循环
        lastSize = newSize;
        for (int i = 0; i < newSize; ++i) {
            if (LR1Items[id].LRGrams[i].gram.right.empty()) continue;  // 如果没有待接收的tokens了，就跳过
            GRAM gram = LR1Items[id].LRGrams[i].gram;  // 创建GRAM变量记录当前项目的LR(1)文法[i]的文法规则
            string newName = gram.right[0];  // 记录要接收的第一个
            if (isTerminal(newName)) continue;  // 首个token是终结符就不用再求closure
            for (auto& newGram : Grams) {  // 遍历原有的全部文法规则
                if (newGram.left != newName) continue;  // 如果当前文法的左部不等于newName，跳过
                // 求下一个可以接收的终结符
                // 如果当前的token已经是最后一个待接收token了，那就继承之前的下一个终结符
                set<string> terms = gram.right.size() > 1 ?
                    First[gram.right[1]] : set<string>({ LR1Items[id].LRGrams[i].term });
                for (auto& newTerm : terms) {  // 新增文法
                    LR1GRAM newLR1({ newGram, newTerm });
                    if (isLR1InItem(id, newLR1)) continue;  // 如果当前文法已经存在该项目，跳过
                    LR1Items[id].LRGrams.push_back(newLR1);
                }
            }
        }
        newSize = LR1Items[id].size();  // 更新当前的文法数量
    }
}

// LR1Items[id]接收token后，得到新的newItem=LR1Items[newID]
inline void go(int id, const string& token) {
    bool flag = false;  // 跳转是否成功的标识符
    int newID = LR1Items.size();  // 创建新的ID
    LR1ITEM newItem({ newID });  // 新建一个LR(1)项目
    for (auto& x : LR1Items[id].LRGrams) {  // 遍历起点项目的所有文法
        // 如果待接收的tokens为空 或者 第一个待接收的token不等于当前的目标token，则跳过
        if (x.gram.right.empty() || x.gram.right[0] != token) continue;
        flag = true;  // 跳转成功
        GRAM newGram = x.gram;  // 记录变量
        vector<string> newFormer = x.former;  // 记录变量
        newFormer.push_back(newGram.right[0]);  // 把原来待接收的tokens第一个token移到新的former中
        newGram.right.erase(newGram.right.begin());  // 删除待接收的tokens中第一个token
        newItem.LRGrams.push_back({ newGram, x.term , newFormer });  // 向新的item中加入新的文法
    }
    if (!flag) return;  // 跳转失败，直接结束
    LR1Items.push_back(newItem);  // 加入新项目item
    closure(newID);  // 紧接着求这个新项目的CLOSURE
    int tmpID = getItemID(LR1Items[newID]);  // 求新的item在项目集中第一次出现时的下标
    // 如果找到的下标不等于新的ID，说明该item在接收token后成环，然后去掉这个新加的item
    if (newID != tmpID) LR1Items.pop_back();
    LR1Edges.push_back({ id, tmpID, token });  // 添加边数据
}

inline void getNextTokens(int id) {
    nextTokens.clear();
    for (auto& x : LR1Items[id].LRGrams) {
        if (x.gram.right.empty()) continue;
        if (!isInVec(x.gram.right[0], nextTokens))
            nextTokens.push_back(x.gram.right[0]);
    }
}
// 构建LR(1)的项目集
inline void buildLR1Item() {
    LR1Items.push_back({ 0, {{Grams[0], END}} });  // 用扩充的开始文法初始化LR(1)项目集
    closure(0);  // 立即求新item的CLOSURE
    int id = 0;
    while (id < LR1Items.size()) {  // 如果没有新增的item，id会到达临界值而跳出循环
        getNextTokens(id);  // 获取Items[id]下一次可以接收的tokens
        for (auto& token : nextTokens)
            go(id, token);  // 跳转到新item
        ++id;
    }
}

inline void mergeGram(LALR1GRAM& LA, const LR1GRAM& LR) {
    if (LA.terms.empty()) {
        LA.former = LR.former;
        LA.gram = LR.gram;
    }
    LA.terms.insert(LR.term);
}
// 将LR(1)文法转换为字符串
inline string gramToStr(const LR1GRAM& x) {
    string str;
    str += x.gram.left + "->";
    for (auto& y : x.gram.right)
        str += y + ' ';
    str += "former:";
    for (auto& y : x.former)
        str += ' ' + y;
    return str;
}
// 对LR(1)项目A合并相同核心的文法，成为LALR(1)项目B
inline void mergeLRToLALRItem(LR1ITEM& A, LALR1ITEM& B) {
    for (auto& x : A.LRGrams) {
        bool flag = true;  // 如果B中不存在与x具有相同核心的文法，可以直接把文法x加入B中
        for (auto& y : B.LRGrams) {
            if (x.former != y.former || x.gram != y.gram) continue;  // 如果文法y不具有和x相同的核心，跳过
            flag = false;  // x和y的核心相同，就不要直接把x加入B中(避免重复)，我们只需要追加终结符term就行
            y.terms.insert(x.term);  // 加入新的终结符
        }
        if (flag) B.LRGrams.push_back({ x.gram, {x.term}, x.former });  // 全新的文法核心，直接把文法x加入B中
    }
}
// 将当前记录的相同核心的LR(1)项目合并成LALR(1)项目，并加入到LALR(1)项目集中
inline void addLALR1Item() {
    LALR1ITEM Item;
    for (auto& x : sameCores) {
        LR2LA[LR1Items[x].id] = LALR1Cnt;
        mergeLRToLALRItem(LR1Items[x], Item);
    }
    Item.id = LALR1Cnt++;
    LALR1Items.push_back(Item);
}

// 不断遍历LR(1)项目集，合并相同核心项
inline void mergeCores() {
    for (auto& item : LR1Items) {
        set<string> s;
        for (auto& gram : item.LRGrams)
            s.insert(gramToStr(gram));
        cores.push_back(s);
    }
    int n = LR1Items.size();
    vector<bool> v(n);
    while (true) {
        int k = -1;
        sameCores.clear();
        for (int i = 0; i < n; ++i) {
            if (v[i]) continue;  // 如果已访问，就跳过
            if (sameCores.empty()) {  // 如果当前没有记录过任何一个LR(1)项目
                sameCores.push_back(i);  // 直接把item加入到samCores中
                v[i] = true;  // 更新标记
                k = i;
                continue;
            }
            if (cores[k] == cores[i]) { // 如果当前项目的核心和sameCores中的第一个相同
                sameCores.push_back(i);  // 追加新item到sameCores中
                v[i] = true;
            }
        }
        if (sameCores.empty()) return;  // 如果所有的item都进行过合并操作，就结束
        addLALR1Item();  // 添加新的LALR(1)项目到LALR(1)项目集中
    }
}

inline void buildLALR1Edges() {  // 构造LALR1边
    // 二维数组v[n][n]，v[x][y]=true说明项目x到项目y的边已经记录过了
    vector<vector<bool>> v(LALR1Cnt, vector<bool>(LALR1Cnt, false));
    for (auto& e : LR1Edges) {
        // 获取当前的LR(1)边的端点项目对应的起点LALR(1)项目的id
        int x = LR2LA[e.x], y = LR2LA[e.y];
        if (v[x][y]) continue;  // 如果已经遍历过，则跳过
        v[x][y] = true;  // 更新标记
        LALR1Edges.push_back({ x, y, e.s });  // 添加新边
    }
}

inline void buildLALR1Item() {
    mergeCores();  // 合并相同核心
    buildLALR1Edges();  // 构造边
}

// 构建LALR(1)分析表
inline void buildLALR1Table() {
    // 归约动作（！！！注意，先记录归约动作，因为LALR1仍存在移进-归约冲突，此时优先进行移进动作，所以后记录移进动作，可以覆盖掉归约动作）
    // 如果某一LALR(1)文法没有待接收tokens(已经接收完毕)，那就进入归约动作
    for (auto& item : LALR1Items) {  // 遍历每一个项目
        for (auto& LR1 : item.LRGrams) {  // 遍历每一个项目的每一条文法
            if (!LR1.gram.right.empty()) continue;  // 如果还有待接收的tokens，则跳过
            GRAM gram({ LR1.gram.left, LR1.former });  // 记录该文法信息
            int id = getGramID(gram);  // 获取该文法在全部文法Grams中的下标ID
            for (auto& term : LR1.terms)  // 要对所有可接收的终结符都进行归约
                ACTION[{item.id, term}] = 'r' + to_string(id);  // 更新表格
        }
    }
    // 移进动作 or 跳转动作
    for (auto& e : LALR1Edges) {  // 遍历每一条边
        if (isTerminal(e.s))  // 如果接收的token是终结符，则为移进动作
            ACTION[{e.x, e.s}] = 's' + to_string(e.y);  // 更新表格
        else   // 如果接收的token是非终结符，则为跳转动作
            GOTO[{e.x, e.s}] = to_string(e.y);  // 更新表格(跳转动作前缀不用标记)
    }
    // 接受动作
    // 例如，扩充开始文法[S'->S, $]，它是位于LALR1Items[0]的，在接收一个S后，跳转至LALR1Items[1]
    // 此时跳转至LALR1Items[1]再接收一个结束符$，就完成了整体文法的归约(相当于文法归约的统一出口)
    // 我们定义这一个出口为接受动作，用"acc"表示
    ACTION[{1, END}] = ACCEPT;
}

//output.h

// 根据name打印单个Fset集合
inline void printFset(unordered_map<string, set<string>>& fset, string key, ofstream& ofs) {
    ofs << key << '\t';
    string s;
    for (auto& x : fset[key])
        s += x + ' ';
    s.back() = '\n';
    ofs << s;
}

// 打印全部First集合
inline void printFsetAll(unordered_map<string, set<string>>& fset, const string& root) {
    ofstream ofs;
    if (fset == First)
        ofs.open(root + "/FirstSet.txt");
    else
        ofs.open(root + "/FollowSet.txt");
    for (auto& x : fset)
        printFset(fset, x.first, ofs);
    ofs.close();
}

// 根据ID打印单个LR(1)项目
inline void printLR1Item(int id, ofstream& ofs) {

    ofs << "--Item[" << id << "]--\n";
    for (auto& x : LR1Items[id].LRGrams) {
        ofs << x.gram.left << " ->";
        for (auto& y : x.former)
            ofs << ' ' << y;
        ofs << "·";
        for (auto& y : x.gram.right)
            ofs << ' ' << y;
        ofs << ", " << x.term << '\n';
    }
}

// 打印LR(1)项目集
inline void printLR1ItemAll(const string& root) {
    ofstream ofs(root + "/LR1.txt");
    for (int i = 0; i < LR1Items.size(); ++i)
        printLR1Item(i, ofs);
    ofs.close();
}

// 打印LALR(1)分析表
inline void printLALR1Table(const string& root1 , const string& root2) {
    ofstream ofs1(root1 + "/Action.txt");
    ofs1 << "--ACTION--\n";
    for (auto& x : ACTION)
        ofs1 << x.first.first << '\t' << x.first.second << '\t' << x.second << '\n';
    ofs1.close();
    ofstream ofs2(root2 + "/Goto.txt");
    ofs2 << "--GOTO--\n";
    for (auto& x : GOTO)
        ofs2 << x.first.first << '\t' << x.first.second << '\t' << x.second << '\n';
    ofs2.close();
}

// 打印LALR(1)项目集
inline void printLALR1ItemAll(const string& root) {
    ofstream ofs(root + "/LALR1.txt");
    for (auto& item : LALR1Items) {
        ofs << "--Item[" << item.id << "]--\n";
        for (auto& x : item.LRGrams) {
            ofs << x.gram.left << " ->";
            for (auto& y : x.former)
                ofs << ' ' << y;
            ofs << "·";
            for (auto& y : x.gram.right)
                ofs << ' ' << y;
            ofs << ", ";
            string str;
            for (auto& term : x.terms)
                str += term + '/';
            str.pop_back();
            ofs << str << '\n';
        }
    }
    ofs.close();
}

// 打印LR(1)DFA的边
inline void printLR1Edges(const string& root) {
    ofstream ofs(root + "/LR1Edges.txt");
    for (auto& e : LR1Edges)
        ofs << e.x << '\t' << e.s << '\t' << '\t' << e.y << '\n';
    ofs.close();
}

// 打印LALR(1)DFA的边
inline void printLALR1Edges(const string& root) {
    ofstream ofs(root + +"/LALR1Edges.txt");
    for (auto& e : LALR1Edges)
        ofs << e.x << '\t' << e.s << '\t' << '\t' << e.y << '\n';
    ofs.close();
}

inline void RunAll() {
    scanGrammar();  // 扫描全部文法
    splitGrammarRules();  // 按|拆分文法
    computeAllFirstSets();  // 求全部First集合
    computeAllFollowSets();  // 求全部Follow集合
    buildLR1Item();  // 构建LR(1)项目集
    buildLALR1Item();  // 构建LALR(1)项目集
    buildLALR1Table();  // 构建LALR(1)分析表
}

inline void PrintBNF() {
    juce::File appDataDir = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("CompilationFinalData");
    std::string outpuTPath = appDataDir.getFullPathName().toStdString();

    printFsetAll(First, outpuTPath);

    printFsetAll(Follow, outpuTPath);

    printLR1ItemAll(outpuTPath);

    printLR1Edges(outpuTPath);

    printLALR1ItemAll(outpuTPath);

    printLALR1Edges(outpuTPath);

    printLALR1Table(outpuTPath,outpuTPath);

}
// 导出文法和LALR1分析表
inline void ExportLALR() {
    juce::File appDataDir = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("CompilationFinalData");
    if (!appDataDir.isDirectory()) appDataDir.createDirectory();
    juce::File GramsFile = appDataDir.getChildFile("Grams.txt");
    juce::File TableFile = appDataDir.getChildFile("Table.txt");
    juce::String GramsStr;
    juce::String TableStr;
    for (auto& x : Grams) {
        GramsStr << x.left;
        for (auto& y : x.right)
            GramsStr << ' ' << y;
        GramsStr << '\n';
    }
    GramsFile.replaceWithText(GramsStr);
    for (auto& x : ACTION)
        TableStr << x.first.first << ' ' << x.first.second << ' ' << x.second << '\n';
    for (auto& x : GOTO)
        TableStr << x.first.first << ' ' << x.first.second << ' ' << x.second << '\n';
    TableFile.replaceWithText(TableStr);
}


//int main(int argc, char* argv[]) {
//    if (argc < 2) {  // 没有文件路径参数传入，只进行编译
//        cout << "Usage: " << argv[0] << " <filename>\n";
//        exit(0);
//    }
//    string path = argv[1];  // 文法文件路径
//    ifstream file(path);  // 打开文件
//    if (!file.is_open()) {
//        cout << "Error opening file\n";
//        exit(0);
//    }
//    string line;
//    while (getline(file, line)) {
//        if (line.empty()) continue;  // 跳过空行
//        gramline.push_back(line);
//    }
//    file.close();  // 关闭文件
//    Run();  // 文法分析
//    Print();  // 打印结果
//    Export(argc == 3 ? argv[2] : ".");
//    return 0;
//}
