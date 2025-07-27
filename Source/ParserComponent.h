#pragma once
#include <JuceHeader.h>
#include "BNFAnalysis.h"
#include "tree.h"
#include <filesystem>

//==============================================================================
/*
*/
class ParserComponent  : public juce::Component, public juce::Button::Listener, juce::TableListBoxModel
{
public:
    ParserComponent();
    ~ParserComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextButton BNFButton;
    juce::TextButton LALRButton;
    juce::TextButton AnalysisButton;
    juce::TextButton TreeDefineButton;

    juce::TextButton FirstSetButton;
    juce::TextButton FollowSetButton;
    juce::TextButton LR1Button;
    juce::TextButton LALR1Button;
    juce::TextButton ActionButton;
    juce::TextButton GotoButton;
    juce::TextButton ProcessButton;
    juce::TextButton ShowTreeButton;

    juce::TextEditor BNFoutput;
    juce::TextEditor TreeEditor;
    juce::TextEditor Treeoutput;

    juce::TableListBox table;

    enum class TableView
    {
        None,           // 默认状态，不显示任何表格
        FirstSet,       // 显示 First 集
        FollowSet,      // 显示 Follow 集
        LR1Items,       // 显示 LR(1) 项目集
        LALR1Items,     // 显示 LALR(1) 项目集
        AnalysisAction, // 显示分析表的 ACTION 部分
        AnalysisGoto,    // 显示分析表的 GOTO 部分
        ParsingProcess, // 展示分析步骤
        SyntaxTree
    };

    TableView currentView = TableView::None;

    std::vector<std::vector<std::string>> firstSetTableData;
    std::vector<std::vector<std::string>> followSetTableData;
    std::vector<std::vector<std::string>> lr1ItemsTableData;
    std::vector<std::vector<std::string>> lalr1ItemsTableData;
    std::vector<std::vector<std::string>> analysisTableActionData;
    std::vector<std::vector<std::string>> analysisTableGotoData;
    std::vector<std::vector<std::string>> processTableData;
    std::vector<std::vector<std::string>> syntaxTreeTableData;

    juce::File getAppDataDirectory();
    void updateTable();
    void parseFirstSetFile(const juce::File& file);
    void parseFollowSetFile(const juce::File& file);
    void parseItemsFile(const juce::File& edgesFile, const juce::File& itemsFile,std::vector<std::vector<std::string>>& targetData);
    void parseAnalysisTableFile(const juce::File& file, std::vector<std::vector<std::string>>& targetData);
    void parseProcessFile(const juce::File& file);
    void parseTreeFile(const juce::File& file);

    void buttonClicked(juce::Button* button) override;

    int getNumRows() override;

    void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override {
        if (rowIsSelected)
            g.fillAll(juce::Colours::lightblue);
    }

    void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    void cellClicked(int rowNumber, int columnId, const juce::MouseEvent& event) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParserComponent)
};
