#include "engine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <queue>

Engine::Engine() : nextCaseId(1), nextSuspectId(1), nextCharacterId(1) {
    std::cout << "🔍 Detective Engine Initialized\n";
}

Engine::~Engine() {
    std::cout << "🔍 Detective Engine Shutdown\n";
}

// ==================== CASE MANAGEMENT ====================
bool Engine::addCase(const std::string& title, const std::string& description, 
                     CaseStatus status, CasePriority priority) {
    if (title.empty() || description.empty()) {
        std::cout << "❌ Cannot add case: Title and description cannot be empty\n";
        return false;
    }

    if (caseTitleIndex.find(title) != caseTitleIndex.end()) {
        std::cout << "❌ Case already exists: " << title << "\n";
        return false;
    }

    Case newCase(nextCaseId++, title, description);
    newCase.setStatus(status);
    newCase.setPriority(priority);
    
    cases.insert(newCase);
    Case* inserted = cases.search(newCase);
    
    if (inserted) {
        addToIndices(inserted);
        autoConnectEntities(inserted);
        std::cout << "✅ Case added: " << title << " (ID: " << inserted->getId() << ")\n";
        return true;
    }
    
    std::cout << "❌ Failed to add case: " << title << "\n";
    return false;
}

bool Engine::removeCase(const std::string& title) {
    auto it = caseTitleIndex.find(title);
    if (it == caseTitleIndex.end()) {
        std::cout << "❌ Case not found: " << title << "\n";
        return false;
    }

    Case* casePtr = it->second;
    int caseId = casePtr->getId();
    
    // Remove from data structure
    cases.remove(*casePtr);
    
    // Remove from indices
    caseTitleIndex.erase(title);
    caseIdIndex.erase(caseId);
    
    // Remove from graph
    relationshipGraph.removeNode(title);
    
    std::cout << "✅ Case removed: " << title << "\n";
    return true;
}

Case* Engine::findCase(const std::string& title) {
    auto it = caseTitleIndex.find(title);
    return it != caseTitleIndex.end() ? it->second : nullptr;
}

Case* Engine::findCaseById(int id) {
    auto it = caseIdIndex.find(id);
    return it != caseIdIndex.end() ? it->second : nullptr;
}

std::vector<Case*> Engine::getAllCases() {
    std::vector<Case*> result;
    cases.inOrderTraversal([&](Case* c) { result.push_back(c); });
    return result;
}

std::vector<Case*> Engine::findCasesByStatus(CaseStatus status) {
    std::vector<Case*> result;
    cases.inOrderTraversal([&](Case* c) {
        if (c->getStatus() == status) result.push_back(c);
    });
    return result;
}

std::vector<Case*> Engine::findCasesByPriority(CasePriority priority) {
    std::vector<Case*> result;
    cases.inOrderTraversal([&](Case* c) {
        if (c->getPriority() == priority) result.push_back(c);
    });
    return result;
}

std::vector<Case*> Engine::searchCases(const std::string& keyword) {
    std::vector<Case*> result;
    cases.inOrderTraversal([&](Case* c) {
        if (c->getTitle().find(keyword) != std::string::npos ||
            c->getDescription().find(keyword) != std::string::npos) {
            result.push_back(c);
        }
    });
    return result;
}

// ==================== SUSPECT MANAGEMENT ====================
bool Engine::addSuspect(const std::string& name, const std::string& background, 
                        const std::string& story, int age, const std::string& occupation) {
    if (name.empty()) {
        std::cout << "❌ Cannot add suspect: Name cannot be empty\n";
        return false;
    }

    if (suspectNameIndex.find(name) != suspectNameIndex.end()) {
        std::cout << "❌ Suspect already exists: " << name << "\n";
        return false;
    }

    Suspect newSuspect(nextSuspectId++, name, story, background, age, occupation);
    suspects.insert(newSuspect);
    Suspect* inserted = suspects.search(newSuspect);
    
    if (inserted) {
        addToIndices(inserted);
        autoConnectEntities(inserted);
        std::cout << "✅ Suspect added: " << name << " (ID: " << inserted->getId() << ")\n";
        return true;
    }
    
    std::cout << "❌ Failed to add suspect: " << name << "\n";
    return false;
}

