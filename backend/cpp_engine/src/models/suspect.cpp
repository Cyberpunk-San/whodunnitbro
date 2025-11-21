#include "suspect.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
// Constructor
Suspect::Suspect() : id(0), name(""), story(""), background(""), motive(""), alibi(""),
                     alibiStrength(AlibiStrength::NONE), status(SuspectStatus::UNINVESTIGATED),
                     age(0), occupation("Unknown"), lastKnownLocation("Unknown"),
                     suspicionLevel(0.0) {
    dateAdded = std::chrono::system_clock::now();
    lastModified = dateAdded;
}

Suspect::Suspect(const std::string& name, const std::string& story)
    : id(0), name(name), story(story), background(""), motive(""), alibi(""),
      alibiStrength(AlibiStrength::NONE), status(SuspectStatus::UNINVESTIGATED),
      age(0), occupation("Unknown"), lastKnownLocation("Unknown"), suspicionLevel(0.0) {
    dateAdded = std::chrono::system_clock::now();
    lastModified = dateAdded;
}

Suspect::Suspect(int id, const std::string& name, const std::string& story)
    : id(id), name(name), story(story), background(""), motive(""), alibi(""),
      alibiStrength(AlibiStrength::NONE), status(SuspectStatus::UNINVESTIGATED),
      age(0), occupation("Unknown"), lastKnownLocation("Unknown"), suspicionLevel(0.0) {
    dateAdded = std::chrono::system_clock::now();
    lastModified = dateAdded;
}

Suspect::Suspect(int id, const std::string& name, const std::string& story,
                 const std::string& background, int age, const std::string& occupation)
    : id(id), name(name), story(story), background(background), motive(""), alibi(""),
      alibiStrength(AlibiStrength::NONE), status(SuspectStatus::UNINVESTIGATED),
      age(age), occupation(occupation), lastKnownLocation("Unknown"), suspicionLevel(0.0) {
    dateAdded = std::chrono::system_clock::now();
    lastModified = dateAdded;
}

// Getters
int Suspect::getId() const { return id; }

std::string Suspect::getName() const { return name; }

std::string Suspect::getStory() const { return story; }

std::string Suspect::getBackground() const { return background; }

std::string Suspect::getMotive() const { return motive; }

std::string Suspect::getAlibi() const { return alibi; }

AlibiStrength Suspect::getAlibiStrength() const { return alibiStrength; }

std::string Suspect::getAlibiStrengthString() const {
    return SuspectUtils::alibiStrengthToString(alibiStrength);
}

SuspectStatus Suspect::getStatus() const { return status; }

std::string Suspect::getStatusString() const {
    return SuspectUtils::statusToString(status);
}

int Suspect::getAge() const { return age; }

std::string Suspect::getOccupation() const { return occupation; }

std::string Suspect::getLastKnownLocation() const { return lastKnownLocation; }

std::vector<int> Suspect::getCases() const { return caseIds; }

std::vector<std::string> Suspect::getPhysicalDescription() const { return physicalDescription; }

std::vector<std::string> Suspect::getKnownAssociates() const { return knownAssociates; }

std::vector<std::string> Suspect::getEvidenceAgainst() const { return evidenceAgainst; }

std::vector<std::string> Suspect::getEvidenceFor() const { return evidenceFor; }

double Suspect::getSuspicionLevel() const { return suspicionLevel; }

std::string Suspect::getSuspicionLevelString() const {
    if (suspicionLevel < 25) return "Low";
    if (suspicionLevel < 50) return "Medium";
    if (suspicionLevel < 75) return "High";
    return "Very High";
}

