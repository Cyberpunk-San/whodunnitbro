#include <iostream>
#include "core/engine.h"
#include "core/story_manager.h"
#include "core/utils.h"

int main() {
    std::cout << "=== WhoDunnitBro C++ Engine Test ===\n\n";

    Engine engine;                     // Core engine
    StoryManager story(&engine);       // Reporting / narrative layer

    // --------------------------------------------------------
    // 1. ADD SAMPLE CASES
    // --------------------------------------------------------
    std::cout << "\nAdding cases...\n";
    engine.addCase("Diamond Heist",
                   "A priceless diamond was stolen from the museum.",
                   CaseStatus::OPEN, 
                   CasePriority::HIGH);

    engine.addCase("Missing Scientist",
                   "A researcher disappeared from the lab under mysterious circumstances.",
                   CaseStatus::OPEN,
                   CasePriority::MEDIUM);

    // --------------------------------------------------------
    // 2. ADD SAMPLE SUSPECTS
    // --------------------------------------------------------
    std::cout << "\nAdding suspects...\n";
    engine.addSuspect("John Vex", 
                      "Former security guard with a criminal past.",
                      "Was acting strangely around the museum.",
                      42,
                      "Unemployed");

    engine.addSuspect("Linda Frost",
                      "Lab assistant with access to restricted areas.",
                      "Discovered arguing with the missing scientist.",
                      29,
                      "Biochemist");

    // --------------------------------------------------------
    // 3. ADD CHARACTERS / WITNESSES
    // --------------------------------------------------------
    std::cout << "\nAdding characters...\n";
    engine.addCharacter("Detective Rowan", CharacterRole::DETECTIVE, 
                        "Lead investigator with a sharp intuition.");

    engine.addCharacter("Evan Glass", CharacterRole::WITNESS,
                        "Claims to have seen a shadowy figure at the museum.");

    // --------------------------------------------------------
    // 4. LINK ENTITIES IN RELATIONSHIP GRAPH
    // --------------------------------------------------------
    std::cout << "\nLinking suspects and characters to cases...\n";
    engine.linkSuspectToCase("John Vex", "Diamond Heist");
    engine.linkSuspectToCase("Linda Frost", "Missing Scientist");

    engine.linkCharacterToCase("Detective Rowan", "Diamond Heist");
    engine.linkCharacterToCase("Evan Glass", "Diamond Heist");

    // --------------------------------------------------------
    // 5. RECALCULATE SUSPICION LEVELS
    // --------------------------------------------------------
    std::cout << "\nRecalculating suspicion levels...\n";
    engine.recalculateAllSuspicionLevels();

    // --------------------------------------------------------
    // 6. GENERATE STORY REPORTS
    // --------------------------------------------------------
    std::cout << "\n=== Case Summary ===\n";
    std::cout << story.generateCaseSummary("Diamond Heist") << "\n";

    std::cout << "\n=== Suspect Profile ===\n";
    std::cout << story.generateSuspectProfile("John Vex") << "\n";

    std::cout << "\n=== Investigation Timeline ===\n";
    std::cout << story.generateInvestigationTimeline() << "\n";

    std::cout << "\n=== Suspicion Report ===\n";
    std::cout << story.generateSuspicionReport() << "\n";

    // --------------------------------------------------------
    // 7. ANALYZE RELATIONSHIPS / GRAPH
    // --------------------------------------------------------
    std::cout << "\n=== Relationship Path (Case ↔ Suspect) ===\n";
    auto path = engine.findPath("Diamond Heist", "John Vex");

    if (!path.empty()) {
        for (auto& p : path) std::cout << p << " -> ";
        std::cout << "END\n";
    } else {
        std::cout << "No connection found.\n";
    }

    // --------------------------------------------------------
    // 8. STATISTICS FROM ENGINE
    // --------------------------------------------------------
    std::cout << "\n=== Engine Statistics ===\n";
    engine.printStatistics();

    // --------------------------------------------------------
    // 9. DATA INTEGRITY CHECK
    // --------------------------------------------------------
    std::cout << "\n=== Data Integrity Check ===\n";
    if (engine.validateData()) {
        std::cout << "All data structures are valid.\n";
    } else {
        auto issues = engine.getDataIssues();
        for (auto& i : issues) std::cout << "- " << i << "\n";
    }

    std::cout << "\n=== TEST COMPLETE ===\n";
    return 0;
}
