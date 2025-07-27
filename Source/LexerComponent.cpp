#include <JuceHeader.h>
#include "LexerComponent.h"

void runCommand(const string& command) {
    system(command.c_str());
}

//==============================================================================
LexerComponent::LexerComponent()
{
    setSize(1400, 950);

    addAndMakeVisible(loadFileButton);
    addAndMakeVisible(NFAButton);
    addAndMakeVisible(DFAButton);
    addAndMakeVisible(MDFAButton);
    addAndMakeVisible(outputEditor);
    addAndMakeVisible(saveButton);
    addAndMakeVisible(loadsampleCodeButton);
    addAndMakeVisible(sampleTINY);
    addAndMakeVisible(codeButton);
    addAndMakeVisible(codeoutput);
    addAndMakeVisible(LoadlexButton);

    outputEditor.setMultiLine(true);
    outputEditor.setReturnKeyStartsNewLine(true);
    outputEditor.setReadOnly(false);

    sampleTINY.setMultiLine(true);
    sampleTINY.setReturnKeyStartsNewLine(true);
    sampleTINY.setReadOnly(false);

    codeoutput.setMultiLine(true);
    codeoutput.setReturnKeyStartsNewLine(true);
    codeoutput.setReadOnly(false);

    loadFileButton.setButtonText(juce::CharPointer_UTF8("\xe5\x8a\xa0\xe8\xbd\xbd\xe6\xad\xa3\xe5\x88\x99\xe8\xa1\xa8\xe8\xbe\xbe\xe5\xbc\x8f"));
    NFAButton.setButtonText(juce::CharPointer_UTF8("\xe5\xb1\x95\xe7\xa4\xbaNFA"));
    DFAButton.setButtonText(juce::CharPointer_UTF8("\xe5\xb1\x95\xe7\xa4\xba""DFA"));
    MDFAButton.setButtonText(juce::CharPointer_UTF8("\xe5\xb1\x95\xe7\xa4\xba\xe6\x9c\x80\xe5\xb0\x8f""DFA"));
    saveButton.setButtonText(juce::CharPointer_UTF8("\xe4\xbf\x9d\xe5\xad\x98\xe6\xad\xa3\xe5\x88\x99\xe8\xa1\xa8\xe8\xbe\xbe\xe5\xbc\x8f"));
    loadsampleCodeButton.setButtonText(juce::CharPointer_UTF8("\xe5\x8a\xa0\xe8\xbd\xbd\xe6\xba\x90\xe7\xa8\x8b\xe5\xba\x8f"));
    codeButton.setButtonText(juce::CharPointer_UTF8("\xe8\xbf\x9b\xe8\xa1\x8c\xe7\xbc\x96\xe7\xa0\x81"));
    LoadlexButton.setButtonText(juce::CharPointer_UTF8("\xe5\xb1\x95\xe7\xa4\xba\xe7\xbc\x96\xe7\xa0\x81\xe6\x96\x87\xe4\xbb\xb6"));

    loadFileButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    loadFileButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    NFAButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    NFAButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    DFAButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    DFAButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    MDFAButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    MDFAButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    saveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    saveButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    loadsampleCodeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    loadsampleCodeButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    outputEditor.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    outputEditor.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    sampleTINY.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    sampleTINY.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    codeoutput.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    codeoutput.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    codeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    codeButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    LoadlexButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    LoadlexButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    

    loadFileButton.addListener(this);
    loadFileButton.setEnabled(true);
    NFAButton.addListener(this);
    NFAButton.setEnabled(true);
    DFAButton.addListener(this);
    DFAButton.setEnabled(true);
    MDFAButton.addListener(this);
    MDFAButton.setEnabled(true);
    saveButton.addListener(this);
    saveButton.setEnabled(true);
    loadsampleCodeButton.addListener(this);
    loadsampleCodeButton.setEnabled(true);
    codeButton.addListener(this);
    codeButton.setEnabled(true);
    LoadlexButton.addListener(this);
    LoadlexButton.setEnabled(true);

    addAndMakeVisible(table);
}

LexerComponent::~LexerComponent()
{
}

void LexerComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::whitesmoke);
    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);
}

void LexerComponent::resized()
{
    int offset = getWidth() / 5;
    int dex = (offset - 120) / 2;

    saveButton.setBounds(dex + offset * 4, 40, 120, 40);
    MDFAButton.setBounds(offset * 3 + dex, 40, 120, 40);
    DFAButton.setBounds(offset * 2 + dex, 40, 120, 40);
    NFAButton.setBounds(offset * 1 + dex, 40, 120, 40);
    loadFileButton.setBounds(dex, 40, 120, 40);
    loadsampleCodeButton.setBounds(dex, 100, 120, 40);
    codeButton.setBounds(offset * 1 + dex, 100, 120, 40);
    LoadlexButton.setBounds(offset * 2 + dex, 100, 120, 40);

    outputEditor.setBounds(dex, 160, 400, 240);
    sampleTINY.setBounds(dex, 410, 400, 240);
    codeoutput.setBounds(dex, 660, 400, 240);

    table.setBounds(dex + offset * 2, 160, offset*2 +120 , 740);

}