std::string Suspect::getAddedDate() const {
    auto time = std::chrono::system_clock::to_time_t(dateAdded);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Suspect::getLastModifiedDate() const {
    auto time = std::chrono::system_clock::to_time_t(lastModified);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Setters
void Suspect::setName(const std::string& newName) { 
    name = newName; 
    updateModificationDate();
}

void Suspect::setStory(const std::string& newStory) { 
    story = newStory; 
    updateModificationDate();
}

void Suspect::setBackground(const std::string& newBackground) { 
    background = newBackground; 
    updateModificationDate();
}

void Suspect::setMotive(const std::string& newMotive) { 
    motive = newMotive; 
    updateModificationDate();
}

void Suspect::setAlibi(const std::string& newAlibi) { 
    alibi = newAlibi; 
    updateModificationDate();
}

void Suspect::setAlibiStrength(AlibiStrength strength) { 
    alibiStrength = strength; 
    updateModificationDate();
    updateSuspicionLevel();
}

void Suspect::setStatus(SuspectStatus newStatus) { 
    status = newStatus; 
    updateModificationDate();
    updateSuspicionLevel();
}

void Suspect::setAge(int newAge) { 
    age = newAge; 
    updateModificationDate();
}

void Suspect::setOccupation(const std::string& newOccupation) { 
    occupation = newOccupation; 
    updateModificationDate();
}

void Suspect::setLastKnownLocation(const std::string& newLocation) { 
    lastKnownLocation = newLocation; 
    updateModificationDate();
}

void Suspect::setSuspicionLevel(double level) { 
    suspicionLevel = std::max(0.0, std::min(100.0, level)); 
    updateModificationDate();
}

// Management methods
void Suspect::addCase(int caseId) {
    if (std::find(caseIds.begin(), caseIds.end(), caseId) == caseIds.end()) {
        caseIds.push_back(caseId);
        updateModificationDate();
        updateSuspicionLevel();
    }
}

void Suspect::removeCase(int caseId) {
    auto it = std::find(caseIds.begin(), caseIds.end(), caseId);
    if (it != caseIds.end()) {
        caseIds.erase(it);
        updateModificationDate();
        updateSuspicionLevel();
    }
}

void Suspect::addPhysicalDescription(const std::string& description) {
    if (std::find(physicalDescription.begin(), physicalDescription.end(), description) == physicalDescription.end()) {
        physicalDescription.push_back(description);
        updateModificationDate();
    }
}

void Suspect::removePhysicalDescription(const std::string& description) {
    auto it = std::find(physicalDescription.begin(), physicalDescription.end(), description);
    if (it != physicalDescription.end()) {
        physicalDescription.erase(it);
        updateModificationDate();
    }
}

void Suspect::addKnownAssociate(const std::string& associate) {
    if (std::find(knownAssociates.begin(), knownAssociates.end(), associate) == knownAssociates.end()) {
        knownAssociates.push_back(associate);
        updateModificationDate();
    }
}

void Suspect::removeKnownAssociate(const std::string& associate) {
    auto it = std::find(knownAssociates.begin(), knownAssociates.end(), associate);
    if (it != knownAssociates.end()) {
        knownAssociates.erase(it);
        updateModificationDate();
    }
}

void Suspect::addEvidenceAgainst(const std::string& evidence) {
    if (std::find(evidenceAgainst.begin(), evidenceAgainst.end(), evidence) == evidenceAgainst.end()) {
        evidenceAgainst.push_back(evidence);
        updateModificationDate();
        updateSuspicionLevel();
    }
}

void Suspect::removeEvidenceAgainst(const std::string& evidence) {
    auto it = std::find(evidenceAgainst.begin(), evidenceAgainst.end(), evidence);
    if (it != evidenceAgainst.end()) {
        evidenceAgainst.erase(it);
        updateModificationDate();
        updateSuspicionLevel();
    }
}

void Suspect::addEvidenceFor(const std::string& evidence) {
    if (std::find(evidenceFor.begin(), evidenceFor.end(), evidence) == evidenceFor.end()) {
        evidenceFor.push_back(evidence);
        updateModificationDate();
        updateSuspicionLevel();
    }
}

void Suspect::removeEvidenceFor(const std::string& evidence) {
    auto it = std::find(evidenceFor.begin(), evidenceFor.end(), evidence);
    if (it != evidenceFor.end()) {
        evidenceFor.erase(it);
        updateModificationDate();
        updateSuspicionLevel();
    }
}

void Suspect::clearPhysicalDescription() {
    physicalDescription.clear();
    updateModificationDate();
}

void Suspect::clearKnownAssociates() {
    knownAssociates.clear();
    updateModificationDate();
}

void Suspect::clearEvidence() {
    evidenceAgainst.clear();
    evidenceFor.clear();
    updateModificationDate();
    updateSuspicionLevel();
}

// Utility methods
bool Suspect::isPrimeSuspect() const {
    return status == SuspectStatus::PRIME_SUSPECT;
}

bool Suspect::isCleared() const {
    return status == SuspectStatus::CLEARED || status == SuspectStatus::ACQUITTED;
}

bool Suspect::hasStrongAlibi() const {
    return alibiStrength == AlibiStrength::STRONG || alibiStrength == AlibiStrength::CONFIRMED;
}

bool Suspect::isInvolvedInCase(int caseId) const {
    return std::find(caseIds.begin(), caseIds.end(), caseId) != caseIds.end();
}

bool Suspect::hasEvidence(const std::string& evidence) const {
    return std::find(evidenceAgainst.begin(), evidenceAgainst.end(), evidence) != evidenceAgainst.end() ||
           std::find(evidenceFor.begin(), evidenceFor.end(), evidence) != evidenceFor.end();
}

bool Suspect::hasKnownAssociate(const std::string& associate) const {
    return std::find(knownAssociates.begin(), knownAssociates.end(), associate) != knownAssociates.end();
}

bool Suspect::hasMotive() const {
    return !motive.empty();
}

bool Suspect::hasAlibi() const {
    return !alibi.empty();
}

int Suspect::getEvidenceCount() const {
    return evidenceAgainst.size() + evidenceFor.size();
}

int Suspect::getCaseInvolvementCount() const {
    return caseIds.size();
}

void Suspect::updateSuspicionLevel() {
    double score = calculateSuspicionScore();
    setSuspicionLevel(score);
}


// Analysis methods
double Suspect::calculateSuspicionScore() const {
    double score = 0.0;
    
    // Base suspicion from status
    switch (status) {
        case SuspectStatus::PRIME_SUSPECT: score += 80; break;
        case SuspectStatus::UNDER_INVESTIGATION: score += 60; break;
        case SuspectStatus::UNINVESTIGATED: score += 30; break;
        case SuspectStatus::CLEARED: score += 10; break;
        case SuspectStatus::ACQUITTED: score += 5; break;
        case SuspectStatus::CONVICTED: score += 95; break;
    }
    
    // Adjust based on alibi strength
    switch (alibiStrength) {
        case AlibiStrength::NONE: score += 20; break;
        case AlibiStrength::WEAK: score += 10; break;
        case AlibiStrength::MODERATE: score += 0; break;
        case AlibiStrength::STRONG: score -= 30; break;
        case AlibiStrength::CONFIRMED: score -= 50; break;
    }
    
    // Evidence against increases suspicion
    score += evidenceAgainst.size() * 5;
    
    // Evidence for decreases suspicion
    score -= evidenceFor.size() * 5;
    
    // Multiple case involvement increases suspicion
    score += caseIds.size() * 3;
    
    // Having a motive increases suspicion
    if (!motive.empty()) score += 15;
    
    return std::max(0.0, std::min(100.0, score));
}

// Display methods
void Suspect::display() const {
    std::cout << "Suspect: " << name << " (ID: " << id << ")\n";
    std::cout << "Status: " << getStatusString() << " | Suspicion: " << getSuspicionLevelString() << "\n";
    std::cout << "Story: " << story << "\n";
}

void Suspect::displaySummary() const {
    std::cout << "🕵️ SUSPECT: " << name << "\n";
    std::cout << "   Status: " << getStatusString() << " | Age: " << age << " | Occupation: " << occupation << "\n";
    std::cout << "   Suspicion Level: " << getSuspicionLevelString() << " (" << suspicionLevel << "%)\n";
    std::cout << "   Cases: " << caseIds.size() << " | Evidence: " << getEvidenceCount() << "\n";
}

void Suspect::displayDetailed() const {
    std::cout << "========================================\n";
    std::cout << "🕵️ SUSPECT DETAILS\n";
    std::cout << "========================================\n";
    std::cout << "ID: " << id << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Status: " << getStatusString() << "\n";
    std::cout << "Suspicion Level: " << getSuspicionLevelString() << " (" << suspicionLevel << "%)\n";
    std::cout << "Age: " << age << " | Occupation: " << occupation << "\n";
    std::cout << "Last Known Location: " << lastKnownLocation << "\n";
    std::cout << "Background: " << background << "\n";
    std::cout << "Story: " << story << "\n";
    
    if (!motive.empty()) {
        std::cout << "Motive: " << motive << "\n";
    }
    
    if (!alibi.empty()) {
        std::cout << "Alibi: " << alibi << " [" << getAlibiStrengthString() << "]\n";
    }
    
    std::cout << "Cases Involved (" << caseIds.size() << "): ";
    for (int caseId : caseIds) {
        std::cout << caseId << " ";
    }
    std::cout << "\n";
    
    std::cout << "Physical Description (" << physicalDescription.size() << "):\n";
    for (const auto& desc : physicalDescription) {
        std::cout << "  • " << desc << "\n";
    }
    
    std::cout << "Known Associates (" << knownAssociates.size() << "):\n";
    for (const auto& associate : knownAssociates) {
        std::cout << "  • " << associate << "\n";
    }
    
    std::cout << "Evidence Against (" << evidenceAgainst.size() << "):\n";
    for (const auto& evidence : evidenceAgainst) {
        std::cout << "  • " << evidence << "\n";
    }
    
    std::cout << "Evidence For (" << evidenceFor.size() << "):\n";
    for (const auto& evidence : evidenceFor) {
        std::cout << "  • " << evidence << "\n";
    }
    
    std::cout << "Added: " << getAddedDate() << " | Modified: " << getLastModifiedDate() << "\n";
}

std::string Suspect::to_string() const {
    std::stringstream ss;
    ss << "Suspect{Name:\"" << name << "\", Status:" << getStatusString() 
       << ", Suspicion:" << suspicionLevel << "%}";
    return ss.str();
}

// Serialization
std::string Suspect::serialize() const {
    std::stringstream ss;
    ss << id << "|" << name << "|" << story << "|" << background << "|"
       << motive << "|" << alibi << "|" << static_cast<int>(alibiStrength) << "|"
       << static_cast<int>(status) << "|" << age << "|" << occupation << "|"
       << lastKnownLocation << "|" << suspicionLevel;
    
    // Serialize case IDs
    ss << "|";
    for (size_t i = 0; i < caseIds.size(); ++i) {
        if (i > 0) ss << ",";
        ss << caseIds[i];
    }
    
    // Serialize physical description
    ss << "|";
    for (size_t i = 0; i < physicalDescription.size(); ++i) {
        if (i > 0) ss << ",";
        ss << physicalDescription[i];
    }
    
    // Serialize known associates
    ss << "|";
    for (size_t i = 0; i < knownAssociates.size(); ++i) {
        if (i > 0) ss << ",";
        ss << knownAssociates[i];
    }
    
    // Serialize evidence against
    ss << "|";
    for (size_t i = 0; i < evidenceAgainst.size(); ++i) {
        if (i > 0) ss << ",";
        ss << evidenceAgainst[i];
    }
    
    // Serialize evidence for
    ss << "|";
    for (size_t i = 0; i < evidenceFor.size(); ++i) {
        if (i > 0) ss << ",";
        ss << evidenceFor[i];
    }
    
    // Serialize dates
    ss << "|" << std::chrono::system_clock::to_time_t(dateAdded);
    ss << "|" << std::chrono::system_clock::to_time_t(lastModified);
    
    return ss.str();
}

Suspect Suspect::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() < 12) {
        throw std::invalid_argument("Invalid suspect data format");
    }
    
    Suspect s(std::stoi(tokens[0]), tokens[1], tokens[2]);
    s.setBackground(tokens[3]);
    s.setMotive(tokens[4]);
    s.setAlibi(tokens[5]);
    s.setAlibiStrength(static_cast<AlibiStrength>(std::stoi(tokens[6])));
    s.setStatus(static_cast<SuspectStatus>(std::stoi(tokens[7])));
    s.setAge(std::stoi(tokens[8]));
    s.setOccupation(tokens[9]);
    s.setLastKnownLocation(tokens[10]);
    s.setSuspicionLevel(std::stod(tokens[11]));
    
    // Deserialize case IDs
    if (tokens.size() > 12 && !tokens[12].empty()) {
        std::stringstream caseStream(tokens[12]);
        std::string caseToken;
        while (std::getline(caseStream, caseToken, ',')) {
            s.addCase(std::stoi(caseToken));
        }
    }
    
    // Deserialize physical description
    if (tokens.size() > 13 && !tokens[13].empty()) {
        std::stringstream descStream(tokens[13]);
        std::string descToken;
        while (std::getline(descStream, descToken, ',')) {
            s.addPhysicalDescription(descToken);
        }
    }
    
    // Deserialize known associates
    if (tokens.size() > 14 && !tokens[14].empty()) {
        std::stringstream associateStream(tokens[14]);
        std::string associateToken;
        while (std::getline(associateStream, associateToken, ',')) {
            s.addKnownAssociate(associateToken);
        }
    }
    
    // Deserialize evidence against
    if (tokens.size() > 15 && !tokens[15].empty()) {
        std::stringstream evidenceStream(tokens[15]);
        std::string evidenceToken;
        while (std::getline(evidenceStream, evidenceToken, ',')) {
            s.addEvidenceAgainst(evidenceToken);
        }
    }
    
    // Deserialize evidence for
    if (tokens.size() > 16 && !tokens[16].empty()) {
        std::stringstream evidenceStream(tokens[16]);
        std::string evidenceToken;
        while (std::getline(evidenceStream, evidenceToken, ',')) {
            s.addEvidenceFor(evidenceToken);
        }
    }
    
    return s;
}

