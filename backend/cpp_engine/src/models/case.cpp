#include "case.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <stdexcept>

// Constructor
Case::Case() : id(0), title(""), description(""), location("Unknown"), 
               status(CaseStatus::OPEN), priority(CasePriority::MEDIUM) {
    dateCreated = std::chrono::system_clock::now();
    dateModified = dateCreated;
    incidentDate = dateCreated;
}

Case::Case(int id, const std::string& title, const std::string& description)
    : id(id), title(title), description(description), location("Unknown"),
      status(CaseStatus::OPEN), priority(CasePriority::MEDIUM) {
    dateCreated = std::chrono::system_clock::now();
    dateModified = dateCreated;
    incidentDate = dateCreated;
}

Case::Case(int id, const std::string& title, const std::string& description,
           const std::string& location, CasePriority priority)
    : id(id), title(title), description(description), location(location),
      status(CaseStatus::OPEN), priority(priority) {
    dateCreated = std::chrono::system_clock::now();
    dateModified = dateCreated;
    incidentDate = dateCreated;
}

// Getters
int Case::getId() const { return id; }

std::string Case::getTitle() const { return title; }

std::string Case::getDescription() const { return description; }

std::string Case::getLocation() const { return location; }

std::string Case::getStatusString() const {
    return CaseUtils::statusToString(status);
}

std::string Case::getPriorityString() const {
    return CaseUtils::priorityToString(priority);
}

CaseStatus Case::getStatus() const { return status; }

CasePriority Case::getPriority() const { return priority; }

std::string Case::getSolution() const { return solution; }

std::string Case::getNotes() const { return notes; }

std::vector<int> Case::getSuspects() const { return suspectIds; }

std::vector<int> Case::getCharacters() const { return characterIds; }

std::vector<std::string> Case::getEvidence() const { return evidence; }

std::vector<std::string> Case::getTags() const { return tags; }

std::string Case::getCreationDate() const {
    auto time = std::chrono::system_clock::to_time_t(dateCreated);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Case::getModificationDate() const {
    auto time = std::chrono::system_clock::to_time_t(dateModified);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Case::getIncidentDate() const {
    auto time = std::chrono::system_clock::to_time_t(incidentDate);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Setters
void Case::setTitle(const std::string& newTitle) { 
    title = newTitle; 
    updateModificationDate();
}

void Case::setDescription(const std::string& newDescription) { 
    description = newDescription; 
    updateModificationDate();
}

void Case::setLocation(const std::string& newLocation) { 
    location = newLocation; 
    updateModificationDate();
}

void Case::setStatus(CaseStatus newStatus) { 
    status = newStatus; 
    updateModificationDate();
}

void Case::setPriority(CasePriority newPriority) { 
    priority = newPriority; 
    updateModificationDate();
}

void Case::setSolution(const std::string& newSolution) { 
    solution = newSolution; 
    updateModificationDate();
}

void Case::setNotes(const std::string& newNotes) { 
    notes = newNotes; 
    updateModificationDate();
}

void Case::setIncidentDate(const std::chrono::system_clock::time_point& date) {
    incidentDate = date;
    updateModificationDate();
}

// Management methods
void Case::addSuspect(int suspectId) {
    if (std::find(suspectIds.begin(), suspectIds.end(), suspectId) == suspectIds.end()) {
        suspectIds.push_back(suspectId);
        updateModificationDate();
    }
}

void Case::removeSuspect(int suspectId) {
    auto it = std::find(suspectIds.begin(), suspectIds.end(), suspectId);
    if (it != suspectIds.end()) {
        suspectIds.erase(it);
        updateModificationDate();
    }
}

void Case::addCharacter(int characterId) {
    if (std::find(characterIds.begin(), characterIds.end(), characterId) == characterIds.end()) {
        characterIds.push_back(characterId);
        updateModificationDate();
    }
}

void Case::removeCharacter(int characterId) {
    auto it = std::find(characterIds.begin(), characterIds.end(), characterId);
    if (it != characterIds.end()) {
        characterIds.erase(it);
        updateModificationDate();
    }
}

void Case::addEvidence(const std::string& evidenceItem) {
    if (std::find(evidence.begin(), evidence.end(), evidenceItem) == evidence.end()) {
        evidence.push_back(evidenceItem);
        updateModificationDate();
    }
}

void Case::removeEvidence(const std::string& evidenceItem) {
    auto it = std::find(evidence.begin(), evidence.end(), evidenceItem);
    if (it != evidence.end()) {
        evidence.erase(it);
        updateModificationDate();
    }
}

void Case::addTag(const std::string& tag) {
    if (std::find(tags.begin(), tags.end(), tag) == tags.end()) {
        tags.push_back(tag);
        updateModificationDate();
    }
}

void Case::removeTag(const std::string& tag) {
    auto it = std::find(tags.begin(), tags.end(), tag);
    if (it != tags.end()) {
        tags.erase(it);
        updateModificationDate();
    }
}

void Case::clearEvidence() {
    evidence.clear();
    updateModificationDate();
}

void Case::clearTags() {
    tags.clear();
    updateModificationDate();
}

// Utility methods
bool Case::isSolved() const {
    return status == CaseStatus::SOLVED;
}

bool Case::isColdCase() const {
    return status == CaseStatus::COLD;
}

bool Case::involvesSuspect(int suspectId) const {
    return std::find(suspectIds.begin(), suspectIds.end(), suspectId) != suspectIds.end();
}

bool Case::involvesCharacter(int characterId) const {
    return std::find(characterIds.begin(), characterIds.end(), characterId) != characterIds.end();
}

bool Case::hasEvidence(const std::string& evidenceItem) const {
    return std::find(evidence.begin(), evidence.end(), evidenceItem) != evidence.end();
}

bool Case::hasTag(const std::string& tag) const {
    return std::find(tags.begin(), tags.end(), tag) != tags.end();
}

int Case::getDaysSinceIncident() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - incidentDate);
    return duration.count() / 24;
}

int Case::getDaysSinceCreation() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - dateCreated);
    return duration.count() / 24;
}