juce::File LexerComponent::getAppDataDirectory()
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

void LexerComponent::parseLexFile(const juce::File& file) {
    lexFileTableData.clear();
    if (!file.existsAsFile()) return;

    juce::StringArray lines;
    file.readLines(lines);

    for (const auto& line : lines)
    {
        if (line.trim().isEmpty()) continue;

        auto tokens = juce::StringArray::fromTokens(line, "\t", "");
        tokens.removeEmptyStrings(true); // 去除制表符分割后可能产生的空字符串

        if (tokens.size() == 1)
        {
            // 格式1: 只有编码
            lexFileTableData.push_back({ tokens[0].toStdString(), " " });
        }
        else if (tokens.size() >= 2)
        {
            // 格式2: 编码 + 值
            lexFileTableData.push_back({ tokens[0].toStdString(), tokens[1].toStdString() });
        }
    }
}

void LexerComponent::buttonClicked(juce::Button* button)
{
    if (button == &loadFileButton)
    {

        juce::FileChooser fileChooser("Select a text file", juce::File::getSpecialLocation(juce::File::userDesktopDirectory), "*.txt");
        if (fileChooser.browseForFileToOpen())
        {
            juce::File appDataDir = getAppDataDirectory();
            resetLexerAnalysis();
            juce::File selectedFile = fileChooser.getResult();
            // 打开文件并读取内容
            juce::String fileContents = selectedFile.loadFileAsString();
            // 将文件内容加载到文本框
            outputEditor.setText(fileContents);
            //处理正则表达式
            InRegex = fileContents.toStdString();
            Run();
            Export();
            juce::File lexerFile = appDataDir.getChildFile("Lexer.c");
            juce::String LexerContents = lexerFile.loadFileAsString();
            codeoutput.setText(LexerContents);
        }
    }

    else if (button == &saveButton)
    {
        juce::FileChooser directoryChooser("Choose a directory to save the file, the file will be save as your_regex.txt ", juce::File(), "");

        if (directoryChooser.browseForDirectory())
        {
            juce::File selectedDirectory = directoryChooser.getResult();

            juce::String fileName = "your_regex.txt";

            juce::File fileToSave = selectedDirectory.getChildFile(fileName);

            juce::String textToSave = outputEditor.getText();
            fileToSave.replaceWithText(textToSave);

            juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::InfoIcon,
                "File Saved",
                juce::CharPointer_UTF8("\xe6\x96\x87\xe4\xbb\xb6\xe4\xbf\x9d\xe5\xad\x98\xe6\x88\x90\xe5\x8a\x9f\xef\xbc\x8c\xe4\xbd\xa0\xe7\x9a\x84\xe6\x96\x87\xe4\xbb\xb6\xe8\xa2\xab\xe4\xbf\x9d\xe5\xad\x98\xe4\xb8\xbayour_regex.txt"),
                juce::CharPointer_UTF8("\xe7\xa1\xae\xe5\xae\x9a"));
        }
    }

    else if (button == &loadsampleCodeButton)
    {
        juce::FileChooser fileChooser(juce::CharPointer_UTF8("\xe9\x80\x89\xe6\x8b\xa9TINY\xe6\xba\x90\xe4\xbb\xa3\xe7\xa0\x81"),
            juce::File::getSpecialLocation(juce::File::userDesktopDirectory), "*.txt", ".tny");
        if (fileChooser.browseForFileToOpen()) {
            juce::File selectedFile = fileChooser.getResult();

            filePath = selectedFile.getFullPathName();

            juce::String fileContents = selectedFile.loadFileAsString();

            sampleTINY.setText(fileContents);
        }
    }

    else if (button == &codeButton)
    {
        string code = sampleTINY.getText().toStdString();

        juce::File appDataDir = getAppDataDirectory();
        // 获取当前可执行文件的路径
        juce::File appFile = juce::File::getSpecialLocation(juce::File::currentApplicationFile);
        juce::File appDir = appFile.getParentDirectory();

        // 构建 TCC 编译命令
        // tcc -o [输出文件名] [输入文件名]

        juce::String tccCompilerPath = appDir.getChildFile("tcc").getFullPathName();
        juce::String outputFile = appDataDir.getChildFile("Lexer.exe").getFullPathName();
        juce::String inputFile = appDataDir.getChildFile("Lexer.c").getFullPathName();

        juce::String compileCommand = "\"" + tccCompilerPath + " -o " + outputFile + " " + inputFile + "\"";

        // 执行编译命令
        runCommand(compileCommand.toStdString());

        // 检查 result.exe 是否生成成功
        if (!juce::File(outputFile).existsAsFile())
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "fail", "check it");
            return;
        }

        //执行编译好的 result.exe
        juce::String lexFile = appDataDir.getChildFile("sample.lex").getFullPathName();
        juce::String executeCommand = "\"" + outputFile + " " + filePath.toStdString() + " " + lexFile + "\"";
        runCommand(executeCommand.toStdString());

    }

    else if (button == &NFAButton)
    {
        drawNFA = true;
        drawDFA = false;
        drawMDFA = false;
        drawLex = false;
        removeChildComponent(&table);
        table.setModel(this);
        int numCols = NFAGraph[0].size();
        int numRows = NFAGraph.size();
        table.setHeaderHeight(30);
        table.setRowHeight(30);
        table.setAutoSizeMenuOptionShown(true);
        table.getHeader().removeAllColumns();
        for (int i = 0; i < numCols; ++i) {
            string toprint = "";
            for (auto y : NFAGraph[0][i])
                toprint += y;
            table.getHeader().addColumn(toprint, i + 1, 30);
        }
        addAndMakeVisible(table);
    }

    else if (button == &DFAButton)
    {
        removeChildComponent(&table);
        drawDFA = true;
        drawMDFA = false;
        drawNFA = false;
        drawLex = false;
        table.setModel(this);
        int numCols = DFAGraph[0].size();
        int numRows = DFAGraph.size();
        table.setHeaderHeight(30);
        table.setRowHeight(30);
        table.setAutoSizeMenuOptionShown(true);
        table.getHeader().removeAllColumns();
        for (int i = 0; i < numCols; ++i) {
            string toprint = "";
            for (auto y : DFAGraph[0][i])
                toprint += y;
            table.getHeader().addColumn(toprint, i + 1, 30);
        }
        addAndMakeVisible(table);
    }

    else if (button == &MDFAButton)
    {
        removeChildComponent(&table);
        drawDFA = false;
        drawMDFA = true;
        drawNFA = false;
        drawLex = false;
        table.setModel(this);
        int numCols = MDFAGraph[0].size();
        int numRows = MDFAGraph.size();
        table.setHeaderHeight(30);
        table.setRowHeight(30);
        table.setAutoSizeMenuOptionShown(true);
        table.getHeader().removeAllColumns();
        for (int i = 0; i < numCols; ++i) {
            string toprint = "";
            for (auto y : MDFAGraph[0][i])
                toprint += y;
            table.getHeader().addColumn(toprint, i + 1, 30);
        }
        addAndMakeVisible(table);
    }

    //加载编码表
    else if (button == &LoadlexButton) {
        juce::File appDataDir = getAppDataDirectory();
        juce::File dataFile = appDataDir.getChildFile("sample.lex");
        parseLexFile(dataFile);
        removeChildComponent(&table);
        drawDFA = false;
        drawMDFA = false;
        drawNFA = false;
        drawLex = true;
        table.setModel(this);
        table.getHeader().removeAllColumns();
        table.getHeader().addColumn("Code", 1, 150);
        table.getHeader().addColumn("Value", 2, 250);
        table.updateContent();
        addAndMakeVisible(table);
    }
}

