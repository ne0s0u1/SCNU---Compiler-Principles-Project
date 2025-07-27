#include <JuceHeader.h>
#include "ParserComponent.h"

//==============================================================================
ParserComponent::ParserComponent()
{
    setSize(1400, 950);

    addAndMakeVisible(BNFButton);
    addAndMakeVisible(AnalysisButton);
    addAndMakeVisible(BNFoutput);
    addAndMakeVisible(LALRButton);
    addAndMakeVisible(TreeEditor);
    //addAndMakeVisible(Treeoutput);
    addAndMakeVisible(TreeDefineButton);
    addAndMakeVisible(FirstSetButton);
    addAndMakeVisible(FollowSetButton);
    addAndMakeVisible(LR1Button);
    addAndMakeVisible(LALR1Button);
    addAndMakeVisible(ActionButton);
    addAndMakeVisible(GotoButton);
    addAndMakeVisible(ProcessButton);
    addAndMakeVisible(ShowTreeButton);

    BNFoutput.setMultiLine(true);
    BNFoutput.setReturnKeyStartsNewLine(true);
    BNFoutput.setReadOnly(true);
    TreeEditor.setMultiLine(true);
    TreeEditor.setReturnKeyStartsNewLine(true);
    TreeEditor.setReadOnly(false);
    Treeoutput.setMultiLine(true);
    Treeoutput.setReturnKeyStartsNewLine(true);
    Treeoutput.setReadOnly(false);

    BNFButton.setButtonText(juce::CharPointer_UTF8("\xe5\x8a\xa0\xe8\xbd\xbd""BNF\xe6\x96\x87\xe6\xb3\x95"));
    AnalysisButton.setButtonText(juce::CharPointer_UTF8("\xe7\x94\x9f\xe6\x88\x90\xe8\xaf\xad\xe6\xb3\x95\xe6\xa0\x91"));
    LALRButton.setButtonText(juce::CharPointer_UTF8("LALR\xe5\x88\x86\xe6\x9e\x90"));
    TreeDefineButton.setButtonText(juce::CharPointer_UTF8(juce::CharPointer_UTF8("\xe8\xaf\xad\xe6\xb3\x95\xe6\xa0\x91\xe5\xae\x9a\xe4\xb9\x89")));

    FirstSetButton.setButtonText(juce::CharPointer_UTF8(juce::CharPointer_UTF8("First\xe9\x9b\x86\xe5\x90\x88")));
    FollowSetButton.setButtonText(juce::CharPointer_UTF8(juce::CharPointer_UTF8("Follow\xe9\x9b\x86\xe5\x90\x88")));
    LR1Button.setButtonText(juce::CharPointer_UTF8(juce::CharPointer_UTF8("LR1\xe8\xa1\xa8")));
    LALR1Button.setButtonText(juce::CharPointer_UTF8(juce::CharPointer_UTF8("LALR1\xe8\xa1\xa8")));
    ActionButton.setButtonText(juce::CharPointer_UTF8(juce::CharPointer_UTF8("Action\xe8\xa1\xa8")));
    GotoButton.setButtonText(juce::CharPointer_UTF8(juce::CharPointer_UTF8("Goto\xe8\xa1\xa8")));
    ProcessButton.setButtonText(juce::CharPointer_UTF8(juce::CharPointer_UTF8("\xe5\x88\x86\xe6\x9e\x90\xe8\xbf\x87\xe7\xa8\x8b\xe5\xb1\x95\xe7\xa4\xba")));
    ShowTreeButton.setButtonText(juce::CharPointer_UTF8(juce::CharPointer_UTF8("\xe8\xaf\xad\xe6\xb3\x95\xe6\xa0\x91\xe5\xb1\x95\xe7\xa4\xba")));

    BNFoutput.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    BNFoutput.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    TreeEditor.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    TreeEditor.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    Treeoutput.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    Treeoutput.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    BNFButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    BNFButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    TreeDefineButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    TreeDefineButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    AnalysisButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    AnalysisButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    LALRButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    LALRButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);

    FirstSetButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    FirstSetButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    FollowSetButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    FollowSetButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    LR1Button.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    LR1Button.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    LALR1Button.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    LALR1Button.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    ActionButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    ActionButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    GotoButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    GotoButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    ProcessButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    ProcessButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    ShowTreeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    ShowTreeButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);

    BNFButton.addListener(this);
    BNFButton.setEnabled(true);
    AnalysisButton.addListener(this);
    AnalysisButton.setEnabled(true);
    LALRButton.addListener(this);
    LALRButton.setEnabled(true);
    TreeDefineButton.addListener(this);
    TreeDefineButton.setEnabled(true);
    FirstSetButton.addListener(this);
    FirstSetButton.setEnabled(true);
    FollowSetButton.addListener(this);
    FollowSetButton.setEnabled(true);
    LR1Button.addListener(this);
    LR1Button.setEnabled(true);
    LALR1Button.addListener(this);
    LALR1Button.setEnabled(true);
    ActionButton.addListener(this);
    ActionButton.setEnabled(true);
    GotoButton.addListener(this);
    GotoButton.setEnabled(true);
    ProcessButton.addListener(this);
    ProcessButton.setEnabled(true);
    ShowTreeButton.addListener(this);
    ShowTreeButton.setEnabled(true);

    addAndMakeVisible(table);
    table.setModel(this);
}

