#ifndef CASE_H
#define CASE_H

#include <string>
#include <vector>
#include <chrono>
#include <functional>

enum class CaseStatus {
    OPEN,
    IN_PROGRESS,
    SOLVED,
    COLD,
    UNSOLVED
};

enum class CasePriority {
    LOW,
    MEDIUM,
    HIGH,
    URGENT
};

// Forward declaration
class Case;

class CaseUtils {
public:
    static std::string statusToString(CaseStatus status);
    static CaseStatus stringToStatus(const std::string& statusStr);
    static std::string priorityToString(CasePriority priority);
    static CasePriority stringToPriority(const std::string& priorityStr);
    static std::string generateCaseId(int sequence);
    static bool isCaseTitleUnique(const std::string& title, const std::vector<Case>& cases);
};

class Case {
private:
    int id;
    std::string title;
    std::string description;
    std::string location;
    CaseStatus status;
    CasePriority priority;
    std::string solution;
    std::string notes;
    std::vector<int> suspectIds;
    std::vector<int> characterIds;
    std::vector<std::string> evidence;
    std::vector<std::string> tags;
    std::chrono::system_clock::time_point dateCreated;
    std::chrono::system_clock::time_point dateModified;
    std::chrono::system_clock::time_point incidentDate;

    void updateModificationDate();

public:
    // Constructors
    Case();
    Case(int id, const std::string& title, const std::string& description);
    Case(int id, const std::string& title, const std::string& description,
         const std::string& location, CasePriority priority);

    bool operator==(const Case& other) const {
        return id == other.id && title == other.title;
    }
    
    bool operator<(const Case& other) const {
        return title < other.title; // Or use id for comparison
    }
    
    bool operator>(const Case& other) const {
        return title > other.title; // Or use id for comparison
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Case& c);
    // Getters
    int getId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getLocation() const;
    std::string getStatusString() const;
    std::string getPriorityString() const;
    CaseStatus getStatus() const;
    CasePriority getPriority() const;
    std::string getSolution() const;
    std::string getNotes() const;
    std::vector<int> getSuspects() const;
    std::vector<int> getCharacters() const;
    std::vector<std::string> getEvidence() const;
    std::vector<std::string> getTags() const;
    std::string getCreationDate() const;
    std::string getModificationDate() const;
    std::string getIncidentDate() const;

    // Setters
    void setTitle(const std::string& newTitle);
    void setDescription(const std::string& newDescription);
    void setLocation(const std::string& newLocation);
    void setStatus(CaseStatus newStatus);
    void setPriority(CasePriority newPriority);
    void setSolution(const std::string& newSolution);
    void setNotes(const std::string& newNotes);
    void setIncidentDate(const std::chrono::system_clock::time_point& date);

    // Management methods
    void addSuspect(int suspectId);
    void removeSuspect(int suspectId);
    void addCharacter(int characterId);
    void removeCharacter(int characterId);
    void addEvidence(const std::string& evidenceItem);
    void removeEvidence(const std::string& evidenceItem);
    void addTag(const std::string& tag);
    void removeTag(const std::string& tag);
    void clearEvidence();
    void clearTags();

    // Utility methods
    bool isSolved() const;
    bool isColdCase() const;
    bool involvesSuspect(int suspectId) const;
    bool involvesCharacter(int characterId) const;
    bool hasEvidence(const std::string& evidenceItem) const;
    bool hasTag(const std::string& tag) const;
    int getDaysSinceIncident() const;
    int getDaysSinceCreation() const;
    bool isValid() const;
    static bool validateTitle(const std::string& title);
    static bool validateDescription(const std::string& description);

    // Display methods
    void display() const;
    void displaySummary() const;
    void displayDetailed() const;
    std::string to_string() const;

    // Serialization
    std::string serialize() const;
    static Case deserialize(const std::string& data);
};

#endif // CASE_H