bool Engine::removeSuspect(const std::string& name) {
    auto it = suspectNameIndex.find(name);
    if (it == suspectNameIndex.end()) {
        std::cout << "❌ Suspect not found: " << name << "\n";
        return false;
    }

    Suspect* suspectPtr = it->second;
    int suspectId = suspectPtr->getId();
    
    suspects.remove(*suspectPtr);
    suspectNameIndex.erase(name);
    suspectIdIndex.erase(suspectId);
    relationshipGraph.removeNode(name);
    
    std::cout << "✅ Suspect removed: " << name << "\n";
    return true;
}

Suspect* Engine::findSuspect(const std::string& name) {
    auto it = suspectNameIndex.find(name);
    return it != suspectNameIndex.end() ? it->second : nullptr;
}

Suspect* Engine::findSuspectById(int id) {
    auto it = suspectIdIndex.find(id);
    if (it != suspectIdIndex.end()) return it->second;
    
    // Fallback search
    std::vector<Suspect*> all = getAllSuspects();
    for (Suspect* s : all) {
        if (s->getId() == id) return s;
    }
    return nullptr;
}

std::vector<Suspect*> Engine::getAllSuspects() {
    std::vector<Suspect*> result;
    suspects.inOrderTraversal([&](Suspect* s) { result.push_back(s); });
    return result;
}

std::vector<Suspect*> Engine::findSuspectsByStatus(SuspectStatus status) {
    std::vector<Suspect*> result;
    suspects.inOrderTraversal([&](Suspect* s) {
        if (s->getStatus() == status) result.push_back(s);
    });
    return result;
}

std::vector<Suspect*> Engine::findSuspectsBySuspicionRange(double minLevel, double maxLevel) {
    std::vector<Suspect*> result;
    suspects.inOrderTraversal([&](Suspect* s) {
        double level = s->getSuspicionLevel();
        if (level >= minLevel && level <= maxLevel) result.push_back(s);
    });
    return result;
}

std::vector<Suspect*> Engine::searchSuspects(const std::string& keyword) {
    std::vector<Suspect*> result;
    suspects.inOrderTraversal([&](Suspect* s) {
        if (s->getName().find(keyword) != std::string::npos ||
            s->getOccupation().find(keyword) != std::string::npos ||
            s->getBackground().find(keyword) != std::string::npos) {
            result.push_back(s);
        }
    });
    return result;
}

// ==================== CHARACTER MANAGEMENT ====================
bool Engine::addCharacter(const std::string& name, CharacterRole role, const std::string& story) {
    if (name.empty()) {
        std::cout << "❌ Cannot add character: Name cannot be empty\n";
        return false;
    }

    if (characterNameIndex.find(name) != characterNameIndex.end()) {
        std::cout << "❌ Character already exists: " << name << "\n";
        return false;
    }

    Character newCharacter(nextCharacterId++, name, role, story);
    characters.insertAtEnd(newCharacter);
    
    // Get the inserted character using traversal
    Character* inserted = nullptr;
    characters.traverse([&](Character& ch) {
        if (ch.getId() == newCharacter.getId()) {
            inserted = &ch;
        }
    });
    
    if (inserted) {
        addToIndices(inserted);
        autoConnectEntities(inserted);
        std::cout << "✅ Character added: " << name << " (Role: " << CharacterUtils::roleToString(role) << ")\n";
        return true;
    }
    
    std::cout << "❌ Failed to add character: " << name << "\n";
    return false;
}

bool Engine::removeCharacter(const std::string& name) {
    auto it = characterNameIndex.find(name);
    if (it == characterNameIndex.end()) {
        std::cout << "❌ Character not found: " << name << "\n";
        return false;
    }

    Character* characterPtr = it->second;
    
    // Find and remove from linked list
    bool removed = false;
    characters.traverse([&](Character& ch) {
        if (ch.getName() == name) {
            characters.deleteNode(ch);
            removed = true;
        }
    });

    if (removed) {
        characterNameIndex.erase(name);
        characterIdIndex.erase(characterPtr->getId());
        relationshipGraph.removeNode(name);
        std::cout << "✅ Character removed: " << name << "\n";
        return true;
    }
    
    std::cout << "❌ Failed to remove character: " << name << "\n";
    return false;
}