ParserComponent::~ParserComponent()
{
}

juce::File ParserComponent::getAppDataDirectory()
{
    // 获取用户的文档目录
    juce::File userDocuments = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);

    // 在文档目录下创建一个我们程序专用的文件夹
    juce::File appDataDir = userDocuments.getChildFile("CompilationFinalData");

    // 如果这个文件夹不存在，就创建它
    if (!appDataDir.isDirectory())
    {
        appDataDir.createDirectory();
    }

    return appDataDir;
}

void ParserComponent::parseFirstSetFile(const juce::File& file)
{
    firstSetTableData.clear();

    juce::String fileContent = file.loadFileAsString();
    if (fileContent.isEmpty())
    {
        DBG("File is EMPTY or could not be read: " + file.getFullPathName());
        return;
    }
    DBG("File content loaded successfully. Total length: " + juce::String(fileContent.length()));

    juce::StringArray lines;
    file.readLines(lines);
    DBG("File split into " + juce::String(lines.size()) + " lines.");

    int successful_parses = 0;
    for (const auto& line : lines)
    {
        if (line.isEmpty()) continue;

        int tabPos = line.indexOfChar('\t');

        if (tabPos == -1){
            DBG("FAILED to find tab in line: \"" + line + "\". Skipping line.");
            continue; 
        }

        std::string key = line.substring(0, tabPos).toStdString();
        std::string values = line.substring(tabPos + 1).toStdString();
        firstSetTableData.push_back({ key, values });
        successful_parses++;
    }
    DBG("parseFirstSetFile finished. Total successful parses: " + juce::String(successful_parses));
    DBG("Final size of firstSetTableData: " + juce::String(firstSetTableData.size()));
}

void ParserComponent::parseFollowSetFile(const juce::File& file)
{
    followSetTableData.clear();
    juce::StringArray lines;
    file.readLines(lines);
    for (const auto& line : lines)
    {
        if (line.isEmpty()) continue;
        int tabPos = line.indexOfChar('\t');
        if (tabPos != -1)
        {
            std::string key = line.substring(0, tabPos).toStdString();
            std::string values = line.substring(tabPos + 1).toStdString();
            followSetTableData.push_back({ key, values });
        }
    }
}

