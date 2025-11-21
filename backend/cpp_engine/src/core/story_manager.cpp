#include "story_manager.h"
#include <sstream>
#include <algorithm>

StoryManager::StoryManager(Engine* eng) : engine(eng) {}

std::string StoryManager::generateCaseSummary(const std::string& caseTitle) {
    auto* casePtr = engine->findCase(caseTitle);
    if (!casePtr) return "Case not found: " + caseTitle;
    
    std::ostringstream story;
    story << "CASE SUMMARY: " << casePtr->getTitle() << "\n";
    story << "=====================\n\n";
    story << "Description: " << casePtr->getDescription() << "\n";
    story << "Status: " << casePtr->getStatusString() << "\n";
    story << "Priority: " << casePtr->getPriorityString() << "\n\n";
    
    // Get suspects for this case
    auto suspects = engine->getSuspectsForCase(caseTitle);
    if (!suspects.empty()) {
        story << "SUSPECTS:\n";
        for (auto* suspect : suspects) {
            story << "• " << suspect->getName() << " - " << suspect->getStatusString();
            story << " (" << suspect->getSuspicionLevel() << "% suspicion)\n";
        }
        story << "\n";
    }
    
    return story.str();
}

std::string StoryManager::generateSuspectProfile(const std::string& suspectName) {
    auto* suspect = engine->findSuspect(suspectName);
    if (!suspect) return "Suspect not found: " + suspectName;
    
    std::ostringstream profile;
    profile << "SUSPECT PROFILE: " << suspect->getName() << "\n";
    profile << "=====================\n\n";
    profile << "Background: " << suspect->getBackground() << "\n";
    profile << "Occupation: " << suspect->getOccupation() << " | Age: " << suspect->getAge() << "\n";
    profile << "Status: " << suspect->getStatusString() << "\n";
    profile << "Suspicion Level: " << suspect->getSuspicionLevel() << "%\n\n";
    
    if (!suspect->getMotive().empty()) {
        profile << "Motive: " << suspect->getMotive() << "\n";
    }
    if (!suspect->getAlibi().empty()) {
        profile << "Alibi: " << suspect->getAlibi() << "\n";
    }
    
    return profile.str();
}

std::string StoryManager::generateCharacterIntroduction(const std::string& characterName) {
    auto* character = engine->findCharacter(characterName);
    if (!character) return "Character not found: " + characterName;
    
    std::ostringstream intro;
    intro << character->getName() << ", a " << character->getRoleString();
    intro << ", is involved in the investigation. ";
    intro << character->getStory();
    
    return intro.str();
}

std::string StoryManager::generateInvestigationTimeline() {
    auto cases = engine->getAllCases();
    
    std::ostringstream timeline;
    timeline << "INVESTIGATION TIMELINE\n";
    timeline << "======================\n\n";
    
    for (auto* casePtr : cases) {
        timeline << "• " << casePtr->getTitle() << " - " << casePtr->getStatusString() << "\n";
    }
    
    return timeline.str();
}

std::string StoryManager::generateCaseAnalysis(const std::string& caseTitle) {
    auto* casePtr = engine->findCase(caseTitle);
    if (!casePtr) return "Case not found: " + caseTitle;
    
    auto suspects = engine->getSuspectsForCase(caseTitle);
    
    std::ostringstream analysis;
    analysis << "CASE ANALYSIS: " << casePtr->getTitle() << "\n";
    analysis << "==================\n\n";
    analysis << "Suspect Count: " << suspects.size() << "\n";
    analysis << "Case Status: " << casePtr->getStatusString() << "\n";
    analysis << "Priority: " << casePtr->getPriorityString() << "\n\n";
    
    if (!suspects.empty()) {
        analysis << "SUSPECT ANALYSIS:\n";
        for (auto* suspect : suspects) {
            analysis << "• " << suspect->getName() << ": " << suspect->getSuspicionLevel() << "% suspicion";
            analysis << " | Evidence: " << suspect->getEvidenceAgainst().size() << " against, ";
            analysis << suspect->getEvidenceFor().size() << " for\n";
        }
    }
    
    return analysis.str();
}

std::string StoryManager::generateSuspicionReport() {
    auto topSuspects = engine->getTopSuspects(5);
    
    std::ostringstream report;
    report << "TOP SUSPECTS REPORT\n";
    report << "===================\n\n";
    
    for (size_t i = 0; i < topSuspects.size(); ++i) {
        report << (i + 1) << ". " << topSuspects[i]->getName();
        report << " (" << topSuspects[i]->getSuspicionLevel() << "%)\n";
    }
    
    return report.str();
}

std::vector<std::string> StoryManager::findMissingConnections() {
    std::vector<std::string> issues;
    
    auto cases = engine->getAllCases();
    for (auto* casePtr : cases) {
        auto suspects = engine->getSuspectsForCase(casePtr->getTitle());
        if (suspects.empty()) {
            issues.push_back("Case '" + casePtr->getTitle() + "' has no suspects");
        }
    }
    
    auto suspects = engine->getAllSuspects();
    for (auto* suspect : suspects) {
        auto casesForSuspect = engine->getCasesForSuspect(suspect->getName());
        if (casesForSuspect.empty()) {
            issues.push_back("Suspect '" + suspect->getName() + "' is not linked to any cases");
        }
    }
    
    return issues;
}

std::vector<std::string> StoryManager::suggestNextSteps() {
    std::vector<std::string> suggestions;
    
    auto unsolvedCases = engine->getUnsolvedCases();
    if (!unsolvedCases.empty()) {
        suggestions.push_back("Focus on solving " + std::to_string(unsolvedCases.size()) + " unsolved cases");
    }
    
    auto highSuspicion = engine->findSuspectsBySuspicionRange(70, 100);
    if (!highSuspicion.empty()) {
        suggestions.push_back("Investigate " + std::to_string(highSuspicion.size()) + " high-suspicion suspects");
    }
    
    auto missingConnections = findMissingConnections();
    if (!missingConnections.empty()) {
        suggestions.push_back("Resolve " + std::to_string(missingConnections.size()) + " data inconsistencies");
    }
    
    return suggestions;
}