void Case::updateModificationDate() {
    dateModified = std::chrono::system_clock::now();
}

// Display methods
void Case::display() const {
    std::cout << "Case: " << title << " (ID: " << id << ")\n";
    std::cout << "Status: " << getStatusString() << " | Priority: " << getPriorityString() << "\n";
    std::cout << "Description: " << description << "\n";
}

void Case::displaySummary() const {
    std::cout << "🔍 CASE #" << id << ": " << title << "\n";
    std::cout << "   Status: " << getStatusString() << " | Priority: " << getPriorityString() << "\n";
    std::cout << "   Location: " << location << " | Created: " << getCreationDate() << "\n";
    std::cout << "   Suspects: " << suspectIds.size() << " | Evidence: " << evidence.size() << "\n";
}

void Case::displayDetailed() const {
    std::cout << "========================================\n";
    std::cout << "🔍 CASE DETAILS\n";
    std::cout << "========================================\n";
    std::cout << "ID: " << id << "\n";
    std::cout << "Title: " << title << "\n";
    std::cout << "Status: " << getStatusString() << "\n";
    std::cout << "Priority: " << getPriorityString() << "\n";
    std::cout << "Location: " << location << "\n";
    std::cout << "Incident Date: " << getIncidentDate() << "\n";
    std::cout << "Created: " << getCreationDate() << "\n";
    std::cout << "Modified: " << getModificationDate() << "\n";
    std::cout << "Description: " << description << "\n";
    
    if (!solution.empty()) {
        std::cout << "Solution: " << solution << "\n";
    }
    
    if (!notes.empty()) {
        std::cout << "Notes: " << notes << "\n";
    }
    
    std::cout << "Suspects (" << suspectIds.size() << "): ";
    for (int suspectId : suspectIds) {
        std::cout << suspectId << " ";
    }
    std::cout << "\n";
    
    std::cout << "Characters (" << characterIds.size() << "): ";
    for (int charId : characterIds) {
        std::cout << charId << " ";
    }
    std::cout << "\n";
    
    std::cout << "Evidence (" << evidence.size() << "):\n";
    for (const auto& item : evidence) {
        std::cout << "  • " << item << "\n";
    }
    
    std::cout << "Tags (" << tags.size() << "): ";
    for (const auto& tag : tags) {
        std::cout << "#" << tag << " ";
    }
    std::cout << "\n";
}

std::string Case::to_string() const {
    std::stringstream ss;
    ss << "Case{ID:" << id << ", Title:\"" << title << "\", Status:" << getStatusString() << "}";
    return ss.str();
}