Character* Engine::findCharacter(const std::string& name) {
    auto it = characterNameIndex.find(name);
    return it != characterNameIndex.end() ? it->second : nullptr;
}

Character* Engine::findCharacterById(int id) {
    auto it = characterIdIndex.find(id);
    if (it != characterIdIndex.end()) return it->second;
    
    // Fallback search
    Character* found = nullptr;
    characters.traverse([&](Character& ch) {
        if (ch.getId() == id) {
            found = &ch;
        }
    });
    return found;
}

std::vector<Character*> Engine::getAllCharacters() {
    std::vector<Character*> result;
    characters.traverse([&](Character& ch) { result.push_back(&ch); });
    return result;
}

std::vector<Character*> Engine::findCharactersByRole(CharacterRole role) {
    std::vector<Character*> result;
    characters.traverse([&](Character& ch) {
        if (ch.getRole() == role) result.push_back(&ch);
    });
    return result;
}

std::vector<Character*> Engine::searchCharacters(const std::string& keyword) {
    std::vector<Character*> result;
    characters.traverse([&](Character& ch) {
        if (ch.getName().find(keyword) != std::string::npos ||
            ch.getRoleString().find(keyword) != std::string::npos ||
            ch.getStory().find(keyword) != std::string::npos) {
            result.push_back(&ch);
        }
    });
    return result;
}

// ==================== RELATIONSHIP MANAGEMENT ====================
bool Engine::linkSuspectToCase(const std::string& suspectName, const std::string& caseTitle) {
    Suspect* suspect = findSuspect(suspectName);
    Case* casePtr = findCase(caseTitle);
    
    if (!suspect || !casePtr) {
        std::cout << "❌ Cannot link: Suspect or case not found\n";
        return false;
    }

    suspect->addCase(casePtr->getId());
    casePtr->addSuspect(suspect->getId());
    relationshipGraph.addEdge(caseTitle, suspectName);
    relationshipGraph.addEdge(suspectName, caseTitle);
    
    std::cout << "✅ Linked suspect " << suspectName << " to case " << caseTitle << "\n";
    return true;
}

bool Engine::unlinkSuspectFromCase(const std::string& suspectName, const std::string& caseTitle) {
    Suspect* suspect = findSuspect(suspectName);
    Case* casePtr = findCase(caseTitle);
    
    if (!suspect || !casePtr) {
        std::cout << "❌ Cannot unlink: Suspect or case not found\n";
        return false;
    }

    suspect->removeCase(casePtr->getId());
    casePtr->removeSuspect(suspect->getId());
    relationshipGraph.removeEdge(caseTitle, suspectName);
    relationshipGraph.removeEdge(suspectName, caseTitle);
    
    std::cout << "✅ Unlinked suspect " << suspectName << " from case " << caseTitle << "\n";
    return true;
}

bool Engine::linkCharacterToCase(const std::string& characterName, const std::string& caseTitle) {
    Character* character = findCharacter(characterName);
    Case* casePtr = findCase(caseTitle);
    
    if (!character || !casePtr) {
        std::cout << "❌ Cannot link: Character or case not found\n";
        return false;
    }

    character->addCase(casePtr->getId());
    casePtr->addCharacter(character->getId());
    relationshipGraph.addEdge(caseTitle, characterName);
    relationshipGraph.addEdge(characterName, caseTitle);
    
    std::cout << "✅ Linked character " << characterName << " to case " << caseTitle << "\n";
    return true;
}

bool Engine::unlinkCharacterFromCase(const std::string& characterName, const std::string& caseTitle) {
    Character* character = findCharacter(characterName);
    Case* casePtr = findCase(caseTitle);
    
    if (!character || !casePtr) {
        std::cout << "❌ Cannot unlink: Character or case not found\n";
        return false;
    }

    character->removeCase(casePtr->getId());
    casePtr->removeCharacter(character->getId());
    relationshipGraph.removeEdge(caseTitle, characterName);
    relationshipGraph.removeEdge(characterName, caseTitle);
    
    std::cout << "✅ Unlinked character " << characterName << " from case " << caseTitle << "\n";
    return true;
}