// Validation
bool Suspect::isValid() const {
    return validateName(name) && validateAge(age) && id >= 0;
}

bool Suspect::validateName(const std::string& name) {
    return !name.empty() && name.length() <= 50;
}

bool Suspect::validateAge(int age) {
    return age >= 0 && age <= 150;
}

// SuspectUtils implementation
std::string SuspectUtils::statusToString(SuspectStatus status) {
    switch (status) {
        case SuspectStatus::UNINVESTIGATED: return "Uninvestigated";
        case SuspectStatus::UNDER_INVESTIGATION: return "Under Investigation";
        case SuspectStatus::CLEARED: return "Cleared";
        case SuspectStatus::PRIME_SUSPECT: return "Prime Suspect";
        case SuspectStatus::CONVICTED: return "Convicted";
        case SuspectStatus::ACQUITTED: return "Acquitted";
        default: return "Unknown";
    }
}

SuspectStatus SuspectUtils::stringToStatus(const std::string& statusStr) {
    if (statusStr == "Uninvestigated") return SuspectStatus::UNINVESTIGATED;
    if (statusStr == "Under Investigation") return SuspectStatus::UNDER_INVESTIGATION;
    if (statusStr == "Cleared") return SuspectStatus::CLEARED;
    if (statusStr == "Prime Suspect") return SuspectStatus::PRIME_SUSPECT;
    if (statusStr == "Convicted") return SuspectStatus::CONVICTED;
    if (statusStr == "Acquitted") return SuspectStatus::ACQUITTED;
    return SuspectStatus::UNINVESTIGATED;
}