void ParserComponent::parseItemsFile(const juce::File& edgesFile, const juce::File& itemsFile, std::vector<std::vector<std::string>>& targetData)
{
    targetData.clear(); // 每次解析前清空目标数据容器

    // --- 1. 解析 Items 文件，建立 State -> ItemsContent 的映射 ---
    std::map<std::string, std::string> stateToItemsContent;
    if (itemsFile.existsAsFile())
    {
        juce::StringArray itemLines;
        itemsFile.readLines(itemLines);

        juce::String currentItemContent;
        juce::String currentItemNumber;

        for (const auto& line : itemLines)
        {
            if (line.startsWith("--Item["))
            {
                // 如果我们已经收集了一个 item 的内容，就把它存起来
                if (currentItemNumber.isNotEmpty() && currentItemContent.isNotEmpty())
                {
                    stateToItemsContent[currentItemNumber.toStdString()] = currentItemContent.trim().toStdString();
                }

                // 开始一个新的 item，并提取编号
                currentItemNumber = line.fromFirstOccurrenceOf("[", false, false)
                    .upToFirstOccurrenceOf("]", false, false);
                currentItemContent = ""; // 重置内容
            }
            else if (currentItemNumber.isNotEmpty())
            {
                // 将内容行追加到当前 item，并加上换行符
                currentItemContent += line + "\n";
            }
        }
        // 循环结束后，不要忘了保存最后一个收集到的 item
        if (currentItemNumber.isNotEmpty() && currentItemContent.isNotEmpty())
        {
            stateToItemsContent[currentItemNumber.toStdString()] = currentItemContent.trim().toStdString();
        }
    }

    // --- 2. 解析 Edges 文件，收集所有状态、符号和边信息 ---
    juce::StringArray edgeLines;
    edgesFile.readLines(edgeLines);
    if (edgeLines.isEmpty())
    {
        DBG("Edges file is empty or could not be read: " + edgesFile.getFullPathName());
        return; // 如果没有边信息，无法构建表格
    }

    std::set<std::string> states;
    std::set<std::string> symbols;
    std::vector<std::tuple<std::string, std::string, std::string>> edges;

    for (const auto& line : edgeLines)
    {
        auto tokens = juce::StringArray::fromTokens(line, "\t", "");
        tokens.removeEmptyStrings(true); // 去除可能的空token

        if (tokens.size() == 3)
        {
            const std::string fromState = tokens[0].toStdString();
            const std::string symbol = tokens[1].toStdString();
            const std::string toState = tokens[2].toStdString();

            states.insert(fromState);
            states.insert(toState);
            symbols.insert(symbol);
            edges.emplace_back(fromState, symbol, toState);
        }
    }

    if (states.empty() || symbols.empty())
    {
        DBG("No valid states or symbols found in edges file.");
        return; // 如果没有状态或符号，也无法构建表格
    }

    // --- 3. 构建表格的行列索引 ---
    std::vector<std::string> sortedStates(states.begin(), states.end());
    // 对状态进行数字排序，而不是字母排序
    std::sort(sortedStates.begin(), sortedStates.end(), [](const std::string& a, const std::string& b) {
        try { return std::stoi(a) < std::stoi(b); }
        catch (...) { return a < b; } // 如果转换失败，则按字符串比较
        });
    std::vector<std::string> sortedSymbols(symbols.begin(), symbols.end());

    // 创建映射：符号 -> 列索引，状态 -> 行索引
    std::map<std::string, int> symbol_to_col;
    for (int i = 0; i < sortedSymbols.size(); ++i)
        symbol_to_col[sortedSymbols[i]] = i + 2; // 列索引从2开始 (0: State, 1: Items)

    std::map<std::string, int> state_to_row;
    for (int i = 0; i < sortedStates.size(); ++i)
        state_to_row[sortedStates[i]] = i + 1; // 行索引从1开始 (0: 表头)

    // --- 4. 初始化并填充表格数据 ---
    // 表格总列数 = State列 + Items列 + 所有符号列
    targetData.assign(sortedStates.size() + 1, std::vector<std::string>(sortedSymbols.size() + 2, ""));

    // 填充表头
    targetData[0][0] = "State";
    targetData[0][1] = "Items";
    for (int i = 0; i < sortedSymbols.size(); ++i)
        targetData[0][i + 2] = sortedSymbols[i];

    // 填充第一列(State)和第二列(Items)
    for (int i = 0; i < sortedStates.size(); ++i)
    {
        const std::string& stateNum = sortedStates[i];
        int dataRow = i + 1;

        targetData[dataRow][0] = stateNum; // 第一列放状态编号

        if (stateToItemsContent.count(stateNum))
        {
            targetData[dataRow][1] = stateToItemsContent.at(stateNum); // 第二列放项目集内容
        }
    }

    // 填充状态转移信息
    for (const auto& edge : edges)
    {
        const std::string& fromState = std::get<0>(edge);
        const std::string& symbol = std::get<1>(edge);
        const std::string& toState = std::get<2>(edge);

        // 确保 fromState 和 symbol 存在于我们的映射中，避免崩溃
        if (state_to_row.count(fromState) && symbol_to_col.count(symbol))
        {
            int row = state_to_row.at(fromState);
            int col = symbol_to_col.at(symbol);
            targetData[row][col] = toState;
        }
    }
}