bool Engine::addRelationship(const std::string& entity1, const std::string& entity2, 
                             const std::string& relationshipType) {
    // Verify both entities exist
    bool entity1Exists = findCase(entity1) || findSuspect(entity1) || findCharacter(entity1);
    bool entity2Exists = findCase(entity2) || findSuspect(entity2) || findCharacter(entity2);
    
    if (!entity1Exists || !entity2Exists) {
        std::cout << "❌ Cannot create relationship: One or both entities not found\n";
        return false;
    }

    relationshipGraph.addEdge(entity1, entity2);
    relationshipGraph.addEdge(entity2, entity1);
    
    std::cout << "✅ Created relationship: " << entity1 << " <-> " << entity2 
              << " (" << relationshipType << ")\n";
    return true;
}

bool Engine::removeRelationship(const std::string& entity1, const std::string& entity2) {
    relationshipGraph.removeEdge(entity1, entity2);
    relationshipGraph.removeEdge(entity2, entity1);
    
    std::cout << "✅ Removed relationship: " << entity1 << " -X- " << entity2 << "\n";
    return true;
}

std::vector<std::string> Engine::getRelationships(const std::string& entity) {
    return relationshipGraph.getNeighbors(entity);
}

std::vector<std::string> Engine::findPath(const std::string& from, const std::string& to) {
    return relationshipGraph.shortestPath(from, to);
}

// ==================== ANALYSIS & QUERIES ====================
std::vector<Suspect*> Engine::getSuspectsForCase(const std::string& caseTitle) {
    Case* casePtr = findCase(caseTitle);
    if (!casePtr) return {};
    
    std::vector<Suspect*> result;
    for (int suspectId : casePtr->getSuspects()) {
        Suspect* suspect = findSuspectById(suspectId);
        if (suspect) result.push_back(suspect);
    }
    return result;
}

std::vector<Character*> Engine::getCharactersForCase(const std::string& caseTitle) {
    Case* casePtr = findCase(caseTitle);
    if (!casePtr) return {};
    
    std::vector<Character*> result;
    for (int characterId : casePtr->getCharacters()) {
        Character* character = findCharacterById(characterId);
        if (character) result.push_back(character);
    }
    return result;
}

std::vector<Case*> Engine::getCasesForSuspect(const std::string& suspectName) {
    Suspect* suspect = findSuspect(suspectName);
    if (!suspect) return {};
    
    std::vector<Case*> result;
    for (int caseId : suspect->getCases()) {
        Case* casePtr = findCaseById(caseId);
        if (casePtr) result.push_back(casePtr);
    }
    return result;
}

std::vector<Case*> Engine::getCasesForCharacter(const std::string& characterName) {
    Character* character = findCharacter(characterName);
    if (!character) return {};
    
    std::vector<Case*> result;
    for (int caseId : character->getRelatedCases()) {
        Case* casePtr = findCaseById(caseId);
        if (casePtr) result.push_back(casePtr);
    }
    return result;
}

std::vector<Suspect*> Engine::getPrimeSuspects() {
    return findSuspectsByStatus(SuspectStatus::PRIME_SUSPECT);
}

std::vector<Case*> Engine::getUnsolvedCases() {
    std::vector<Case*> result;
    cases.inOrderTraversal([&](Case* c) {
        if (c->getStatus() != CaseStatus::SOLVED) result.push_back(c);
    });
    return result;
}

std::vector<Case*> Engine::getHighPriorityCases() {
    std::vector<Case*> result;
    cases.inOrderTraversal([&](Case* c) {
        if (c->getPriority() == CasePriority::HIGH || c->getPriority() == CasePriority::URGENT) {
            result.push_back(c);
        }
    });
    return result;
}

void Engine::recalculateAllSuspicionLevels() {
    suspects.inOrderTraversal([&](Suspect* s) {
        s->updateSuspicionLevel();
    });
    std::cout << "✅ Recalculated suspicion levels for all suspects\n";
}

std::vector<Suspect*> Engine::getTopSuspects(int count) {
    std::vector<Suspect*> allSuspects = getAllSuspects();
    std::sort(allSuspects.begin(), allSuspects.end(), 
              [](Suspect* a, Suspect* b) {
                  return a->getSuspicionLevel() > b->getSuspicionLevel();
              });
    
    if ((size_t)count > allSuspects.size()) count = allSuspects.size();
    return std::vector<Suspect*>(allSuspects.begin(), allSuspects.begin() + count);
}

