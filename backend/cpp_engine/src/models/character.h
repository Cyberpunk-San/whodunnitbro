#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>

enum class CharacterRole {
    WITNESS,
    INFORMANT,
    VICTIM,
    OFFICER,
    DETECTIVE,
    EXPERT,
    OTHER
};

enum class Reliability {
    UNRELIABLE,
    SOMEWHAT_RELIABLE,
    RELIABLE,
    HIGHLY_RELIABLE
};

class Character {
private:
    int id;
    std::string name;
    CharacterRole role;
    std::string story;
    std::vector<int> relatedCases;
    std::vector<std::string> knownSuspects;

public:
    // Constructors
    Character();
    Character(int id, const std::string& name, CharacterRole role, const std::string& story);
    
    // Getters
    int getId() const;
    std::string getName() const;
    CharacterRole getRole() const;
    std::string getRoleString() const;
    std::string getStory() const;
    std::vector<int> getRelatedCases() const;
    std::vector<std::string> getKnownSuspects() const;
    
    // Setters
    void setName(const std::string& newName);
    void setRole(CharacterRole newRole);
    void setStory(const std::string& newStory);
    
    // Management methods
    void addCase(int caseId);
    void removeCase(int caseId);
    void addKnownSuspect(const std::string& suspectName);
    void removeKnownSuspect(const std::string& suspectName);
    void clearRelatedCases();
    void clearKnownSuspects();
    
    // Utility methods
    bool isInvolvedInCase(int caseId) const;
    bool knowsSuspect(const std::string& suspectName) const;
    int getCaseInvolvementCount() const;
    int getKnownSuspectsCount() const;
    
    // Display methods
    void display() const;
    void displaySummary() const;
    void displayDetailed() const;
    std::string to_string() const;
    
    // Serialization
    std::string serialize() const;
    static Character deserialize(const std::string& data);
    
    // Validation
    bool isValid() const;
    static bool validateName(const std::string& name);
    static bool validateRole(CharacterRole role);
    bool operator==(const Character& other) const {
        return id == other.id && name == other.name;
    }

    bool operator<(const Character& other) const {
        return name < other.name;
    }

    bool operator>(const Character& other) const {
        return name > other.name;
    }
        
    friend std::ostream& operator<<(std::ostream& os, const Character& ch);
};

class CharacterUtils {
public:
    static std::string roleToString(CharacterRole role);
    static CharacterRole stringToRole(const std::string& roleStr);
    static std::string reliabilityToString(Reliability reliability);
    static Reliability stringToReliability(const std::string& reliabilityStr);
    static std::string generateCharacterId(int sequence);
    static bool isCharacterNameUnique(const std::string& name, const std::vector<Character>& characters);
};

#endif // CHARACTER_H