void ParserComponent::parseAnalysisTableFile(const juce::File& file, std::vector<std::vector<std::string>>& targetData)
{
    targetData.clear();
    if (!file.existsAsFile()) return;

    // --- 1. 读取所有条目，并收集所有状态和符号 ---
    std::map<std::string, std::map<std::string, std::string>> entries;
    std::set<std::string> states, symbols;

    juce::StringArray lines;
    file.readLines(lines);
    for (const auto& line : lines)
    {
        auto tokens = juce::StringArray::fromTokens(line, "\t", "");
        if (tokens.size() == 3)
        {
            const std::string state = tokens[0].toStdString();
            const std::string symbol = tokens[1].toStdString();
            const std::string value = tokens[2].toStdString();

            entries[state][symbol] = value;
            states.insert(state);
            symbols.insert(symbol);
        }
    }

    if (states.empty() || symbols.empty()) return;

    // --- 2. 准备表头和行列索引 ---
    std::vector<std::string> sortedStates(states.begin(), states.end());
    std::sort(sortedStates.begin(), sortedStates.end(), [](const auto& a, const auto& b) { try { return std::stoi(a) < std::stoi(b); } catch (...) { return a < b; } });

    std::vector<std::string> sortedSymbols(symbols.begin(), symbols.end());
    std::sort(sortedSymbols.begin(), sortedSymbols.end());

    // 创建符号到列的映射
    std::map<std::string, int> symbolToCol;
    for (int i = 0; i < sortedSymbols.size(); ++i) {
        symbolToCol[sortedSymbols[i]] = i + 1; // 第0列是State
    }

    // --- 3. 创建并填充二维表格 ---
    // 先添加表头行
    std::vector<std::string> headerRow;
    headerRow.push_back("State");
    headerRow.insert(headerRow.end(), sortedSymbols.begin(), sortedSymbols.end());
    targetData.push_back(headerRow);

    // 再逐行填充数据
    for (const auto& state : sortedStates)
    {
        // 创建一个新行，第一列是状态号
        std::vector<std::string> dataRow(headerRow.size(), "");
        dataRow[0] = state;

        // 查找这个状态的所有条目并填充
        if (entries.count(state))
        {
            for (const auto& pair : entries.at(state))
            {
                const std::string& symbol = pair.first;
                const std::string& value = pair.second;
                if (symbolToCol.count(symbol))
                {
                    dataRow[symbolToCol.at(symbol)] = value;
                }
            }
        }
        targetData.push_back(dataRow);
    }
}

void ParserComponent::parseProcessFile(const juce::File& file)
{
    processTableData.clear();
    if (!file.existsAsFile()) return;

    juce::StringArray lines;
    file.readLines(lines);

    // 临时存储一个步骤的所有信息
    std::string step, stack, input, action;

    for (const auto& line : lines)
    {
        if (line.trim().isEmpty()) continue;

        juce::String trimmedLine = line.trim();

        if (trimmedLine.startsWith(juce::CharPointer_UTF8("[\xe6\xad\xa5\xe9\xaa\xa4"))) // "[步骤"
        {
            if (!step.empty())
            {
                processTableData.push_back({ step, stack, input, action });
                step = ""; stack = ""; input = ""; action = "";
            }
            step = trimmedLine.fromFirstOccurrenceOf(juce::CharPointer_UTF8("\xe9\xaa\xa4"), false, false)
                .upToFirstOccurrenceOf("]", false, false).toStdString();
        }
        else if (trimmedLine.startsWith(juce::CharPointer_UTF8("\xe5\x88\x86\xe6\x9e\x90\xe6\xa0\x88"))) // "分析栈"
        {
            stack = trimmedLine.substring(juce::String(juce::CharPointer_UTF8("\xe5\x88\x86\xe6\x9e\x90\xe6\xa0\x88")).length()).trim().toStdString();
        }
        else if (trimmedLine.startsWith(juce::CharPointer_UTF8("\xe8\xbe\x93\xe5\x85\xa5"))) // "输入"
        {
            // --- 核心修改在这里 ---
            juce::String inputContent = trimmedLine.substring(juce::String(juce::CharPointer_UTF8("\xe8\xbe\x93\xe5\x85\xa5")).length()).trim();

            // 1. 去掉两边的尖括号 <>
            if (inputContent.startsWith("<") && inputContent.endsWith(">"))
            {
                inputContent = inputContent.substring(1, inputContent.length() - 1);
            }

            // 2. 将中间的制表符 \t 替换为 " -> "
            // 注意：你的文件里可能是制表符，也可能是空格，我们两种都处理
            if (inputContent.containsChar('\t'))
            {
                inputContent = inputContent.replace("\t", " -> ");
            }
            else // 如果没有制表符，就尝试按空格分割
            {
                // 找到第一个空格
                int spacePos = inputContent.indexOfChar(' ');
                if (spacePos != -1)
                {
                    // 获取空格前后的部分，然后用 " -> " 拼接
                    juce::String part1 = inputContent.substring(0, spacePos).trim();
                    juce::String part2 = inputContent.substring(spacePos).trim();
                    if (part2.isNotEmpty())
                    {
                        inputContent = part1 + " -> " + part2;
                    }
                }
            }

            input = inputContent.toStdString();
            // --- 修改结束 ---
        }
        else // 动作
        {
            if (action.empty())
                action = trimmedLine.toStdString();
            else
                action += "\n" + trimmedLine.toStdString();
        }
    }

    if (!step.empty())
    {
        processTableData.push_back({ step, stack, input, action });
    }
}