int LexerComponent::getNumRows()
{
    if (drawNFA)
        return NFAGraph.size() > 1 ? NFAGraph.size() - 1 : 0;
    else if (drawDFA)
        return DFAGraph.size() > 1 ? DFAGraph.size() - 1 : 0;
    else if (drawMDFA)
        return MDFAGraph.empty() ? 0 : MDFAGraph.size();
    else
        return (int)lexFileTableData.size();
}

void LexerComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.setColour(juce::Colours::lightgrey);
    g.drawRect(0, 0, width, height);
    if (drawNFA) {
        juce::String toprint = "";
        for (auto y : NFAGraph[rowNumber + 1][columnId - 1])
            toprint += y;
        g.drawText(toprint, 0, 0, width, height, juce::Justification::centred);
    }
    else if (drawDFA) {
        juce::String toprint = "";
        for (auto y : DFAGraph[rowNumber + 1][columnId - 1])
            toprint += y;
        g.drawText(toprint, 0, 0, width, height, juce::Justification::centred);
    }
    else if (drawMDFA) {
        juce::String toprint = "";
        for (auto y : MDFAGraph[rowNumber+1][columnId - 1])
            toprint += y;
        g.drawText(toprint, 0, 0, width, height, juce::Justification::centred);
    }
    else if (drawLex) {
        if (rowNumber < lexFileTableData.size() && columnId - 1 < lexFileTableData[rowNumber].size()){
            juce::String text = lexFileTableData[rowNumber][columnId - 1];
            g.drawText(text, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        }
    }
}