std::vector<Suspect*> Engine::findConnectedSuspects(const std::string& suspectName, int maxDepth) {
    std::vector<Suspect*> result;
    std::unordered_map<std::string, bool> visited;
    std::queue<std::pair<std::string, int>> q;
    
    q.push({suspectName, 0});
    visited[suspectName] = true;
    
    while (!q.empty()) {
        auto [current, depth] = q.front();
        q.pop();
        
        if (depth > 0) { // Don't include the starting suspect
            Suspect* suspect = findSuspect(current);
            if (suspect) result.push_back(suspect);
        }
        
        if (depth < maxDepth) {
            for (const auto& neighbor : relationshipGraph.getNeighbors(current)) {
                if (!visited[neighbor] && findSuspect(neighbor)) {
                    visited[neighbor] = true;
                    q.push({neighbor, depth + 1});
                }
            }
        }
    }
    
    return result;
}

// ==================== STATISTICS ====================
Engine::Statistics Engine::getStatistics() {
    Statistics stats{};
    
    stats.totalCases = getAllCases().size();
    stats.solvedCases = findCasesByStatus(CaseStatus::SOLVED).size();
    stats.openCases = stats.totalCases - stats.solvedCases;
    stats.totalSuspects = getAllSuspects().size();
    stats.primeSuspects = getPrimeSuspects().size();
    
    // Count cleared suspects
    int clearedCount = 0;
    suspects.inOrderTraversal([&](Suspect* s) {
        if (s->isCleared()) clearedCount++;
    });
    stats.clearedSuspects = clearedCount;
    
    stats.totalCharacters = getAllCharacters().size();
    stats.witnesses = findCharactersByRole(CharacterRole::WITNESS).size();
    stats.detectives = findCharactersByRole(CharacterRole::DETECTIVE).size();
    stats.totalRelationships = relationshipGraph.getEdgeCount() / 2; // Undirected edges
    
    // Calculate average suspicion
    double totalSuspicion = 0.0;
    int suspectCount = 0;
    suspects.inOrderTraversal([&](Suspect* s) {
        totalSuspicion += s->getSuspicionLevel();
        suspectCount++;
    });
    stats.averageSuspicionLevel = suspectCount > 0 ? totalSuspicion / suspectCount : 0.0;
    
    return stats;
}

void Engine::printStatistics() {
    Statistics stats = getStatistics();
    
    std::cout << "\n=== DETECTIVE ENGINE STATISTICS ===\n";
    std::cout << "📊 Cases: " << stats.totalCases 
              << " (Solved: " << stats.solvedCases 
              << ", Open: " << stats.openCases << ")\n";
    std::cout << "🕵️ Suspects: " << stats.totalSuspects 
              << " (Prime: " << stats.primeSuspects 
              << ", Cleared: " << stats.clearedSuspects << ")\n";
    std::cout << "👥 Characters: " << stats.totalCharacters 
              << " (Witnesses: " << stats.witnesses 
              << ", Detectives: " << stats.detectives << ")\n";
    std::cout << "🔗 Relationships: " << stats.totalRelationships << "\n";
    std::cout << "📈 Average Suspicion Level: " << std::fixed << std::setprecision(1) 
              << stats.averageSuspicionLevel << "%\n";
}

// ==================== DATA INTEGRITY ====================
bool Engine::validateData() {
    return getDataIssues().empty();
}