std::string SuspectUtils::alibiStrengthToString(AlibiStrength strength) {
    switch (strength) {
        case AlibiStrength::NONE: return "None";
        case AlibiStrength::WEAK: return "Weak";
        case AlibiStrength::MODERATE: return "Moderate";
        case AlibiStrength::STRONG: return "Strong";
        case AlibiStrength::CONFIRMED: return "Confirmed";
        default: return "None";
    }
}

AlibiStrength SuspectUtils::stringToAlibiStrength(const std::string& strengthStr) {
    if (strengthStr == "None") return AlibiStrength::NONE;
    if (strengthStr == "Weak") return AlibiStrength::WEAK;
    if (strengthStr == "Moderate") return AlibiStrength::MODERATE;
    if (strengthStr == "Strong") return AlibiStrength::STRONG;
    if (strengthStr == "Confirmed") return AlibiStrength::CONFIRMED;
    return AlibiStrength::NONE;
}

std::string SuspectUtils::generateSuspectId(int sequence) {
    std::stringstream ss;
    ss << "SUSP-" << std::setw(6) << std::setfill('0') << sequence;
    return ss.str();
}

bool SuspectUtils::isSuspectNameUnique(const std::string& name, const std::vector<Suspect>& suspects) {
    return std::none_of(suspects.begin(), suspects.end(),
                       [&](const Suspect& s) { return s.getName() == name; });
}
std::ostream& operator<<(std::ostream& os, const Suspect& s) {
    os << "Suspect{ID:" << s.id << ", Name:\"" << s.name << "\", Suspicion:" << s.suspicionLevel << "%}";
    return os;
}
void Suspect::updateModificationDate() {
    lastModified = std::chrono::system_clock::now();
}
/*
void demonstrateSuspectClass() {
    std::cout << "=== SUSPECT MANAGEMENT SYSTEM DEMO ===\n\n";
    
    // Create suspects
    Suspect primeSuspect(1, "Professor Moriarty", "Brilliant criminal mastermind", 
                        "Mathematics professor turned criminal organizer", 45, "Professor");
    
    Suspect accomplice(2, "Sebastian Moran", "Former military officer turned hitman",
                      "Disgraced army colonel with sniper expertise", 38, "Former Military");
    
    Suspect witness(3, "Irene Adler", "Opera singer involved with criminal elements",
                   "American opera singer with connections to European royalty", 32, "Opera Singer");
    
    // Set up prime suspect
    primeSuspect.setMotive("Revenge and power acquisition");
    primeSuspect.setAlibi("Working in his study all evening");
    primeSuspect.setAlibiStrength(AlibiStrength::WEAK);
    primeSuspect.setStatus(SuspectStatus::PRIME_SUSPECT);
    primeSuspect.setLastKnownLocation("London");
    primeSuspect.addCase(101);
    primeSuspect.addCase(102);
    primeSuspect.addEvidenceAgainst("Fingerprints at crime scene");
    primeSuspect.addEvidenceAgainst("Financial transactions linked to crime");
    primeSuspect.addKnownAssociate("Sebastian Moran");
    primeSuspect.addPhysicalDescription("Tall, thin, intellectual appearance");
    primeSuspect.addPhysicalDescription("Pale complexion, sharp features");
    
    // Set up accomplice
    accomplice.setMotive("Financial gain");
    accomplice.setAlibi("At the gambling club with multiple witnesses");
    accomplice.setAlibiStrength(AlibiStrength::MODERATE);
    accomplice.setStatus(SuspectStatus::UNDER_INVESTIGATION);
    accomplice.setLastKnownLocation("London");
    accomplice.addCase(101);
    accomplice.addEvidenceAgainst("Ballistic match to weapon used");
    accomplice.addEvidenceAgainst("Seen near crime scene");
    accomplice.addKnownAssociate("Professor Moriarty");
    accomplice.addPhysicalDescription("Military bearing, athletic build");
    accomplice.addPhysicalDescription("Scar on left cheek");
    
    // Set up witness who might be involved
    witness.setAlibi("Performing at Royal Opera House");
    witness.setAlibiStrength(AlibiStrength::CONFIRMED);
    witness.setStatus(SuspectStatus::CLEARED);
    witness.setLastKnownLocation("London");
    witness.addCase(101);
    witness.addEvidenceFor("Multiple eyewitness confirmation of alibi");
    witness.addPhysicalDescription("Beautiful, charismatic performer");
    witness.addPhysicalDescription("Dark hair, striking blue eyes");
    
    // Display suspects
    std::cout << "SUSPECT SUMMARIES:\n";
    primeSuspect.displaySummary();
    accomplice.displaySummary();
    witness.displaySummary();
    
    std::cout << "\nDETAILED SUSPECT VIEW:\n";
    primeSuspect.displayDetailed();
    
    std::cout << "\nBASIC SUSPECT VIEW:\n";
    accomplice.display();
    
    // Test utility methods
    std::cout << "\nUTILITY METHOD TESTS:\n";
    std::cout << "Is Moriarty a prime suspect? " 
              << (primeSuspect.isPrimeSuspect() ? "Yes" : "No") << "\n";
    std::cout << "Is Irene Adler cleared? " 
              << (witness.isCleared() ? "Yes" : "No") << "\n";
    std::cout << "Does Moriarty have a strong alibi? " 
              << (primeSuspect.hasStrongAlibi() ? "Yes" : "No") << "\n";
    std::cout << "Is Moran involved in case 101? " 
              << (accomplice.isInvolvedInCase(101) ? "Yes" : "No") << "\n";
    std::cout << "Does Moriarty know Moran? " 
              << (primeSuspect.hasKnownAssociate("Sebastian Moran") ? "Yes" : "No") << "\n";
    
    // Test serialization
    std::cout << "\nSERIALIZATION TEST:\n";
    std::string serialized = primeSuspect.serialize();
    std::cout << "Serialized data: " << serialized << "\n";
    
    Suspect deserializedSuspect = Suspect::deserialize(serialized);
    std::cout << "Deserialized suspect: " << deserializedSuspect.to_string() << "\n";
    
    // Test SuspectUtils
    std::cout << "\nSUSPECT UTILS TESTS:\n";
    std::cout << "Status string for PRIME_SUSPECT: " << SuspectUtils::statusToString(SuspectStatus::PRIME_SUSPECT) << "\n";
    std::cout << "Generated suspect ID: " << SuspectUtils::generateSuspectId(123) << "\n";
    
    std::vector<Suspect> suspects = {primeSuspect, accomplice, witness};
    std::cout << "Is 'Professor Moriarty' unique? " 
              << (SuspectUtils::isSuspectNameUnique("Professor Moriarty", suspects) ? "Yes" : "No") << "\n";
    std::cout << "Is 'New Suspect' unique? " 
              << (SuspectUtils::isSuspectNameUnique("New Suspect", suspects) ? "Yes" : "No") << "\n";
    
    // Test validation
    std::cout << "\nVALIDATION TESTS:\n";
    std::cout << "Is prime suspect valid? " << (primeSuspect.isValid() ? "Yes" : "No") << "\n";
    
    Suspect invalidSuspect(-1, "", "");
    std::cout << "Is invalid suspect valid? " << (invalidSuspect.isValid() ? "Yes" : "No") << "\n";
    
    std::cout << "Is valid name? " << (Suspect::validateName("Valid Name") ? "Yes" : "No") << "\n";
    std::cout << "Is empty name valid? " << (Suspect::validateName("") ? "Yes" : "No") << "\n";
    std::cout << "Is valid age? " << (Suspect::validateAge(30) ? "Yes" : "No") << "\n";
    std::cout << "Is invalid age valid? " << (Suspect::validateAge(-5) ? "Yes" : "No") << "\n";
}

int main() {
    try {
        demonstrateSuspectClass();
        std::cout << "\n=== DEMO COMPLETED SUCCESSFULLY ===\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
    */