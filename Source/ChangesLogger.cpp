#include "ChangesLogger.h"

void ChangesLogger::addChange(const juce::String& description, const juce::Colour& color) {
    ChangeLog log;
    log.description = description;
    log.color = color;
    log.timestamp = juce::Time::getCurrentTime();
    
    changes.push_back(log);
    
    if (changes.size() > maxChanges) {
        changes.erase(changes.begin());
    }
}

std::vector<ChangeLog> ChangesLogger::getChanges() const {
    return changes;
}

void ChangesLogger::clear() {
    changes.clear();
}

