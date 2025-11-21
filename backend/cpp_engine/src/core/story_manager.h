#ifndef STORY_MANAGER_H
#define STORY_MANAGER_H

#include "engine.h"
#include <string>
#include <vector>

class StoryManager {
private:
    Engine* engine;

public:
    StoryManager(Engine* eng);
    
    // Basic story generation
    std::string generateCaseSummary(const std::string& caseTitle);
    std::string generateSuspectProfile(const std::string& suspectName);
    std::string generateCharacterIntroduction(const std::string& characterName);
    std::string generateInvestigationTimeline();
    
    // Analysis reports
    std::string generateCaseAnalysis(const std::string& caseTitle);
    std::string generateSuspicionReport();
    
    // Simple utilities
    std::vector<std::string> findMissingConnections();
    std::vector<std::string> suggestNextSteps();
};

#endif // STORY_MANAGER_H