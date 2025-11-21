// Fixed py_wrapper.cpp - use correct return_value_policy to avoid double-free crashes
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include "src/core/engine.h"
#include "src/core/story_manager.h"
#include "src/models/case.h"
#include "src/models/character.h"
#include "src/models/suspect.h"

namespace py = pybind11;

// Enum bindings
PYBIND11_MODULE(whodunnit_engine, m) {
    m.doc() = "Detective Engine Python Bindings - A comprehensive crime investigation engine";

    // ==================== ENUM BINDINGS ====================
    py::enum_<CaseStatus>(m, "CaseStatus")
        .value("OPEN", CaseStatus::OPEN)
        .value("IN_PROGRESS", CaseStatus::IN_PROGRESS)
        .value("SOLVED", CaseStatus::SOLVED)
        .value("COLD", CaseStatus::COLD)
        .value("UNSOLVED", CaseStatus::UNSOLVED)
        .export_values();

    py::enum_<CasePriority>(m, "CasePriority")
        .value("LOW", CasePriority::LOW)
        .value("MEDIUM", CasePriority::MEDIUM)
        .value("HIGH", CasePriority::HIGH)
        .value("URGENT", CasePriority::URGENT)
        .export_values();

    py::enum_<CharacterRole>(m, "CharacterRole")
        .value("WITNESS", CharacterRole::WITNESS)
        .value("INFORMANT", CharacterRole::INFORMANT)
        .value("VICTIM", CharacterRole::VICTIM)
        .value("OFFICER", CharacterRole::OFFICER)
        .value("DETECTIVE", CharacterRole::DETECTIVE)
        .value("EXPERT", CharacterRole::EXPERT)
        .value("OTHER", CharacterRole::OTHER)
        .export_values();

    py::enum_<SuspectStatus>(m, "SuspectStatus")
        .value("UNINVESTIGATED", SuspectStatus::UNINVESTIGATED)
        .value("UNDER_INVESTIGATION", SuspectStatus::UNDER_INVESTIGATION)
        .value("CLEARED", SuspectStatus::CLEARED)
        .value("PRIME_SUSPECT", SuspectStatus::PRIME_SUSPECT)
        .value("CONVICTED", SuspectStatus::CONVICTED)
        .value("ACQUITTED", SuspectStatus::ACQUITTED)
        .export_values();

    py::enum_<AlibiStrength>(m, "AlibiStrength")
        .value("NONE", AlibiStrength::NONE)
        .value("WEAK", AlibiStrength::WEAK)
        .value("MODERATE", AlibiStrength::MODERATE)
        .value("STRONG", AlibiStrength::STRONG)
        .value("CONFIRMED", AlibiStrength::CONFIRMED)
        .export_values();

    py::enum_<Reliability>(m, "Reliability")
        .value("UNRELIABLE", Reliability::UNRELIABLE)
        .value("SOMEWHAT_RELIABLE", Reliability::SOMEWHAT_RELIABLE)
        .value("RELIABLE", Reliability::RELIABLE)
        .value("HIGHLY_RELIABLE", Reliability::HIGHLY_RELIABLE)
        .export_values();

    // ==================== UTILITY CLASSES ====================
    py::class_<CaseUtils>(m, "CaseUtils")
        .def_static("status_to_string", &CaseUtils::statusToString)
        .def_static("string_to_status", &CaseUtils::stringToStatus)
        .def_static("priority_to_string", &CaseUtils::priorityToString)
        .def_static("string_to_priority", &CaseUtils::stringToPriority)
        .def_static("generate_case_id", &CaseUtils::generateCaseId)
        .def_static("is_case_title_unique", &CaseUtils::isCaseTitleUnique);

    py::class_<CharacterUtils>(m, "CharacterUtils")
        .def_static("role_to_string", &CharacterUtils::roleToString)
        .def_static("string_to_role", &CharacterUtils::stringToRole)
        .def_static("reliability_to_string", &CharacterUtils::reliabilityToString)
        .def_static("string_to_reliability", &CharacterUtils::stringToReliability)
        .def_static("generate_character_id", &CharacterUtils::generateCharacterId)
        .def_static("is_character_name_unique", &CharacterUtils::isCharacterNameUnique);

    py::class_<SuspectUtils>(m, "SuspectUtils")
        .def_static("status_to_string", &SuspectUtils::statusToString)
        .def_static("string_to_status", &SuspectUtils::stringToStatus)
        .def_static("alibi_strength_to_string", &SuspectUtils::alibiStrengthToString)
        .def_static("string_to_alibi_strength", &SuspectUtils::stringToAlibiStrength)
        .def_static("generate_suspect_id", &SuspectUtils::generateSuspectId)
        .def_static("is_suspect_name_unique", &SuspectUtils::isSuspectNameUnique);

    // ==================== MODEL CLASSES ====================
    // Case class
    py::class_<Case>(m, "Case")
        .def(py::init<>())
        .def(py::init<int, const std::string&, const std::string&>())
        .def("get_id", &Case::getId)
        .def("get_title", &Case::getTitle)
        .def("get_description", &Case::getDescription)
        .def("get_location", &Case::getLocation)
        .def("get_status", &Case::getStatus)
        .def("get_status_string", &Case::getStatusString)
        .def("get_priority", &Case::getPriority)
        .def("get_priority_string", &Case::getPriorityString)
        .def("get_solution", &Case::getSolution)
        .def("get_notes", &Case::getNotes)
        // return policies for getters that return internal pointers/containers: reference
        .def("get_suspects", &Case::getSuspects, py::return_value_policy::reference)
        .def("get_characters", &Case::getCharacters, py::return_value_policy::reference)
        .def("get_evidence", &Case::getEvidence, py::return_value_policy::reference)
        .def("get_tags", &Case::getTags, py::return_value_policy::reference)
        .def("set_title", &Case::setTitle)
        .def("set_description", &Case::setDescription)
        .def("set_location", &Case::setLocation)
        .def("set_status", &Case::setStatus)
        .def("set_priority", &Case::setPriority)
        .def("set_solution", &Case::setSolution)
        .def("set_notes", &Case::setNotes)
        .def("add_suspect", &Case::addSuspect)
        .def("remove_suspect", &Case::removeSuspect)
        .def("add_character", &Case::addCharacter)
        .def("remove_character", &Case::removeCharacter)
        .def("add_evidence", &Case::addEvidence)
        .def("remove_evidence", &Case::removeEvidence)
        .def("add_tag", &Case::addTag)
        .def("remove_tag", &Case::removeTag)
        .def("is_solved", &Case::isSolved)
        .def("is_cold_case", &Case::isColdCase)
        .def("involves_suspect", &Case::involvesSuspect)
        .def("involves_character", &Case::involvesCharacter)
        .def("has_evidence", &Case::hasEvidence)
        .def("has_tag", &Case::hasTag)
        .def("get_days_since_incident", &Case::getDaysSinceIncident)
        .def("get_days_since_creation", &Case::getDaysSinceCreation)
        .def("is_valid", &Case::isValid)
        .def("display", &Case::display)
        .def("display_summary", &Case::displaySummary)
        .def("display_detailed", &Case::displayDetailed)
        .def("to_string", &Case::to_string)
        .def("serialize", &Case::serialize)
        .def_static("deserialize", &Case::deserialize);

    // Character class
    py::class_<Character>(m, "Character")
        .def(py::init<>())
        .def(py::init<int, const std::string&, CharacterRole, const std::string&>())
        .def("get_id", &Character::getId)
        .def("get_name", &Character::getName)
        .def("get_role", &Character::getRole)
        .def("get_role_string", &Character::getRoleString)
        .def("get_story", &Character::getStory)
        .def("get_related_cases", &Character::getRelatedCases, py::return_value_policy::reference)
        .def("get_known_suspects", &Character::getKnownSuspects, py::return_value_policy::reference)
        .def("set_name", &Character::setName)
        .def("set_role", &Character::setRole)
        .def("set_story", &Character::setStory)
        .def("add_case", &Character::addCase)
        .def("remove_case", &Character::removeCase)
        .def("add_known_suspect", &Character::addKnownSuspect)
        .def("remove_known_suspect", &Character::removeKnownSuspect)
        .def("clear_related_cases", &Character::clearRelatedCases)
        .def("clear_known_suspects", &Character::clearKnownSuspects)
        .def("is_involved_in_case", &Character::isInvolvedInCase)
        .def("knows_suspect", &Character::knowsSuspect)
        .def("get_case_involvement_count", &Character::getCaseInvolvementCount)
        .def("get_known_suspects_count", &Character::getKnownSuspectsCount)
        .def("display", &Character::display)
        .def("display_summary", &Character::displaySummary)
        .def("display_detailed", &Character::displayDetailed)
        .def("to_string", &Character::to_string)
        .def("serialize", &Character::serialize)
        .def_static("deserialize", &Character::deserialize)
        .def("is_valid", &Character::isValid);

    // Suspect class
    py::class_<Suspect>(m, "Suspect")
        .def(py::init<>())
        .def(py::init<const std::string&, const std::string&>())
        .def(py::init<int, const std::string&, const std::string&>())
        .def("get_id", &Suspect::getId)
        .def("get_name", &Suspect::getName)
        .def("get_story", &Suspect::getStory)
        .def("get_background", &Suspect::getBackground)
        .def("get_motive", &Suspect::getMotive)
        .def("get_alibi", &Suspect::getAlibi)
        .def("get_alibi_strength", &Suspect::getAlibiStrength)
        .def("get_alibi_strength_string", &Suspect::getAlibiStrengthString)
        .def("get_status", &Suspect::getStatus)
        .def("get_status_string", &Suspect::getStatusString)
        .def("get_age", &Suspect::getAge)
        .def("get_occupation", &Suspect::getOccupation)
        .def("get_last_known_location", &Suspect::getLastKnownLocation)
        .def("get_cases", &Suspect::getCases, py::return_value_policy::reference)
        .def("get_physical_description", &Suspect::getPhysicalDescription, py::return_value_policy::reference)
        .def("get_known_associates", &Suspect::getKnownAssociates, py::return_value_policy::reference)
        .def("get_evidence_against", &Suspect::getEvidenceAgainst, py::return_value_policy::reference)
        .def("get_evidence_for", &Suspect::getEvidenceFor, py::return_value_policy::reference)
        .def("get_suspicion_level", &Suspect::getSuspicionLevel)
        .def("get_suspicion_level_string", &Suspect::getSuspicionLevelString)
        .def("set_name", &Suspect::setName)
        .def("set_story", &Suspect::setStory)
        .def("set_background", &Suspect::setBackground)
        .def("set_motive", &Suspect::setMotive)
        .def("set_alibi", &Suspect::setAlibi)
        .def("set_alibi_strength", &Suspect::setAlibiStrength)
        .def("set_status", &Suspect::setStatus)
        .def("set_age", &Suspect::setAge)
        .def("set_occupation", &Suspect::setOccupation)
        .def("set_last_known_location", &Suspect::setLastKnownLocation)
        .def("set_suspicion_level", &Suspect::setSuspicionLevel)
        .def("add_case", &Suspect::addCase)
        .def("remove_case", &Suspect::removeCase)
        .def("add_physical_description", &Suspect::addPhysicalDescription)
        .def("remove_physical_description", &Suspect::removePhysicalDescription)
        .def("add_known_associate", &Suspect::addKnownAssociate)
        .def("remove_known_associate", &Suspect::removeKnownAssociate)
        .def("add_evidence_against", &Suspect::addEvidenceAgainst)
        .def("remove_evidence_against", &Suspect::removeEvidenceAgainst)
        .def("add_evidence_for", &Suspect::addEvidenceFor)
        .def("remove_evidence_for", &Suspect::removeEvidenceFor)
        .def("clear_physical_description", &Suspect::clearPhysicalDescription)
        .def("clear_known_associates", &Suspect::clearKnownAssociates)
        .def("clear_evidence", &Suspect::clearEvidence)
        .def("is_prime_suspect", &Suspect::isPrimeSuspect)
        .def("is_cleared", &Suspect::isCleared)
        .def("has_strong_alibi", &Suspect::hasStrongAlibi)
        .def("is_involved_in_case", &Suspect::isInvolvedInCase)
        .def("has_evidence", &Suspect::hasEvidence)
        .def("has_known_associate", &Suspect::hasKnownAssociate)
        .def("has_motive", &Suspect::hasMotive)
        .def("has_alibi", &Suspect::hasAlibi)
        .def("get_evidence_count", &Suspect::getEvidenceCount)
        .def("get_case_involvement_count", &Suspect::getCaseInvolvementCount)
        .def("update_suspicion_level", &Suspect::updateSuspicionLevel)
        .def("calculate_suspicion_score", &Suspect::calculateSuspicionScore)
        .def("display", &Suspect::display)
        .def("display_summary", &Suspect::displaySummary)
        .def("display_detailed", &Suspect::displayDetailed)
        .def("to_string", &Suspect::to_string)
        .def("serialize", &Suspect::serialize)
        .def_static("deserialize", &Suspect::deserialize)
        .def("is_valid", &Suspect::isValid);

    // ==================== ENGINE STATISTICS STRUCT ====================
    py::class_<Engine::Statistics>(m, "EngineStatistics")
        .def_readonly("total_cases", &Engine::Statistics::totalCases)
        .def_readonly("solved_cases", &Engine::Statistics::solvedCases)
        .def_readonly("open_cases", &Engine::Statistics::openCases)
        .def_readonly("total_suspects", &Engine::Statistics::totalSuspects)
        .def_readonly("prime_suspects", &Engine::Statistics::primeSuspects)
        .def_readonly("cleared_suspects", &Engine::Statistics::clearedSuspects)
        .def_readonly("total_characters", &Engine::Statistics::totalCharacters)
        .def_readonly("witnesses", &Engine::Statistics::witnesses)
        .def_readonly("detectives", &Engine::Statistics::detectives)
        .def_readonly("average_suspicion_level", &Engine::Statistics::averageSuspicionLevel)
        .def_readonly("total_relationships", &Engine::Statistics::totalRelationships)
        .def("__repr__", [](const Engine::Statistics& stats) {
            return "EngineStatistics(total_cases=" + std::to_string(stats.totalCases) +
                   ", solved_cases=" + std::to_string(stats.solvedCases) +
                   ", total_suspects=" + std::to_string(stats.totalSuspects) + ")";
        });

    // ==================== MAIN ENGINE CLASS ====================
    py::class_<Engine>(m, "DetectiveEngine")
        .def(py::init<>())
        
        // Case Management
        .def("add_case", &Engine::addCase, 
             py::arg("title"), py::arg("description"), 
             py::arg("status") = CaseStatus::OPEN, 
             py::arg("priority") = CasePriority::MEDIUM)
        .def("remove_case", &Engine::removeCase)
        .def("update_case", &Engine::updateCase)
        .def("find_case", &Engine::findCase, py::return_value_policy::reference)
        .def("find_case_by_id", &Engine::findCaseById, py::return_value_policy::reference)
        .def("get_all_cases", &Engine::getAllCases, py::return_value_policy::reference)
        .def("find_cases_by_status", &Engine::findCasesByStatus, py::return_value_policy::reference)
        .def("find_cases_by_priority", &Engine::findCasesByPriority, py::return_value_policy::reference)
        .def("search_cases", &Engine::searchCases, py::return_value_policy::reference)
        
        // Suspect Management
        .def("add_suspect", &Engine::addSuspect,
             py::arg("name"), py::arg("background"),
             py::arg("story") = "", py::arg("age") = 0,
             py::arg("occupation") = "Unknown")
        .def("remove_suspect", &Engine::removeSuspect)
        .def("update_suspect", &Engine::updateSuspect)
        .def("find_suspect", &Engine::findSuspect, py::return_value_policy::reference)
        .def("find_suspect_by_id", &Engine::findSuspectById, py::return_value_policy::reference)
        .def("get_all_suspects", &Engine::getAllSuspects, py::return_value_policy::reference)
        .def("find_suspects_by_status", &Engine::findSuspectsByStatus, py::return_value_policy::reference)
        .def("find_suspects_by_suspicion_range", &Engine::findSuspectsBySuspicionRange, py::return_value_policy::reference)
        .def("search_suspects", &Engine::searchSuspects, py::return_value_policy::reference)
        
        // Character Management
        .def("add_character", &Engine::addCharacter,
             py::arg("name"), py::arg("role"),
             py::arg("story") = "")
        .def("remove_character", &Engine::removeCharacter)
        .def("update_character", &Engine::updateCharacter)
        .def("find_character", &Engine::findCharacter, py::return_value_policy::reference)
        .def("find_character_by_id", &Engine::findCharacterById, py::return_value_policy::reference)
        .def("get_all_characters", &Engine::getAllCharacters, py::return_value_policy::reference)
        .def("find_characters_by_role", &Engine::findCharactersByRole, py::return_value_policy::reference)
        .def("search_characters", &Engine::searchCharacters, py::return_value_policy::reference)
        
        // Relationship Management
        .def("link_suspect_to_case", &Engine::linkSuspectToCase)
        .def("unlink_suspect_from_case", &Engine::unlinkSuspectFromCase)
        .def("link_character_to_case", &Engine::linkCharacterToCase)
        .def("unlink_character_from_case", &Engine::unlinkCharacterFromCase)
        .def("add_relationship", &Engine::addRelationship,
             py::arg("entity1"), py::arg("entity2"),
             py::arg("relationship_type") = "related")
        .def("remove_relationship", &Engine::removeRelationship)
        .def("get_relationships", &Engine::getRelationships, py::return_value_policy::reference)
        .def("find_path", &Engine::findPath, py::return_value_policy::reference)
        
        // Analysis & Queries
        .def("get_suspects_for_case", &Engine::getSuspectsForCase, py::return_value_policy::reference)
        .def("get_characters_for_case", &Engine::getCharactersForCase, py::return_value_policy::reference)
        .def("get_cases_for_suspect", &Engine::getCasesForSuspect, py::return_value_policy::reference)
        .def("get_cases_for_character", &Engine::getCasesForCharacter, py::return_value_policy::reference)
        .def("get_prime_suspects", &Engine::getPrimeSuspects, py::return_value_policy::reference)
        .def("get_unsolved_cases", &Engine::getUnsolvedCases, py::return_value_policy::reference)
        .def("get_high_priority_cases", &Engine::getHighPriorityCases, py::return_value_policy::reference)
        .def("recalculate_all_suspicion_levels", &Engine::recalculateAllSuspicionLevels)
        .def("get_top_suspects", &Engine::getTopSuspects, py::arg("count") = 5, py::return_value_policy::reference)
        .def("find_connected_suspects", &Engine::findConnectedSuspects,
             py::arg("suspect_name"), py::arg("max_depth") = 2, py::return_value_policy::reference)
        
        // Statistics
        .def("get_statistics", &Engine::getStatistics)
        .def("print_statistics", &Engine::printStatistics)
        
        // Data Integrity
        .def("validate_data", &Engine::validateData)
        .def("get_data_issues", &Engine::getDataIssues, py::return_value_policy::reference)
        .def("rebuild_all_connections", &Engine::rebuildAllConnections)
        
        // Utility Methods
        .def("display_all_data", &Engine::displayAllData)
        .def("display_case_network", &Engine::displayCaseNetwork)
        .def("display_suspect_network", &Engine::displaySuspectNetwork)
        .def("print_debug_info", &Engine::printDebugInfo);

    // ==================== STORY MANAGER CLASS ====================
    py::class_<StoryManager>(m, "StoryManager")
        .def(py::init<Engine*>())
        .def("generate_case_summary", &StoryManager::generateCaseSummary)
        .def("generate_suspect_profile", &StoryManager::generateSuspectProfile)
        .def("generate_character_introduction", &StoryManager::generateCharacterIntroduction)
        .def("generate_investigation_timeline", &StoryManager::generateInvestigationTimeline)
        .def("generate_case_analysis", &StoryManager::generateCaseAnalysis)
        .def("generate_suspicion_report", &StoryManager::generateSuspicionReport)
        .def("find_missing_connections", &StoryManager::findMissingConnections)
        .def("suggest_next_steps", &StoryManager::suggestNextSteps);

    // ==================== UTILITY FUNCTIONS ====================
    // When creating a new Engine or StoryManager from Python, transfer ownership to Python
    m.def("initialize_engine", []() { return new Engine(); }, py::return_value_policy::take_ownership);
    m.def("create_story_manager", [](Engine* engine) { return new StoryManager(engine); }, py::return_value_policy::take_ownership);

    // Version info
    m.attr("__version__") = "1.0.0";
    m.attr("__author__") = "WhoDunnitBro Team";
    m.attr("__description__") = "A comprehensive detective engine for crime investigation and case management";
}