std::vector<std::string> Engine::getDataIssues() {
    std::vector<std::string> issues;
    
    // Check for orphaned references
    cases.inOrderTraversal([&](Case* c) {
        for (int suspectId : c->getSuspects()) {
            if (!findSuspectById(suspectId)) {
                issues.push_back("Case '" + c->getTitle() + "' references non-existent suspect ID " + std::to_string(suspectId));
            }
        }
        for (int characterId : c->getCharacters()) {
            if (!findCharacterById(characterId)) {
                issues.push_back("Case '" + c->getTitle() + "' references non-existent character ID " + std::to_string(characterId));
            }
        }
    });
    
    // Check for duplicate names
    std::unordered_map<std::string, std::string> nameMap;
    cases.inOrderTraversal([&](Case* c) {
        if (nameMap.count(c->getTitle())) {
            issues.push_back("Duplicate case title: " + c->getTitle());
        }
        nameMap[c->getTitle()] = "case";
    });
    
    suspects.inOrderTraversal([&](Suspect* s) {
        if (nameMap.count(s->getName())) {
            issues.push_back("Duplicate name: " + s->getName() + " (already used as " + nameMap[s->getName()] + ")");
        }
        nameMap[s->getName()] = "suspect";
    });
    
    characters.traverse([&](Character& ch) {
        if (nameMap.count(ch.getName())) {
            issues.push_back("Duplicate name: " + ch.getName() + " (already used as " + nameMap[ch.getName()] + ")");
        }
        nameMap[ch.getName()] = "character";
    });
    
    return issues;
}

void Engine::rebuildAllConnections() {
    relationshipGraph.clear();
    
    // Rebuild all connections
    cases.inOrderTraversal([&](Case* c) { autoConnectEntities(c); });
    suspects.inOrderTraversal([&](Suspect* s) { autoConnectEntities(s); });
    characters.traverse([&](Character& ch) { autoConnectEntities(&ch); });
    
    std::cout << "✅ Rebuilt all connections\n";
}

// ==================== UTILITY METHODS ====================
void Engine::displayAllData() {
    std::cout << "\n=== DETECTIVE ENGINE DATA ===\n";
    
    std::cout << "\n📁 CASES:\n";
    cases.inOrderTraversal([](Case* c) { c->displaySummary(); });
    
    std::cout << "\n🕵️ SUSPECTS:\n";
    suspects.inOrderTraversal([](Suspect* s) { s->displaySummary(); });
    
    std::cout << "\n👥 CHARACTERS:\n";
    characters.traverse([](Character& c) { c.displaySummary(); });
    
    std::cout << "\n🔗 RELATIONSHIP NETWORK:\n";
    relationshipGraph.displayGraph();
}

void Engine::displayCaseNetwork(const std::string& caseTitle) {
    Case* casePtr = findCase(caseTitle);
    if (!casePtr) {
        std::cout << "❌ Case not found: " << caseTitle << "\n";
        return;
    }
    
    std::cout << "\n🔗 NETWORK FOR CASE: " << caseTitle << "\n";
    std::cout << "Connected entities:\n";
    
    auto connections = getRelationships(caseTitle);
    for (const auto& connection : connections) {
        std::cout << "  • " << connection << "\n";
    }
}

void Engine::displaySuspectNetwork(const std::string& suspectName) {
    Suspect* suspect = findSuspect(suspectName);
    if (!suspect) {
        std::cout << "❌ Suspect not found: " << suspectName << "\n";
        return;
    }
    
    std::cout << "\n🔗 NETWORK FOR SUSPECT: " << suspectName << "\n";
    std::cout << "Connected entities:\n";
    
    auto connections = getRelationships(suspectName);
    for (const auto& connection : connections) {
        std::cout << "  • " << connection << "\n";
    }
    
    auto connectedSuspects = findConnectedSuspects(suspectName, 2);
    if (!connectedSuspects.empty()) {
        std::cout << "\nConnected suspects (within 2 degrees):\n";
        for (auto* connected : connectedSuspects) {
            std::cout << "  • " << connected->getName() << " (" << connected->getSuspicionLevel() << "%)\n";
        }
    }
}

// ==================== PRIVATE HELPER METHODS ====================
void Engine::addToIndices(Case* casePtr) {
    caseTitleIndex[casePtr->getTitle()] = casePtr;
    caseIdIndex[casePtr->getId()] = casePtr;
    relationshipGraph.addNode(casePtr->getTitle());
}

void Engine::addToIndices(Suspect* suspectPtr) {
    suspectNameIndex[suspectPtr->getName()] = suspectPtr;
    suspectIdIndex[suspectPtr->getId()] = suspectPtr;
    relationshipGraph.addNode(suspectPtr->getName());
}

void Engine::addToIndices(Character* characterPtr) {
    characterNameIndex[characterPtr->getName()] = characterPtr;
    characterIdIndex[characterPtr->getId()] = characterPtr;
    relationshipGraph.addNode(characterPtr->getName());
}

