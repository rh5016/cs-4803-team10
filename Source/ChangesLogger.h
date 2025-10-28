#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <vector>

struct ChangeLog {
    juce::String description;
    juce::Colour color;
    juce::Time timestamp;
};

class ChangesLogger {
public:
    void addChange(const juce::String& description, const juce::Colour& color);
    std::vector<ChangeLog> getChanges() const;
    void clear();
    
private:
    std::vector<ChangeLog> changes;
    static constexpr int maxChanges = 20;
};

