// character.cpp
#include "character.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>

// Constructors
Character::Character() : id(0), name(""), role(CharacterRole::OTHER), story("") {}

Character::Character(int id, const std::string& name, CharacterRole role, const std::string& story)
    : id(id), name(name), role(role), story(story) {}

// Getters
int Character::getId() const { return id; }
std::string Character::getName() const { return name; }
CharacterRole Character::getRole() const { return role; }
std::string Character::getRoleString() const { return CharacterUtils::roleToString(role); }
std::string Character::getStory() const { return story; }
std::vector<int> Character::getRelatedCases() const { return relatedCases; }
std::vector<std::string> Character::getKnownSuspects() const { return knownSuspects; }

// Setters
void Character::setName(const std::string& newName) { name = newName; }
void Character::setRole(CharacterRole newRole) { role = newRole; }
void Character::setStory(const std::string& newStory) { story = newStory; }

// Management methods
void Character::addCase(int caseId) {
    if (std::find(relatedCases.begin(), relatedCases.end(), caseId) == relatedCases.end()) {
        relatedCases.push_back(caseId);
    }
}
void Character::removeCase(int caseId) {
    auto it = std::find(relatedCases.begin(), relatedCases.end(), caseId);
    if (it != relatedCases.end()) relatedCases.erase(it);
}
void Character::addKnownSuspect(const std::string& suspectName) {
    if (std::find(knownSuspects.begin(), knownSuspects.end(), suspectName) == knownSuspects.end()) {
        knownSuspects.push_back(suspectName);
    }
}
void Character::removeKnownSuspect(const std::string& suspectName) {
    auto it = std::find(knownSuspects.begin(), knownSuspects.end(), suspectName);
    if (it != knownSuspects.end()) knownSuspects.erase(it);
}
void Character::clearRelatedCases() { relatedCases.clear(); }
void Character::clearKnownSuspects() { knownSuspects.clear(); }

// Utility methods
bool Character::isInvolvedInCase(int caseId) const {
    return std::find(relatedCases.begin(), relatedCases.end(), caseId) != relatedCases.end();
}
bool Character::knowsSuspect(const std::string& suspectName) const {
    return std::find(knownSuspects.begin(), knownSuspects.end(), suspectName) != knownSuspects.end();
}
int Character::getCaseInvolvementCount() const { return static_cast<int>(relatedCases.size()); }
int Character::getKnownSuspectsCount() const { return static_cast<int>(knownSuspects.size()); }

// Display methods
void Character::display() const {
    std::cout << "Character ID: " << id << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Role: " << getRoleString() << "\n";
    std::cout << "Story: " << story << "\n";
    std::cout << "Related Cases: ";
    for (auto c : relatedCases) std::cout << c << " ";
    std::cout << "\n";
    std::cout << "Known Suspects: ";
    for (auto s : knownSuspects) std::cout << s << " ";
    std::cout << "\n";
}

void Character::displaySummary() const {
    std::cout << "👤 CHARACTER: " << name << " (ID: " << id << ")\n";
    std::cout << "   Role: " << getRoleString() << " | Cases: " << relatedCases.size() << "\n";
    std::cout << "   Known Suspects: " << knownSuspects.size() << "\n";
}

void Character::displayDetailed() const {
    std::cout << "========================================\n";
    std::cout << "👤 CHARACTER DETAILS\n";
    std::cout << "========================================\n";
    std::cout << "ID: " << id << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Role: " << getRoleString() << "\n";
    std::cout << "Story: " << story << "\n";
    std::cout << "Related Cases (" << relatedCases.size() << "): ";
    for (int caseId : relatedCases) std::cout << caseId << " ";
    std::cout << "\n";
    std::cout << "Known Suspects (" << knownSuspects.size() << "):\n";
    for (const auto& suspect : knownSuspects) {
        std::cout << "  • " << suspect << "\n";
    }
}

// to_string
std::string Character::to_string() const {
    std::stringstream ss;
    ss << "Character{ID:" << id << ", Name:\"" << name << "\", Role:\"" << getRoleString() << "\"}";
    return ss.str();
}