void ParserComponent::parseTreeFile(const juce::File& file)
{
    syntaxTreeTableData.clear();
    if (!file.existsAsFile()) return;

    juce::StringArray lines;
    file.readLines(lines);

    int maxDepth = 0;
    std::vector<std::pair<int, std::string>> parsedNodes;

    // --- 1. 第一次遍历：精确计算深度和内容 ---
    for (const auto& line : lines)
    {
        if (line.trim().isEmpty()) continue;

        // --- 核心修改在这里 ---
        // 计算前导空格的数量，这就是深度
        int depth = 0;
        for (char c : line) {
            if (c == ' ') {
                depth++;
            }
            else {
                // 遇到第一个非空格字符，停止计算
                break;
            }
        }
        // -----------------------

        if (depth > maxDepth) {
            maxDepth = depth;
        }

        parsedNodes.push_back({ depth, line.trim().toStdString() });
    }

    if (parsedNodes.empty()) return;

    // --- 2. 第二次遍历：构建二维表格 (这部分逻辑不变) ---
    int numColumns = maxDepth + 1;

    for (const auto& nodeInfo : parsedNodes)
    {
        int depth = nodeInfo.first;
        const std::string& text = nodeInfo.second;

        std::vector<std::string> newRow(numColumns, "");

        if (depth < numColumns)
        {
            newRow[depth] = text;
        }

        syntaxTreeTableData.push_back(newRow);
    }
}

void ParserComponent::updateTable(){
    table.setAutoSizeMenuOptionShown(true);
    table.getHeader().removeAllColumns();
    switch (currentView)
    {
    case TableView::FirstSet:
        table.getHeader().addColumn("Non-terminal", 1, 150);
        table.getHeader().addColumn("First Set", 2, 400);
        break;
    case TableView::FollowSet:
        table.getHeader().addColumn("Non-terminal", 1, 150);
        table.getHeader().addColumn("Follow Set", 2, 400);
        break;
    case TableView::LR1Items:
    case TableView::LALR1Items:
    {
        auto& data = (currentView == TableView::LR1Items) ? lr1ItemsTableData : lalr1ItemsTableData;
        if (!data.empty()) {
            for (int i = 0; i < data[0].size(); ++i) {
                // 为 State 和 Items 列设置不同的宽度
                int columnWidth = 50; // 默认宽度
                if (i == 0) columnWidth = 70;  // State 列
                if (i == 1) columnWidth = 400; // Items 列
                table.getHeader().addColumn(data[0][i], i + 1, columnWidth);
            }
        }
    }
        break;
    case TableView::AnalysisAction:
        if (!analysisTableActionData.empty())
        {
            const auto& headers = analysisTableActionData[0]; // 表头在数据的第0行
            for (int i = 0; i < headers.size(); ++i)
            {
                table.getHeader().addColumn(headers[i], i + 1, (i == 0) ? 80 : 60);
            }
        }
        break;
    case TableView::AnalysisGoto:
        if (!analysisTableGotoData.empty())
        {
            const auto& headers = analysisTableGotoData[0]; // 表头在数据的第0行
            for (int i = 0; i < headers.size(); ++i)
            {
                table.getHeader().addColumn(headers[i], i + 1, (i == 0) ? 80 : 60);
            }
        }
        break;
    case TableView::ParsingProcess:
        table.setRowHeight(40); 
        table.getHeader().addColumn(juce::CharPointer_UTF8("\xe6\xad\xa5\xe9\xaa\xa4"), 1, 60);
        table.getHeader().addColumn(juce::CharPointer_UTF8("\xe5\x88\x86\xe6\x9e\x90\xe6\xa0\x88"), 2, 400);
        table.getHeader().addColumn(juce::CharPointer_UTF8("\xe8\xbe\x93\xe5\x85\xa5"), 3, 200);
        table.getHeader().addColumn(juce::CharPointer_UTF8("\xe5\x8a\xa8\xe4\xbd\x9c"), 4, 300);
        break;
    case TableView::SyntaxTree:
        table.setRowHeight(25);
        if (!syntaxTreeTableData.empty()){
            int numColumns = syntaxTreeTableData[0].size();
            for (int i = 0; i < numColumns; ++i)
                table.getHeader().addColumn(juce::String(i), i + 1, (getWidth() / 2 + 120) / numColumns);
        }
        break;
    default: break;
    }
    table.updateContent();
}

void ParserComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::whitesmoke);
    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);
}

void ParserComponent::resized()
{
    int offset = getWidth() / 6;
    int dex = (offset - 120) / 2;

    BNFButton.setBounds(dex, 40, 120, 40);
    LALRButton.setBounds(offset * 1 + dex, 40, 120, 40);
    TreeDefineButton.setBounds(offset * 2 + dex, 40, 120, 40);
    AnalysisButton.setBounds(offset * 3 + dex, 40, 120, 40);

    FirstSetButton.setBounds(dex, 100, 120, 40);
    FollowSetButton.setBounds(offset * 1 + dex, 100, 120, 40);
    LR1Button.setBounds(offset * 2 + dex, 100, 120, 40);
    LALR1Button.setBounds(offset * 3 + dex, 100, 120, 40);
    ActionButton.setBounds(offset * 4 + dex, 100, 120, 40);
    GotoButton.setBounds(offset * 5 + dex, 100, 120, 40);
    ProcessButton.setBounds(offset * 4 + dex, 40, 120, 40);
    ShowTreeButton.setBounds(offset * 5 + dex, 40, 120, 40);

    BNFoutput.setBounds(dex, 160, 400, 360);
    TreeEditor.setBounds(dex, 540, 400, 360);
    //Treeoutput.setBounds(dex, 660, 400, 240);

    table.setBounds(dex + offset * 2 , 160, offset * 3 + 120, 740);

}

