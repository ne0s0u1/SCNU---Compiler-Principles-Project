#pragma once
#include <JuceHeader.h>
#include "LexerComponent.h"
#include "ParserComponent.h"

using namespace std;


//==============================================================================

class MainComponent  : public juce::Component
{
public:
    
    MainComponent();
    ~MainComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TabbedComponent tabs;
    

   std::unique_ptr<LexerComponent> lexerTab;
   std::unique_ptr<ParserComponent> parserTab;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