// Serialization
std::string Character::serialize() const {
    std::stringstream ss;
    ss << id << "|" << name << "|" << static_cast<int>(role) << "|" << story;
    ss << "|";
    for (size_t i = 0; i < relatedCases.size(); ++i) {
        if (i) ss << ",";
        ss << relatedCases[i];
    }
    ss << "|";
    for (size_t i = 0; i < knownSuspects.size(); ++i) {
        if (i) ss << ",";
        ss << knownSuspects[i];
    }
    return ss.str();
}

Character Character::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, '|')) tokens.push_back(token);
    if (tokens.size() < 4) throw std::invalid_argument("Invalid character data format");
    Character ch(std::stoi(tokens[0]), tokens[1], static_cast<CharacterRole>(std::stoi(tokens[2])), tokens[3]);
    if (tokens.size() > 4 && !tokens[4].empty()) {
        std::stringstream caseStream(tokens[4]);
        std::string caseToken;
        while (std::getline(caseStream, caseToken, ',')) ch.addCase(std::stoi(caseToken));
    }
    if (tokens.size() > 5 && !tokens[5].empty()) {
        std::stringstream suspectStream(tokens[5]);
        std::string suspectToken;
        while (std::getline(suspectStream, suspectToken, ',')) ch.addKnownSuspect(suspectToken);
    }
    return ch;
}

// Validation
bool Character::isValid() const { return validateName(name) && id >= 0; }
bool Character::validateName(const std::string& name) { return !name.empty() && name.length() <= 50; }
bool Character::validateRole(CharacterRole role) {
    return role >= CharacterRole::WITNESS && role <= CharacterRole::OTHER;
}

// CharacterUtils - small helpers
std::string CharacterUtils::roleToString(CharacterRole role) {
    switch (role) {
        case CharacterRole::WITNESS: return "Witness";
        case CharacterRole::INFORMANT: return "Informant";
        case CharacterRole::VICTIM: return "Victim";
        case CharacterRole::OFFICER: return "Officer";
        case CharacterRole::DETECTIVE: return "Detective";
        case CharacterRole::EXPERT: return "Expert";
        default: return "Other";
    }
}
CharacterRole CharacterUtils::stringToRole(const std::string& roleStr) {
    if (roleStr == "Witness") return CharacterRole::WITNESS;
    if (roleStr == "Informant") return CharacterRole::INFORMANT;
    if (roleStr == "Victim") return CharacterRole::VICTIM;
    if (roleStr == "Officer") return CharacterRole::OFFICER;
    if (roleStr == "Detective") return CharacterRole::DETECTIVE;
    if (roleStr == "Expert") return CharacterRole::EXPERT;
    return CharacterRole::OTHER;
}
std::string CharacterUtils::reliabilityToString(Reliability reliability) {
    switch (reliability) {
        case Reliability::UNRELIABLE: return "Unreliable";
        case Reliability::SOMEWHAT_RELIABLE: return "Somewhat Reliable";
        case Reliability::RELIABLE: return "Reliable";
        case Reliability::HIGHLY_RELIABLE: return "Highly Reliable";
        default: return "Unknown";
    }
}
Reliability CharacterUtils::stringToReliability(const std::string& reliabilityStr) {
    if (reliabilityStr == "Unreliable") return Reliability::UNRELIABLE;
    if (reliabilityStr == "Somewhat Reliable") return Reliability::SOMEWHAT_RELIABLE;
    if (reliabilityStr == "Reliable") return Reliability::RELIABLE;
    if (reliabilityStr == "Highly Reliable") return Reliability::HIGHLY_RELIABLE;
    return Reliability::UNRELIABLE;
}
std::string CharacterUtils::generateCharacterId(int sequence) {
    std::stringstream ss;
    ss << "CHAR-" << std::setw(4) << std::setfill('0') << sequence;
    return ss.str();
}
bool CharacterUtils::isCharacterNameUnique(const std::string& name, const std::vector<Character>& characters) {
    return std::none_of(characters.begin(), characters.end(), [&](const Character& c){ return c.getName() == name; });
}

// operator<<
std::ostream& operator<<(std::ostream& os, const Character& ch) {
    os << "Character{ID:" << ch.id << ", Name:\"" << ch.name << "\", Role:\"" << ch.getRoleString() << "\"}";
    return os;
}