// Serialization
std::string Case::serialize() const {
    std::stringstream ss;
    ss << id << "|" << title << "|" << description << "|" << location << "|"
       << static_cast<int>(status) << "|" << static_cast<int>(priority) << "|"
       << solution << "|" << notes;
    
    // Serialize suspect IDs
    ss << "|";
    for (size_t i = 0; i < suspectIds.size(); ++i) {
        if (i > 0) ss << ",";
        ss << suspectIds[i];
    }
    
    // Serialize character IDs
    ss << "|";
    for (size_t i = 0; i < characterIds.size(); ++i) {
        if (i > 0) ss << ",";
        ss << characterIds[i];
    }
    
    // Serialize evidence
    ss << "|";
    for (size_t i = 0; i < evidence.size(); ++i) {
        if (i > 0) ss << ",";
        ss << evidence[i];
    }
    
    // Serialize tags
    ss << "|";
    for (size_t i = 0; i < tags.size(); ++i) {
        if (i > 0) ss << ",";
        ss << tags[i];
    }
    
    // Serialize dates
    ss << "|" << std::chrono::system_clock::to_time_t(dateCreated);
    ss << "|" << std::chrono::system_clock::to_time_t(dateModified);
    ss << "|" << std::chrono::system_clock::to_time_t(incidentDate);
    
    return ss.str();
}

Case Case::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() < 8) {
        throw std::invalid_argument("Invalid case data format");
    }
    
    Case c(std::stoi(tokens[0]), tokens[1], tokens[2]);
    c.setLocation(tokens[3]);
    c.setStatus(static_cast<CaseStatus>(std::stoi(tokens[4])));
    c.setPriority(static_cast<CasePriority>(std::stoi(tokens[5])));
    c.setSolution(tokens[6]);
    c.setNotes(tokens[7]);
    
    // Deserialize suspect IDs
    if (tokens.size() > 8 && !tokens[8].empty()) {
        std::stringstream suspectStream(tokens[8]);
        std::string suspectToken;
        while (std::getline(suspectStream, suspectToken, ',')) {
            c.addSuspect(std::stoi(suspectToken));
        }
    }
    
    // Deserialize character IDs
    if (tokens.size() > 9 && !tokens[9].empty()) {
        std::stringstream charStream(tokens[9]);
        std::string charToken;
        while (std::getline(charStream, charToken, ',')) {
            c.addCharacter(std::stoi(charToken));
        }
    }
    
    // Deserialize evidence
    if (tokens.size() > 10 && !tokens[10].empty()) {
        std::stringstream evidenceStream(tokens[10]);
        std::string evidenceToken;
        while (std::getline(evidenceStream, evidenceToken, ',')) {
            c.addEvidence(evidenceToken);
        }
    }
    
    // Deserialize tags
    if (tokens.size() > 11 && !tokens[11].empty()) {
        std::stringstream tagStream(tokens[11]);
        std::string tagToken;
        while (std::getline(tagStream, tagToken, ',')) {
            c.addTag(tagToken);
        }
    }
    
    return c;
}

// Validation
bool Case::isValid() const {
    return validateTitle(title) && validateDescription(description) && id >= 0;
}

bool Case::validateTitle(const std::string& title) {
    return !title.empty() && title.length() <= 100;
}

bool Case::validateDescription(const std::string& description) {
    return !description.empty() && description.length() <= 1000;
}

// CaseUtils implementation
std::string CaseUtils::statusToString(CaseStatus status) {
    switch (status) {
        case CaseStatus::OPEN: return "Open";
        case CaseStatus::IN_PROGRESS: return "In Progress";
        case CaseStatus::SOLVED: return "Solved";
        case CaseStatus::COLD: return "Cold Case";
        case CaseStatus::UNSOLVED: return "Unsolved";
        default: return "Unknown";
    }
}

CaseStatus CaseUtils::stringToStatus(const std::string& statusStr) {
    if (statusStr == "Open") return CaseStatus::OPEN;
    if (statusStr == "In Progress") return CaseStatus::IN_PROGRESS;
    if (statusStr == "Solved") return CaseStatus::SOLVED;
    if (statusStr == "Cold Case") return CaseStatus::COLD;
    if (statusStr == "Unsolved") return CaseStatus::UNSOLVED;
    return CaseStatus::OPEN;
}

std::string CaseUtils::priorityToString(CasePriority priority) {
    switch (priority) {
        case CasePriority::LOW: return "Low";
        case CasePriority::MEDIUM: return "Medium";
        case CasePriority::HIGH: return "High";
        case CasePriority::URGENT: return "Urgent";
        default: return "Medium";
    }
}

CasePriority CaseUtils::stringToPriority(const std::string& priorityStr) {
    if (priorityStr == "Low") return CasePriority::LOW;
    if (priorityStr == "Medium") return CasePriority::MEDIUM;
    if (priorityStr == "High") return CasePriority::HIGH;
    if (priorityStr == "Urgent") return CasePriority::URGENT;
    return CasePriority::MEDIUM;
}

