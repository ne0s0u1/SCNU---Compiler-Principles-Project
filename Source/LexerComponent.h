#pragma once
#include <JuceHeader.h>
#include "RegexAnalysis.h"
#include <filesystem>
//==============================================================================
/*
*/
class LexerComponent  : public juce::Component, public juce::Button::Listener, juce::TableListBoxModel
{
public:
    LexerComponent();
    ~LexerComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextButton loadFileButton;
    juce::TextButton NFAButton;
    juce::TextButton DFAButton;
    juce::TextButton MDFAButton;
    juce::TextButton loadsampleCodeButton;
    juce::TextButton saveButton;
    juce::TextButton codeButton;//编译按钮
    juce::TextButton LoadlexButton;

    juce::TableListBox table;

    juce::TextEditor outputEditor;
    juce::TextEditor sampleTINY;//代码展示窗口
    juce::TextEditor codeoutput;//编码显示窗口

    bool drawNFA = false;
    bool drawDFA = false;
    bool drawMDFA = false;
    bool drawLex = false;

    juce::String filePath;

    std::vector<std::vector<std::string>> lexFileTableData;

    juce::File getAppDataDirectory();

    void parseLexFile(const juce::File& file);

    void buttonClicked(juce::Button* button) override;

    int getNumRows() override;

    void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override {
        if (rowIsSelected)
            g.fillAll(juce::Colours::lightblue);
    }

    void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LexerComponent)
};
