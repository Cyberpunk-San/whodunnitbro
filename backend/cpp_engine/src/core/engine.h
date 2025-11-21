#ifndef ENGINE_H
#define ENGINE_H

#include "../models/case.h"
#include "../models/character.h"
#include "../models/suspect.h"
#include "../data_structures/avl_tree.h"
#include "../data_structures/rb_tree.h"
#include "../data_structures/linked_list.h"
#include "../data_structures/graph.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

class Engine {
private:
    // Core data storage
    AVLTree<Case> cases;
    RBTree<Suspect> suspects;
    LinkedList<Character> characters;
    Graph relationshipGraph;

    // Indexing for fast lookup
    std::unordered_map<std::string, Case*> caseTitleIndex;
    std::unordered_map<std::string, Suspect*> suspectNameIndex;
    std::unordered_map<std::string, Character*> characterNameIndex;
    std::unordered_map<int, Case*> caseIdIndex;
    std::unordered_map<int, Suspect*> suspectIdIndex;
    std::unordered_map<int, Character*> characterIdIndex;

    // ID counters
    int nextCaseId;
    int nextSuspectId;
    int nextCharacterId;

    // Private helper methods
    void rebuildIndices();
    void addToIndices(Case* casePtr);
    void addToIndices(Suspect* suspectPtr);
    void addToIndices(Character* characterPtr);
    void removeFromIndices(const std::string& caseTitle, const std::string& suspectName, const std::string& characterName);
    void autoConnectEntities(Case* casePtr);
    void autoConnectEntities(Suspect* suspectPtr);
    void autoConnectEntities(Character* characterPtr);

public:
    Engine();
    ~Engine();

    // ==================== CASE MANAGEMENT ====================
    bool addCase(const std::string& title, const std::string& description, 
                 CaseStatus status = CaseStatus::OPEN, CasePriority priority = CasePriority::MEDIUM);
    bool removeCase(const std::string& title);
    bool updateCase(const std::string& title, const std::string& newDescription, 
                    CaseStatus newStatus, CasePriority newPriority);
    Case* findCase(const std::string& title);
    Case* findCaseById(int id);
    std::vector<Case*> getAllCases();
    std::vector<Case*> findCasesByStatus(CaseStatus status);
    std::vector<Case*> findCasesByPriority(CasePriority priority);
    std::vector<Case*> searchCases(const std::string& keyword);

    // ==================== SUSPECT MANAGEMENT ====================
    bool addSuspect(const std::string& name, const std::string& background, 
                    const std::string& story = "", int age = 0, 
                    const std::string& occupation = "Unknown");
    bool removeSuspect(const std::string& name);
    bool updateSuspect(const std::string& name, const std::string& newBackground, 
                       const std::string& newStory, int newAge, 
                       const std::string& newOccupation);
    Suspect* findSuspect(const std::string& name);
    Suspect* findSuspectById(int id);
    std::vector<Suspect*> getAllSuspects();
    std::vector<Suspect*> findSuspectsByStatus(SuspectStatus status);
    std::vector<Suspect*> findSuspectsBySuspicionRange(double minLevel, double maxLevel);
    std::vector<Suspect*> searchSuspects(const std::string& keyword);

    // ==================== CHARACTER MANAGEMENT ====================
    bool addCharacter(const std::string& name, CharacterRole role, 
                      const std::string& story = "");
    bool removeCharacter(const std::string& name);
    bool updateCharacter(const std::string& name, CharacterRole newRole, 
                         const std::string& newStory);
    Character* findCharacter(const std::string& name);
    Character* findCharacterById(int id);
    std::vector<Character*> getAllCharacters();
    std::vector<Character*> findCharactersByRole(CharacterRole role);
    std::vector<Character*> searchCharacters(const std::string& keyword);

    // ==================== RELATIONSHIP MANAGEMENT ====================
    bool linkSuspectToCase(const std::string& suspectName, const std::string& caseTitle);
    bool unlinkSuspectFromCase(const std::string& suspectName, const std::string& caseTitle);
    bool linkCharacterToCase(const std::string& characterName, const std::string& caseTitle);
    bool unlinkCharacterFromCase(const std::string& characterName, const std::string& caseTitle);
    bool addRelationship(const std::string& entity1, const std::string& entity2, 
                         const std::string& relationshipType = "related");
    bool removeRelationship(const std::string& entity1, const std::string& entity2);
    std::vector<std::string> getRelationships(const std::string& entity);
    std::vector<std::string> findPath(const std::string& from, const std::string& to);

    // ==================== ANALYSIS & QUERIES ====================
    std::vector<Suspect*> getSuspectsForCase(const std::string& caseTitle);
    std::vector<Character*> getCharactersForCase(const std::string& caseTitle);
    std::vector<Case*> getCasesForSuspect(const std::string& suspectName);
    std::vector<Case*> getCasesForCharacter(const std::string& characterName);
    
    std::vector<Suspect*> getPrimeSuspects();
    std::vector<Case*> getUnsolvedCases();
    std::vector<Case*> getHighPriorityCases();
    
    // Suspicion analysis
    void recalculateAllSuspicionLevels();
    std::vector<Suspect*> getTopSuspects(int count = 5);
    std::vector<Suspect*> findConnectedSuspects(const std::string& suspectName, int maxDepth = 2);

    // ==================== STATISTICS ====================
    struct Statistics {
        int totalCases;
        int solvedCases;
        int openCases;
        int totalSuspects;
        int primeSuspects;
        int clearedSuspects;
        int totalCharacters;
        int witnesses;
        int detectives;
        double averageSuspicionLevel;
        int totalRelationships;
    };

    Statistics getStatistics();
    void printStatistics();

    // ==================== DATA INTEGRITY ====================
    bool validateData();
    std::vector<std::string> getDataIssues();
    void rebuildAllConnections();

    // ==================== UTILITY METHODS ====================
    void displayAllData();
    void displayCaseNetwork(const std::string& caseTitle);
    void displaySuspectNetwork(const std::string& suspectName);
    
    // Debug
    void printDebugInfo();
};

#endif // ENGINE_H