std::string CaseUtils::generateCaseId(int sequence) {
    std::stringstream ss;
    ss << "CASE-" << std::setw(6) << std::setfill('0') << sequence;
    return ss.str();
}

bool CaseUtils::isCaseTitleUnique(const std::string& title, const std::vector<Case>& cases) {
    return std::none_of(cases.begin(), cases.end(), 
                       [&](const Case& c) { return c.getTitle() == title; });
}
std::ostream& operator<<(std::ostream& os, const Case& c) {
    os << "Case{ID:" << c.id << ", Title:\"" << c.title << "\"}";
    return os;
}
/*
// Main function to test Case class
int main() {
    std::cout << "=== Testing Case Management System ===\n\n";
    
    // Test case creation
    std::cout << "Creating new cases...\n";
    Case case1(1, "The Mysterious Disappearance", 
               "A prominent businessman vanished without a trace from his office.");
    Case case2(2, "Art Gallery Heist", 
               "Priceless paintings stolen from the city museum during a blackout.",
               "City Art Museum", CasePriority::HIGH);
    
    // Test setters
    case1.setLocation("Downtown Office Building");
    case1.setPriority(CasePriority::URGENT);
    case1.setStatus(CaseStatus::IN_PROGRESS);
    
    // Add suspects and evidence
    case1.addSuspect(101);
    case1.addSuspect(102);
    case1.addEvidence("Security footage");
    case1.addEvidence("Fingerprints");
    case1.addTag("Disappearance");
    case1.addTag("Business");
    
    case2.addSuspect(201);
    case2.addEvidence("Broken lock");
    case2.addEvidence("Paint chips");
    case2.addTag("Art");
    case2.addTag("Heist");
    
    // Display cases
    std::cout << "\n=== Case Summaries ===\n";
    case1.displaySummary();
    std::cout << "\n";
    case2.displaySummary();
    
    std::cout << "\n=== Detailed Case View ===\n";
    case1.displayDetailed();
    
    // Test utility methods
    std::cout << "\n=== Utility Methods ===\n";
    std::cout << "Case 1 solved: " << (case1.isSolved() ? "Yes" : "No") << "\n";
    std::cout << "Case 1 involves suspect 101: " << (case1.involvesSuspect(101) ? "Yes" : "No") << "\n";
    std::cout << "Case 1 has evidence 'Fingerprints': " << (case1.hasEvidence("Fingerprints") ? "Yes" : "No") << "\n";
    std::cout << "Days since case 1 creation: " << case1.getDaysSinceCreation() << "\n";
    
    // Test serialization
    std::cout << "\n=== Serialization Test ===\n";
    std::string serialized = case1.serialize();
    std::cout << "Serialized data: " << serialized << "\n";
    
    Case deserializedCase = Case::deserialize(serialized);
    std::cout << "Deserialized case title: " << deserializedCase.getTitle() << "\n";
    
    // Test validation
    std::cout << "\n=== Validation Tests ===\n";
    std::cout << "Valid title: " << (Case::validateTitle("Valid Title") ? "Yes" : "No") << "\n";
    std::cout << "Empty title: " << (Case::validateTitle("") ? "Yes" : "No") << "\n";
    std::cout << "Case 1 valid: " << (case1.isValid() ? "Yes" : "No") << "\n";
    
    // Test CaseUtils
    std::cout << "\n=== CaseUtils Tests ===\n";
    std::cout << "Status string: " << CaseUtils::statusToString(CaseStatus::SOLVED) << "\n";
    std::cout << "Priority string: " << CaseUtils::priorityToString(CasePriority::URGENT) << "\n";
    std::cout << "Generated case ID: " << CaseUtils::generateCaseId(42) << "\n";
    
    // Test case modifications
    std::cout << "\n=== Modification Tests ===\n";
    std::cout << "Before modification - Modified: " << case1.getModificationDate() << "\n";
    case1.setNotes("New evidence suggests foul play");
    std::cout << "After modification - Modified: " << case1.getModificationDate() << "\n";
    
    // Test removal operations
    std::cout << "\n=== Removal Tests ===\n";
    std::cout << "Before removal - Suspects: " << case1.getSuspects().size() << "\n";
    case1.removeSuspect(101);
    std::cout << "After removal - Suspects: " << case1.getSuspects().size() << "\n";
    
    return 0;
}
    */