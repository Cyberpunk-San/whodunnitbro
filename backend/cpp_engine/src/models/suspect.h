#ifndef SUSPECT_H
#define SUSPECT_H

#include <string>
#include <vector>
#include <chrono>

enum class SuspectStatus {
    UNINVESTIGATED,
    UNDER_INVESTIGATION,
    CLEARED,
    PRIME_SUSPECT,
    CONVICTED,
    ACQUITTED
};

enum class AlibiStrength {
    NONE,
    WEAK,
    MODERATE,
    STRONG,
    CONFIRMED
};

class Suspect {
private:
    int id;
    std::string name;
    std::string story;
    std::string background;
    std::string motive;
    std::string alibi;
    AlibiStrength alibiStrength;
    SuspectStatus status;
    int age;
    std::string occupation;
    std::string lastKnownLocation;
    double suspicionLevel;
    
    std::vector<int> caseIds;
    std::vector<std::string> physicalDescription;
    std::vector<std::string> knownAssociates;
    std::vector<std::string> evidenceAgainst;
    std::vector<std::string> evidenceFor;
    
    std::chrono::system_clock::time_point dateAdded;
    std::chrono::system_clock::time_point lastModified;
    


public:
    void updateModificationDate();

    void updateSuspicionLevel();
    double calculateSuspicionScore() const;
    // Default constructor
    Suspect();
    // Constructors
    Suspect(const std::string& name, const std::string& story);
    Suspect(int id, const std::string& name, const std::string& story);
    Suspect(int id, const std::string& name, const std::string& story,
            const std::string& background, int age, const std::string& occupation);
    
    // Getters
    int getId() const;
    std::string getName() const;
    std::string getStory() const;
    std::string getBackground() const;
    std::string getMotive() const;
    std::string getAlibi() const;
    AlibiStrength getAlibiStrength() const;
    std::string getAlibiStrengthString() const;
    SuspectStatus getStatus() const;
    std::string getStatusString() const;
    int getAge() const;
    std::string getOccupation() const;
    std::string getLastKnownLocation() const;
    std::vector<int> getCases() const;
    std::vector<std::string> getPhysicalDescription() const;
    std::vector<std::string> getKnownAssociates() const;
    std::vector<std::string> getEvidenceAgainst() const;
    std::vector<std::string> getEvidenceFor() const;
    double getSuspicionLevel() const;
    std::string getSuspicionLevelString() const;
    std::string getAddedDate() const;
    std::string getLastModifiedDate() const;
    
    // Setters
    void setName(const std::string& newName);
    void setStory(const std::string& newStory);
    void setBackground(const std::string& newBackground);
    void setMotive(const std::string& newMotive);
    void setAlibi(const std::string& newAlibi);
    void setAlibiStrength(AlibiStrength strength);
    void setStatus(SuspectStatus newStatus);
    void setAge(int newAge);
    void setOccupation(const std::string& newOccupation);
    void setLastKnownLocation(const std::string& newLocation);
    void setSuspicionLevel(double level);
    
    // Management methods
    void addCase(int caseId);
    void removeCase(int caseId);
    void addPhysicalDescription(const std::string& description);
    void removePhysicalDescription(const std::string& description);
    void addKnownAssociate(const std::string& associate);
    void removeKnownAssociate(const std::string& associate);
    void addEvidenceAgainst(const std::string& evidence);
    void removeEvidenceAgainst(const std::string& evidence);
    void addEvidenceFor(const std::string& evidence);
    void removeEvidenceFor(const std::string& evidence);
    void clearPhysicalDescription();
    void clearKnownAssociates();
    void clearEvidence();
    
    // Utility methods
    bool isPrimeSuspect() const;
    bool isCleared() const;
    bool hasStrongAlibi() const;
    bool isInvolvedInCase(int caseId) const;
    bool hasEvidence(const std::string& evidence) const;
    bool hasKnownAssociate(const std::string& associate) const;
    bool hasMotive() const;
    bool hasAlibi() const;
    int getEvidenceCount() const;
    int getCaseInvolvementCount() const;
    
    // Display methods
    void display() const;
    void displaySummary() const;
    void displayDetailed() const;
    std::string to_string() const;
    
    // Serialization
    std::string serialize() const;
    static Suspect deserialize(const std::string& data);
    
    // Validation
    bool isValid() const;
    static bool validateName(const std::string& name);
    static bool validateAge(int age);
    bool operator==(const Suspect& other) const {
        return id == other.id && name == other.name;
    }
    
    bool operator<(const Suspect& other) const {
        return name < other.name; // Or use suspicion level for comparison
    }
    
    bool operator>(const Suspect& other) const {
        return name > other.name; // Or use suspicion level for comparison
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Suspect& s);
};

class SuspectUtils {
public:
    static std::string statusToString(SuspectStatus status);
    static SuspectStatus stringToStatus(const std::string& statusStr);
    static std::string alibiStrengthToString(AlibiStrength strength);
    static AlibiStrength stringToAlibiStrength(const std::string& strengthStr);
    static std::string generateSuspectId(int sequence);
    static bool isSuspectNameUnique(const std::string& name, const std::vector<Suspect>& suspects);
};

#endif // SUSPECT_H