void Engine::autoConnectEntities(Case* casePtr) {
    // Cases automatically connect to their suspects and characters
    for (int suspectId : casePtr->getSuspects()) {
        Suspect* suspect = findSuspectById(suspectId);
        if (suspect) {
            relationshipGraph.addEdge(casePtr->getTitle(), suspect->getName());
        }
    }
    
    for (int characterId : casePtr->getCharacters()) {
        Character* character = findCharacterById(characterId);
        if (character) {
            relationshipGraph.addEdge(casePtr->getTitle(), character->getName());
        }
    }
}

void Engine::autoConnectEntities(Suspect* suspectPtr) {
    // Suspects automatically connect to their cases
    for (int caseId : suspectPtr->getCases()) {
        Case* casePtr = findCaseById(caseId);
        if (casePtr) {
            relationshipGraph.addEdge(suspectPtr->getName(), casePtr->getTitle());
        }
    }
}

void Engine::autoConnectEntities(Character* characterPtr) {
    // Characters automatically connect to their cases
    for (int caseId : characterPtr->getRelatedCases()) {
        Case* casePtr = findCaseById(caseId);
        if (casePtr) {
            relationshipGraph.addEdge(characterPtr->getName(), casePtr->getTitle());
        }
    }
}

// ==================== DEBUG METHODS ====================
void Engine::printDebugInfo() {
    std::cout << "\n=== ENGINE DEBUG INFO ===\n";
    std::cout << "Next Case ID: " << nextCaseId << "\n";
    std::cout << "Next Suspect ID: " << nextSuspectId << "\n";
    std::cout << "Next Character ID: " << nextCharacterId << "\n";
    std::cout << "Cases in index: " << caseTitleIndex.size() << "\n";
    std::cout << "Suspects in index: " << suspectNameIndex.size() << "\n";
    std::cout << "Characters in index: " << characterNameIndex.size() << "\n";
    std::cout << "Graph nodes: " << relationshipGraph.getAllNodes().size() << "\n";
    std::cout << "Graph edges: " << relationshipGraph.getEdgeCount() << "\n";
    
    auto issues = getDataIssues();
    if (!issues.empty()) {
        std::cout << "\n⚠️ DATA ISSUES:\n";
        for (const auto& issue : issues) {
            std::cout << "  • " << issue << "\n";
        }
    }
}

// ==================== MISSING FUNCTION IMPLEMENTATIONS ====================

bool Engine::updateCase(const std::string& title, const std::string& newDescription, 
                       CaseStatus newStatus, CasePriority newPriority) {
    Case* casePtr = findCase(title);
    if (!casePtr) {
        std::cout << "❌ Case not found: " << title << "\n";
        return false;
    }

    if (!newDescription.empty()) {
        casePtr->setDescription(newDescription);
    }
    
    casePtr->setStatus(newStatus);
    casePtr->setPriority(newPriority);
    
    std::cout << "✅ Case updated: " << title << "\n";
    return true;
}

bool Engine::updateSuspect(const std::string& name, const std::string& newBackground, 
                          const std::string& newStory, int newAge, 
                          const std::string& newOccupation) {
    Suspect* suspect = findSuspect(name);
    if (!suspect) {
        std::cout << "❌ Suspect not found: " << name << "\n";
        return false;
    }

    if (!newBackground.empty()) {
        suspect->setBackground(newBackground);
    }
    
    if (!newStory.empty()) {
        suspect->setStory(newStory);
    }
    
    if (newAge > 0) {
        suspect->setAge(newAge);
    }
    
    if (!newOccupation.empty() && newOccupation != "Unknown") {
        suspect->setOccupation(newOccupation);
    }
    
    // Recalculate suspicion level after update
    suspect->updateSuspicionLevel();
    
    std::cout << "✅ Suspect updated: " << name << "\n";
    return true;
}

bool Engine::updateCharacter(const std::string& name, CharacterRole newRole, 
                            const std::string& newStory) {
    Character* character = findCharacter(name);
    if (!character) {
        std::cout << "❌ Character not found: " << name << "\n";
        return false;
    }

    character->setRole(newRole);
    
    if (!newStory.empty()) {
        character->setStory(newStory);
    }
    
    std::cout << "✅ Character updated: " << name << "\n";
    return true;
}