void ParserComponent::buttonClicked(juce::Button* button)
{
    juce::File appDataDir = getAppDataDirectory();

    if (button == &BNFButton) {
        juce::FileChooser fileChooser("Select a text file", juce::File::getSpecialLocation(juce::File::userDesktopDirectory), "*.txt");
        if (fileChooser.browseForFileToOpen()) {
            resetAnalysis();
            juce::File selectedFile = fileChooser.getResult();
            // 打开文件并读取内容
            juce::String fileContents = selectedFile.loadFileAsString();
            BNFoutput.setText(fileContents);

            juce::StringArray lines;
            lines.addLines(fileContents);
            for (auto line : lines) {
                if (line == "") continue;
                gramline.push_back(line.toStdString());
            }
            scanGrammar();  // 扫描全部文法
            splitGrammarRules();
            computeAllFirstSets();  // 求全部First集合
            computeAllFollowSets();  // 求全部Follow集合
            buildLR1Item();  // 构建LR(1)项目集
            buildLALR1Item();  // 构建LALR(1)项目集
            buildLALR1Table();  // 构建LALR(1)分析表
            PrintBNF();
        }
    }

    else if (button == &LALRButton)
    {
        ExportLALR();
    }
    
    else if (button == &TreeDefineButton) {
        juce::FileChooser fileChooser("Select a your Tree Defination file", juce::File::getSpecialLocation(juce::File::userDesktopDirectory), "*.txt");
        if (fileChooser.browseForFileToOpen()) {
            juce::File selectedFile = fileChooser.getResult();
            juce::String fileContents = selectedFile.loadFileAsString();
            TreeEditor.setText(fileContents);
            std::string FilesavePath = appDataDir.getFullPathName().toStdString();
            ofstream ofs(FilesavePath + "/TreeDefine.txt");
            ofs << fileContents.toStdString();
            ofs.close();
        }
    }

    else if (button == &AnalysisButton) {
        resetParserAndTree();
        std::string FileloadPath = appDataDir.getFullPathName().toStdString();
        ofstream Tofs(FileloadPath + "/Treeoutput.txt");
        loadGrams(FileloadPath+"/Grams.txt");
        loadTable(FileloadPath+"/Table.txt");
        loadMap(FileloadPath+"/Map.txt");
        loadLex(FileloadPath+"/sample.lex");
        loadTree(FileloadPath + "/TreeDefine.txt");
        ofstream Pofs(FileloadPath + "/Process.txt");
        parse(Pofs);
        printTree(Tofs);
        Tofs.close();
    }

    else if (button == &FirstSetButton) {
        juce::File dataFile = appDataDir.getChildFile("FirstSet.txt");
        if (dataFile.existsAsFile()){
            parseFirstSetFile(dataFile);
            currentView = TableView::FirstSet;
            updateTable();
        }
        else
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "fail", "can't find FirstSet.txt");
    }

    else if (button == &FollowSetButton)
    {
        juce::File dataFile = appDataDir.getChildFile("FollowSet.txt");
        if (dataFile.existsAsFile()) {
            parseFollowSetFile(dataFile);
            currentView = TableView::FollowSet;
            updateTable();
        }
        else
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "fail", "can't find FollowSet.txt");
    }

    else if (button == &LR1Button) 
    {
        juce::File itemsFile = appDataDir.getChildFile("LR1.txt");
        juce::File edgesFile = appDataDir.getChildFile("LR1Edges.txt");
        parseItemsFile(edgesFile,itemsFile, lr1ItemsTableData);
        currentView = TableView::LR1Items;
        updateTable();
    }

    else if (button == &LALR1Button)
    {
        juce::File itemsFile = appDataDir.getChildFile("LALR1.txt");
        juce::File edgesFile = appDataDir.getChildFile("LALR1Edges.txt");
        parseItemsFile(edgesFile, itemsFile ,lalr1ItemsTableData);
        currentView = TableView::LALR1Items;
        updateTable();
    }

    else if (button == &ActionButton) 
    {
        juce::File ActionFile = appDataDir.getChildFile("Action.txt");
        parseAnalysisTableFile(ActionFile, analysisTableActionData);
        currentView = TableView::AnalysisAction;
        updateTable();
    }

    else if (button == &GotoButton)
    {
        juce::File ActionFile = appDataDir.getChildFile("Goto.txt");
        parseAnalysisTableFile(ActionFile, analysisTableGotoData);
        currentView = TableView::AnalysisGoto;
        updateTable();
    }

    else if (button == &ProcessButton) {
        juce::File dataFile = appDataDir.getChildFile("Process.txt");
        if (!dataFile.existsAsFile()){
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Fail", "Can't find TXT");
            return;
        }
        parseProcessFile(dataFile);
        currentView = TableView::ParsingProcess;
        updateTable();
    }

    else if (button == &ShowTreeButton)
    {
        juce::File dataFile = appDataDir.getChildFile("Treeoutput.txt");
        parseTreeFile(dataFile);
        currentView = TableView::SyntaxTree;
        updateTable();
        }
}

int ParserComponent::getNumRows()
{
    size_t numRows = 0;
    std::vector<std::vector<std::string>>* activeData = nullptr;

    switch (currentView)
    {
    case TableView::FirstSet:       activeData = &firstSetTableData; break;
    case TableView::FollowSet:      activeData = &followSetTableData; break;
    case TableView::LR1Items:        activeData = &lr1ItemsTableData; break;
    case TableView::LALR1Items:      activeData = &lalr1ItemsTableData; break;
    case TableView::AnalysisAction: activeData = &analysisTableActionData; break;
    case TableView::AnalysisGoto:   activeData = &analysisTableGotoData; break;
    case TableView::ParsingProcess: return (int)processTableData.size();
    case TableView::SyntaxTree: return (int)syntaxTreeTableData.size();
    default:                        return 0;
    }

    if (!activeData) return 0;

    // --- 核心修改在这里 ---
    // 判断这个视图的数据是否包含表头行
    bool hasHeaderInData = (currentView == TableView::LR1Items || currentView == TableView::LALR1Items);

    if (hasHeaderInData)
    {
        // 如果有表头，数据行数是总行数减一
        numRows = activeData->size() > 1 ? activeData->size() - 1 : 0;
    }
    else
    {
        // 如果没有表头，数据行数就是总行数
        numRows = activeData->size();
    }

    return (int)numRows;
}

void ParserComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.setColour(juce::Colours::lightgrey);
    g.drawRect(0, 0, width, height);

    std::string text_to_paint;
    std::vector<std::vector<std::string>>* activeData = nullptr;
    bool hasHeaderInData = false;

    switch (currentView)
    {
    case TableView::FirstSet:       activeData = &firstSetTableData; break;
    case TableView::FollowSet:      activeData = &followSetTableData; break;
    case TableView::LR1Items:
        activeData = &lr1ItemsTableData;
        hasHeaderInData = true;
        break;
    case TableView::LALR1Items:
        activeData = &lalr1ItemsTableData;
        hasHeaderInData = true;
        break;
    case TableView::AnalysisAction:
        activeData = &analysisTableActionData;
        hasHeaderInData = true;
        break;
    case TableView::AnalysisGoto:
        activeData = &analysisTableGotoData;
        hasHeaderInData = true;
        break;
    case TableView::ParsingProcess:
        activeData = &processTableData;
        hasHeaderInData = false; 
        break;
    case TableView::SyntaxTree:
        activeData = &syntaxTreeTableData;
        hasHeaderInData = false; 
        break;
    default: break;
    }

    if (activeData)
    {
        // 将表格行号映射到数据数组的行索引
        int dataRow = hasHeaderInData ? rowNumber + 1 : rowNumber;
        int dataCol = columnId - 1;

        if (dataRow < activeData->size() && dataCol < (*activeData)[dataRow].size())
        {
            text_to_paint = (*activeData)[dataRow][dataCol];
        }
    }
    
    g.drawText(text_to_paint, 2, 0, width - 4, height, juce::Justification::centred);
    
}

void ParserComponent::cellClicked(int rowNumber, int columnId, const juce::MouseEvent& event)
{
    // 我们通常只关心鼠标左键的单击事件
    if (!event.mods.isLeftButtonDown())
        return;
    
    // --- 获取被点击单元格的完整文本 ---
    // 这段逻辑和你的 paintCell 几乎完全一样

    std::string fullText;
    const auto& getData = [&](const auto& tableData, bool hasHeader) {
        int dataRow = hasHeader ? rowNumber + 1 : rowNumber;
        int dataCol = columnId - 1;
        if (dataRow >= 0 && dataRow < tableData.size() &&
            dataCol >= 0 && dataCol < tableData[dataRow].size())
        {
            fullText = tableData[dataRow][dataCol];
        }
        };

    // 根据当前视图，从对应的数据源获取文本
    switch (currentView)
    {
    case TableView::FirstSet:       getData(firstSetTableData, false); break;
    case TableView::FollowSet:      getData(followSetTableData, false); break;
    case TableView::LR1Items:        getData(lr1ItemsTableData, true); break;
    case TableView::LALR1Items:      getData(lalr1ItemsTableData, true); break;
    case TableView::AnalysisAction: getData(analysisTableActionData, false); break;
    case TableView::AnalysisGoto:   getData(analysisTableGotoData, false); break;
    case TableView::ParsingProcess: getData(processTableData, false); break;
    default:
        return; 
    }

    // --- 如果成功获取到非空文本，就用 AlertWindow 显示它 ---
    if (!fullText.empty())
    {
        // 为了让多行文本在 AlertWindow 中正确显示，我们需要确保换行符是 '\n'
        // std::string 通常没问题，juce::String 的换行符可能是 \r\n，需要转换
        juce::String textToShow(fullText);
        textToShow = textToShow.replace("\r\n", "\n");

        juce::AlertWindow::showMessageBox(
            juce::AlertWindow::InfoIcon,        // 对话框左侧的图标
            juce::CharPointer_UTF8("Items\xe5\x86\x85\xe5\xae\xb9\xe8\xaf\xa6\xe6\x83\x85"),                     // 对话框的标题
            textToShow,                         // 要显示的主要文本内容
            juce::CharPointer_UTF8("\xe5\x85\xb3\xe9\x97\xad")                              // 确认按钮上显示的文字
        );
    }
}

