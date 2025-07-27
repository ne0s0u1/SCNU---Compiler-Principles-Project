#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent() : tabs(juce::TabbedButtonBar::TabsAtTop)
{
    setSize(1400, 950);
    
    lexerTab = std::make_unique<LexerComponent>();
    parserTab = std::make_unique<ParserComponent>();

    tabs.addTab(juce::CharPointer_UTF8("\xe8\xaf\x8d\xe6\xb3\x95\xe5\x88\x86\xe6\x9e\x90"), juce::Colours::grey, lexerTab.get(), false);
    tabs.addTab(juce::CharPointer_UTF8("\xe8\xaf\xad\xe6\xb3\x95\xe5\x88\x86\xe6\x9e\x90"), juce::Colours::grey, parserTab.get(), false);

    

    addAndMakeVisible(tabs);
}

MainComponent::~MainComponent()
{
    tabs.clearTabs();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::whitesmoke);
    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);
}

void MainComponent::resized()
{
    tabs.setBounds(getLocalBounds());
}




