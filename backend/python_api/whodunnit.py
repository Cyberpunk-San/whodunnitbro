"""
WhoDunnit Engine - Python API Wrapper
A comprehensive Python interface for the detective engine.
"""

import os
import sys
from typing import List, Dict, Optional, Union, Any, Tuple
from enum import Enum
from datetime import datetime, timedelta

# Import the compiled C++ module
try:
    from . import whodunnit_engine as engine_native
except ImportError:
    # For development, try direct import
    import whodunnit_engine as engine_native


class CaseStatus(Enum):
    """Case status enumeration"""
    OPEN = engine_native.CaseStatus.OPEN
    IN_PROGRESS = engine_native.CaseStatus.IN_PROGRESS
    SOLVED = engine_native.CaseStatus.SOLVED
    COLD = engine_native.CaseStatus.COLD
    UNSOLVED = engine_native.CaseStatus.UNSOLVED


class CasePriority(Enum):
    """Case priority enumeration"""
    LOW = engine_native.CasePriority.LOW
    MEDIUM = engine_native.CasePriority.MEDIUM
    HIGH = engine_native.CasePriority.HIGH
    URGENT = engine_native.CasePriority.URGENT


class CharacterRole(Enum):
    """Character role enumeration"""
    WITNESS = engine_native.CharacterRole.WITNESS
    INFORMANT = engine_native.CharacterRole.INFORMANT
    VICTIM = engine_native.CharacterRole.VICTIM
    OFFICER = engine_native.CharacterRole.OFFICER
    DETECTIVE = engine_native.CharacterRole.DETECTIVE
    EXPERT = engine_native.CharacterRole.EXPERT
    OTHER = engine_native.CharacterRole.OTHER


class SuspectStatus(Enum):
    """Suspect status enumeration"""
    UNINVESTIGATED = engine_native.SuspectStatus.UNINVESTIGATED
    UNDER_INVESTIGATION = engine_native.SuspectStatus.UNDER_INVESTIGATION
    CLEARED = engine_native.SuspectStatus.CLEARED
    PRIME_SUSPECT = engine_native.SuspectStatus.PRIME_SUSPECT
    CONVICTED = engine_native.SuspectStatus.CONVICTED
    ACQUITTED = engine_native.SuspectStatus.ACQUITTED


class AlibiStrength(Enum):
    """Alibi strength enumeration"""
    NONE = engine_native.AlibiStrength.NONE
    WEAK = engine_native.AlibiStrength.WEAK
    MODERATE = engine_native.AlibiStrength.MODERATE
    STRONG = engine_native.AlibiStrength.STRONG
    CONFIRMED = engine_native.AlibiStrength.CONFIRMED


class Reliability(Enum):
    """Reliability enumeration"""
    UNRELIABLE = engine_native.Reliability.UNRELIABLE
    SOMEWHAT_RELIABLE = engine_native.Reliability.SOMEWHAT_RELIABLE
    RELIABLE = engine_native.Reliability.RELIABLE
    HIGHLY_RELIABLE = engine_native.Reliability.HIGHLY_RELIABLE


class Case:
    """Python wrapper for Case class"""
    
    def __init__(self, native_case=None):
        if native_case is None:
            self._native_case = engine_native.Case()
        else:
            self._native_case = native_case
    
    @property
    def id(self) -> int:
        return self._native_case.get_id()
    
    @property
    def title(self) -> str:
        return self._native_case.get_title()
    
    @title.setter
    def title(self, value: str):
        self._native_case.set_title(value)
    
    @property
    def description(self) -> str:
        return self._native_case.get_description()
    
    @description.setter
    def description(self, value: str):
        self._native_case.set_description(value)
    
    @property
    def location(self) -> str:
        return self._native_case.get_location()
    
    @location.setter
    def location(self, value: str):
        self._native_case.set_location(value)
    
    @property
    def status(self) -> CaseStatus:
        return CaseStatus(self._native_case.get_status())
    
    @status.setter
    def status(self, value: CaseStatus):
        self._native_case.set_status(value.value)
    
    @property
    def priority(self) -> CasePriority:
        return CasePriority(self._native_case.get_priority())
    
    @priority.setter
    def priority(self, value: CasePriority):
        self._native_case.set_priority(value.value)
    
    @property
    def solution(self) -> str:
        return self._native_case.get_solution()
    
    @solution.setter
    def solution(self, value: str):
        self._native_case.set_solution(value)
    
    @property
    def notes(self) -> str:
        return self._native_case.get_notes()
    
    @notes.setter
    def notes(self, value: str):
        self._native_case.set_notes(value)
    
    @property
    def suspects(self) -> List['Suspect']:
        native_suspects = self._native_case.get_suspects()
        return [Suspect(native_suspect) for native_suspect in native_suspects]
    
    @property
    def characters(self) -> List['Character']:
        native_chars = self._native_case.get_characters()
        return [Character(native_char) for native_char in native_chars]
    
    @property
    def evidence(self) -> List[str]:
        return self._native_case.get_evidence()
    
    @property
    def tags(self) -> List[str]:
        return self._native_case.get_tags()
    
    def add_suspect(self, suspect: 'Suspect') -> bool:
        return self._native_case.add_suspect(suspect._native_suspect)
    
    def remove_suspect(self, suspect: 'Suspect') -> bool:
        return self._native_case.remove_suspect(suspect._native_suspect)
    
    def add_character(self, character: 'Character') -> bool:
        return self._native_case.add_character(character._native_character)
    
    def remove_character(self, character: 'Character') -> bool:
        return self._native_case.remove_character(character._native_character)
    
    def add_evidence(self, evidence: str) -> bool:
        return self._native_case.add_evidence(evidence)
    
    def remove_evidence(self, evidence: str) -> bool:
        return self._native_case.remove_evidence(evidence)
    
    def add_tag(self, tag: str) -> bool:
        return self._native_case.add_tag(tag)
    
    def remove_tag(self, tag: str) -> bool:
        return self._native_case.remove_tag(tag)
    
    def is_solved(self) -> bool:
        return self._native_case.is_solved()
    
    def is_cold_case(self) -> bool:
        return self._native_case.is_cold_case()
    
    def involves_suspect(self, suspect: 'Suspect') -> bool:
        return self._native_case.involves_suspect(suspect._native_suspect)
    
    def involves_character(self, character: 'Character') -> bool:
        return self._native_case.involves_character(character._native_character)
    
    def has_evidence(self, evidence: str) -> bool:
        return self._native_case.has_evidence(evidence)
    
    def has_tag(self, tag: str) -> bool:
        return self._native_case.has_tag(tag)
    
    def get_days_since_incident(self) -> int:
        return self._native_case.get_days_since_incident()
    
    def get_days_since_creation(self) -> int:
        return self._native_case.get_days_since_creation()
    
    def is_valid(self) -> bool:
        return self._native_case.is_valid()
    
    def display(self):
        self._native_case.display()
    
    def display_summary(self):
        self._native_case.display_summary()
    
    def display_detailed(self):
        self._native_case.display_detailed()
    
    def to_dict(self) -> Dict[str, Any]:
        return {
            'id': self.id,
            'title': self.title,
            'description': self.description,
            'location': self.location,
            'status': self.status.name,
            'priority': self.priority.name,
            'solution': self.solution,
            'notes': self.notes,
            'suspect_count': len(self.suspects),
            'character_count': len(self.characters),
            'evidence_count': len(self.evidence),
            'tag_count': len(self.tags),
            'is_solved': self.is_solved(),
            'is_cold_case': self.is_cold_case(),
            'days_since_incident': self.get_days_since_incident(),
            'days_since_creation': self.get_days_since_creation()
        }
    
    def __str__(self) -> str:
        return self._native_case.to_string()
    
    def __repr__(self) -> str:
        return f"Case(id={self.id}, title='{self.title}', status={self.status.name})"


class Character:
    """Python wrapper for Character class"""
    
    def __init__(self, native_character=None):
        if native_character is None:
            self._native_character = engine_native.Character()
        else:
            self._native_character = native_character
    
    @property
    def id(self) -> int:
        return self._native_character.get_id()
    
    @property
    def name(self) -> str:
        return self._native_character.get_name()
    
    @name.setter
    def name(self, value: str):
        self._native_character.set_name(value)
    
    @property
    def role(self) -> CharacterRole:
        return CharacterRole(self._native_character.get_role())
    
    @role.setter
    def role(self, value: CharacterRole):
        self._native_character.set_role(value.value)
    
    @property
    def story(self) -> str:
        return self._native_character.get_story()
    
    @story.setter
    def story(self, value: str):
        self._native_character.set_story(value)
    
    @property
    def related_cases(self) -> List[Case]:
        native_cases = self._native_character.get_related_cases()
        return [Case(native_case) for native_case in native_cases]
    
    @property
    def known_suspects(self) -> List['Suspect']:
        native_suspects = self._native_character.get_known_suspects()
        return [Suspect(native_suspect) for native_suspect in native_suspects]
    
    def add_case(self, case: Case) -> bool:
        return self._native_character.add_case(case._native_case)
    
    def remove_case(self, case: Case) -> bool:
        return self._native_character.remove_case(case._native_case)
    
    def add_known_suspect(self, suspect: 'Suspect') -> bool:
        return self._native_character.add_known_suspect(suspect._native_suspect)
    
    def remove_known_suspect(self, suspect: 'Suspect') -> bool:
        return self._native_character.remove_known_suspect(suspect._native_suspect)
    
    def is_involved_in_case(self, case: Case) -> bool:
        return self._native_character.is_involved_in_case(case._native_case)
    
    def knows_suspect(self, suspect: 'Suspect') -> bool:
        return self._native_character.knows_suspect(suspect._native_suspect)
    
    def get_case_involvement_count(self) -> int:
        return self._native_character.get_case_involvement_count()
    
    def get_known_suspects_count(self) -> int:
        return self._native_character.get_known_suspects_count()
    
    def to_dict(self) -> Dict[str, Any]:
        return {
            'id': self.id,
            'name': self.name,
            'role': self.role.name,
            'story': self.story,
            'case_involvement_count': self.get_case_involvement_count(),
            'known_suspects_count': self.get_known_suspects_count()
        }
    
    def __str__(self) -> str:
        return self._native_character.to_string()
    
    def __repr__(self) -> str:
        return f"Character(id={self.id}, name='{self.name}', role={self.role.name})"


class Suspect:
    """Python wrapper for Suspect class"""
    
    def __init__(self, native_suspect=None):
        if native_suspect is None:
            self._native_suspect = engine_native.Suspect()
        else:
            self._native_suspect = native_suspect
    
    @property
    def id(self) -> int:
        return self._native_suspect.get_id()
    
    @property
    def name(self) -> str:
        return self._native_suspect.get_name()
    
    @name.setter
    def name(self, value: str):
        self._native_suspect.set_name(value)
    
    @property
    def story(self) -> str:
        return self._native_suspect.get_story()
    
    @story.setter
    def story(self, value: str):
        self._native_suspect.set_story(value)
    
    @property
    def background(self) -> str:
        return self._native_suspect.get_background()
    
    @background.setter
    def background(self, value: str):
        self._native_suspect.set_background(value)
    
    @property
    def motive(self) -> str:
        return self._native_suspect.get_motive()
    
    @motive.setter
    def motive(self, value: str):
        self._native_suspect.set_motive(value)
    
    @property
    def alibi(self) -> str:
        return self._native_suspect.get_alibi()
    
    @alibi.setter
    def alibi(self, value: str):
        self._native_suspect.set_alibi(value)
    
    @property
    def alibi_strength(self) -> AlibiStrength:
        return AlibiStrength(self._native_suspect.get_alibi_strength())
    
    @alibi_strength.setter
    def alibi_strength(self, value: AlibiStrength):
        self._native_suspect.set_alibi_strength(value.value)
    
    @property
    def status(self) -> SuspectStatus:
        return SuspectStatus(self._native_suspect.get_status())
    
    @status.setter
    def status(self, value: SuspectStatus):
        self._native_suspect.set_status(value.value)
    
    @property
    def age(self) -> int:
        return self._native_suspect.get_age()
    
    @age.setter
    def age(self, value: int):
        self._native_suspect.set_age(value)
    
    @property
    def occupation(self) -> str:
        return self._native_suspect.get_occupation()
    
    @occupation.setter
    def occupation(self, value: str):
        self._native_suspect.set_occupation(value)
    
    @property
    def last_known_location(self) -> str:
        return self._native_suspect.get_last_known_location()
    
    @last_known_location.setter
    def last_known_location(self, value: str):
        self._native_suspect.set_last_known_location(value)
    
    @property
    def suspicion_level(self) -> float:
        return self._native_suspect.get_suspicion_level()
    
    @suspicion_level.setter
    def suspicion_level(self, value: float):
        self._native_suspect.set_suspicion_level(value)
    
    def is_prime_suspect(self) -> bool:
        return self._native_suspect.is_prime_suspect()
    
    def is_cleared(self) -> bool:
        return self._native_suspect.is_cleared()
    
    def has_strong_alibi(self) -> bool:
        return self._native_suspect.has_strong_alibi()
    
    def to_dict(self) -> Dict[str, Any]:
        return {
            'id': self.id,
            'name': self.name,
            'age': self.age,
            'occupation': self.occupation,
            'status': self.status.name,
            'alibi_strength': self.alibi_strength.name,
            'suspicion_level': self.suspicion_level,
            'background': self.background,
            'motive': self.motive,
            'alibi': self.alibi,
            'last_known_location': self.last_known_location,
            'is_prime_suspect': self.is_prime_suspect(),
            'is_cleared': self.is_cleared(),
            'has_strong_alibi': self.has_strong_alibi()
        }
    
    def __str__(self) -> str:
        return self._native_suspect.to_string()
    
    def __repr__(self) -> str:
        return f"Suspect(id={self.id}, name='{self.name}', status={self.status.name})"


class DetectiveEngine:
    """Main Python interface for the detective engine"""
    
    def __init__(self):
        self._engine = engine_native.initialize_engine()
        self._story_manager = engine_native.create_story_manager(self._engine)
        self._case_cache = {}
        self._suspect_cache = {}
        self._character_cache = {}
    
    # Case Management
    def create_case(self, title: str, description: str, status: CaseStatus = CaseStatus.OPEN, 
                   priority: CasePriority = CasePriority.MEDIUM) -> Optional[Case]:
        """Create a new case - returns Case object if successful"""
        success = self._engine.add_case(title, description, status.value, priority.value)
        if success:
            # Find the case that was just created by matching title and description
            all_cases = self._engine.get_all_cases()
            for native_case in all_cases:
                if (native_case.get_title() == title and 
                    native_case.get_description() == description and
                    native_case.get_status() == status.value):
                    case = Case(native_case)
                    self._case_cache[case.id] = case
                    return case
        return None
    
    def get_case(self, case_id: int) -> Optional[Case]:
        """Get case by ID"""
        if case_id in self._case_cache:
            return self._case_cache[case_id]
        
        native_case = self._engine.find_case_by_id(case_id)
        if native_case:
            case = Case(native_case)
            self._case_cache[case_id] = case
            return case
        return None
    
    def find_case(self, title: str) -> Optional[Case]:
        """Find case by title"""
        native_case = self._engine.find_case(title)
        if native_case:
            case = Case(native_case)
            self._case_cache[case.id] = case
            return case
        return None
    
    def get_all_cases(self) -> List[Case]:
        """Get all cases"""
        native_cases = self._engine.get_all_cases()
        cases = []
        for native_case in native_cases:
            case = Case(native_case)
            self._case_cache[case.id] = case
            cases.append(case)
        return cases
    
    def find_cases_by_status(self, status: CaseStatus) -> List[Case]:
        """Find cases by status"""
        native_cases = self._engine.find_cases_by_status(status.value)
        cases = []
        for native_case in native_cases:
            case = Case(native_case)
            self._case_cache[case.id] = case
            cases.append(case)
        return cases
    
    def find_cases_by_priority(self, priority: CasePriority) -> List[Case]:
        """Find cases by priority"""
        native_cases = self._engine.find_cases_by_priority(priority.value)
        cases = []
        for native_case in native_cases:
            case = Case(native_case)
            self._case_cache[case.id] = case
            cases.append(case)
        return cases
    
    def search_cases(self, query: str) -> List[Case]:
        """Search cases by query"""
        native_cases = self._engine.search_cases(query)
        cases = []
        for native_case in native_cases:
            case = Case(native_case)
            self._case_cache[case.id] = case
            cases.append(case)
        return cases
    
    def remove_case(self, case_title: str) -> bool:
        """Remove a case by title"""
        success = self._engine.remove_case(case_title)
        if success:
            # Remove from cache if present
            for case_id, case in list(self._case_cache.items()):
                if case.title == case_title:
                    del self._case_cache[case_id]
        return success
    
    def remove_case_by_id(self, case_id: int) -> bool:
        """Remove a case by ID (convenience method)"""
        case = self.get_case(case_id)
        if case:
            return self.remove_case(case.title)
        return False
    
    def update_case(self, case_title: str, case_description: str, status: CaseStatus, priority: CasePriority) -> bool:
        """Update a case using its title and new parameters"""
        return self._engine.update_case(case_title, case_description, status.value, priority.value)
    
    def update_case_obj(self, case: Case) -> bool:
        """Update a case using case object (convenience method)"""
        return self.update_case(case.title, case.description, case.status, case.priority)
    
    # Suspect Management
    def create_suspect(self, name: str, background: str, story: str = "", 
                      age: int = 0, occupation: str = "Unknown") -> Optional[Suspect]:
        """Create a new suspect"""
        success = self._engine.add_suspect(name, background, story, age, occupation)
        if success:
            # Find the suspect that was just created
            all_suspects = self._engine.get_all_suspects()
            for native_suspect in all_suspects:
                if (native_suspect.get_name() == name and 
                    native_suspect.get_background() == background):
                    suspect = Suspect(native_suspect)
                    self._suspect_cache[suspect.id] = suspect
                    return suspect
        return None
    
    def get_suspect(self, suspect_id: int) -> Optional[Suspect]:
        """Get suspect by ID"""
        if suspect_id in self._suspect_cache:
            return self._suspect_cache[suspect_id]
        
        native_suspect = self._engine.find_suspect_by_id(suspect_id)
        if native_suspect:
            suspect = Suspect(native_suspect)
            self._suspect_cache[suspect_id] = suspect
            return suspect
        return None
    
    def find_suspect(self, name: str) -> Optional[Suspect]:
        """Find suspect by name"""
        native_suspect = self._engine.find_suspect(name)
        if native_suspect:
            suspect = Suspect(native_suspect)
            self._suspect_cache[suspect.id] = suspect
            return suspect
        return None
    
    def get_all_suspects(self) -> List[Suspect]:
        """Get all suspects"""
        native_suspects = self._engine.get_all_suspects()
        suspects = []
        for native_suspect in native_suspects:
            suspect = Suspect(native_suspect)
            self._suspect_cache[suspect.id] = suspect
            suspects.append(suspect)
        return suspects
    
    def find_suspects_by_status(self, status: SuspectStatus) -> List[Suspect]:
        """Find suspects by status"""
        native_suspects = self._engine.find_suspects_by_status(status.value)
        suspects = []
        for native_suspect in native_suspects:
            suspect = Suspect(native_suspect)
            self._suspect_cache[suspect.id] = suspect
            suspects.append(suspect)
        return suspects
    
    def search_suspects(self, query: str) -> List[Suspect]:
        """Search suspects by query"""
        native_suspects = self._engine.search_suspects(query)
        suspects = []
        for native_suspect in native_suspects:
            suspect = Suspect(native_suspect)
            self._suspect_cache[suspect.id] = suspect
            suspects.append(suspect)
        return suspects
    
    def remove_suspect(self, suspect_name: str) -> bool:
        """Remove a suspect by name"""
        success = self._engine.remove_suspect(suspect_name)
        if success:
            # Remove from cache if present
            for suspect_id, suspect in list(self._suspect_cache.items()):
                if suspect.name == suspect_name:
                    del self._suspect_cache[suspect_id]
        return success
    
    def remove_suspect_by_id(self, suspect_id: int) -> bool:
        """Remove a suspect by ID (convenience method)"""
        suspect = self.get_suspect(suspect_id)
        if suspect:
            return self.remove_suspect(suspect.name)
        return False
    
    def update_suspect(self, suspect_name: str, suspect_background: str, suspect_story: str, age: int, occupation: str) -> bool:
        """Update a suspect using its name and new parameters"""
        return self._engine.update_suspect(suspect_name, suspect_background, suspect_story, age, occupation)
    
    def update_suspect_obj(self, suspect: Suspect) -> bool:
        """Update a suspect using suspect object (convenience method)"""
        return self.update_suspect(suspect.name, suspect.background, suspect.story, suspect.age, suspect.occupation)
    
    def get_prime_suspects(self) -> List[Suspect]:
        """Get all prime suspects"""
        native_suspects = self._engine.get_prime_suspects()
        suspects = []
        for native_suspect in native_suspects:
            suspect = Suspect(native_suspect)
            self._suspect_cache[suspect.id] = suspect
            suspects.append(suspect)
        return suspects
    
    # Character Management
    def create_character(self, name: str, role: CharacterRole, story: str = "") -> Optional[Character]:
        """Create a new character"""
        success = self._engine.add_character(name, role.value, story)
        if success:
            # Find the character that was just created
            all_characters = self._engine.get_all_characters()
            for native_character in all_characters:
                if (native_character.get_name() == name and 
                    native_character.get_role() == role.value):
                    character = Character(native_character)
                    self._character_cache[character.id] = character
                    return character
        return None
    
    def get_character(self, character_id: int) -> Optional[Character]:
        """Get character by ID"""
        if character_id in self._character_cache:
            return self._character_cache[character_id]
        
        native_character = self._engine.find_character_by_id(character_id)
        if native_character:
            character = Character(native_character)
            self._character_cache[character_id] = character
            return character
        return None
    
    def find_character(self, name: str) -> Optional[Character]:
        """Find character by name"""
        native_character = self._engine.find_character(name)
        if native_character:
            character = Character(native_character)
            self._character_cache[character.id] = character
            return character
        return None
    
    def get_all_characters(self) -> List[Character]:
        """Get all characters"""
        native_characters = self._engine.get_all_characters()
        characters = []
        for native_character in native_characters:
            character = Character(native_character)
            self._character_cache[character.id] = character
            characters.append(character)
        return characters
    
    def find_characters_by_role(self, role: CharacterRole) -> List[Character]:
        """Find characters by role"""
        native_characters = self._engine.find_characters_by_role(role.value)
        characters = []
        for native_character in native_characters:
            character = Character(native_character)
            self._character_cache[character.id] = character
            characters.append(character)
        return characters
    
    def search_characters(self, query: str) -> List[Character]:
        """Search characters by query"""
        native_characters = self._engine.search_characters(query)
        characters = []
        for native_character in native_characters:
            character = Character(native_character)
            self._character_cache[character.id] = character
            characters.append(character)
        return characters
    
    def remove_character(self, character_name: str) -> bool:
        """Remove a character by name"""
        success = self._engine.remove_character(character_name)
        if success:
            # Remove from cache if present
            for character_id, character in list(self._character_cache.items()):
                if character.name == character_name:
                    del self._character_cache[character_id]
        return success
    
    def remove_character_by_id(self, character_id: int) -> bool:
        """Remove a character by ID (convenience method)"""
        character = self.get_character(character_id)
        if character:
            return self.remove_character(character.name)
        return False
    
    def update_character(self, character_name: str, role: CharacterRole, story: str) -> bool:
        """Update a character using its name and new parameters"""
        return self._engine.update_character(character_name, role.value, story)
    
    def update_character_obj(self, character: Character) -> bool:
        """Update a character using character object (convenience method)"""
        return self.update_character(character.name, character.role, character.story)
    
    # Relationship Management
    def link_suspect_to_case(self, suspect_name: str, case_title: str) -> bool:
        """Link a suspect to a case using their names"""
        return self._engine.link_suspect_to_case(suspect_name, case_title)
    
    def unlink_suspect_from_case(self, suspect_name: str, case_title: str) -> bool:
        """Unlink a suspect from a case using their names"""
        return self._engine.unlink_suspect_from_case(suspect_name, case_title)
    
    def link_character_to_case(self, character_name: str, case_title: str) -> bool:
        """Link a character to a case using their names"""
        return self._engine.link_character_to_case(character_name, case_title)
    
    def unlink_character_from_case(self, character_name: str, case_title: str) -> bool:
        """Unlink a character from a case using their names"""
        return self._engine.unlink_character_from_case(character_name, case_title)
    
    # Convenience methods that use objects
    def link_suspect_to_case_obj(self, suspect: Suspect, case: Case) -> bool:
        """Link a suspect to a case using objects (convenience method)"""
        return self.link_suspect_to_case(suspect.name, case.title)
    
    def link_character_to_case_obj(self, character: Character, case: Case) -> bool:
        """Link a character to a case using objects (convenience method)"""
        return self.link_character_to_case(character.name, case.title)
    
    def get_suspects_for_case(self, case_title: str) -> List[Suspect]:
        """Get all suspects linked to a case using case title"""
        native_suspects = self._engine.get_suspects_for_case(case_title)
        suspects = []
        for native_suspect in native_suspects:
            suspect = Suspect(native_suspect)
            self._suspect_cache[suspect.id] = suspect
            suspects.append(suspect)
        return suspects
    
    def get_suspects_for_case_obj(self, case: Case) -> List[Suspect]:
        """Get all suspects linked to a case using case object (convenience method)"""
        return self.get_suspects_for_case(case.title)
    
    def get_characters_for_case(self, case_title: str) -> List[Character]:
        """Get all characters linked to a case using case title"""
        native_characters = self._engine.get_characters_for_case(case_title)
        characters = []
        for native_character in native_characters:
            character = Character(native_character)
            self._character_cache[character.id] = character
            characters.append(character)
        return characters
    
    def get_characters_for_case_obj(self, case: Case) -> List[Character]:
        """Get all characters linked to a case using case object (convenience method)"""
        return self.get_characters_for_case(case.title)
    
    def get_cases_for_suspect(self, suspect_name: str) -> List[Case]:
        """Get all cases linked to a suspect using suspect name"""
        native_cases = self._engine.get_cases_for_suspect(suspect_name)
        cases = []
        for native_case in native_cases:
            case = Case(native_case)
            self._case_cache[case.id] = case
            cases.append(case)
        return cases
    
    def get_cases_for_suspect_obj(self, suspect: Suspect) -> List[Case]:
        """Get all cases linked to a suspect using suspect object (convenience method)"""
        return self.get_cases_for_suspect(suspect.name)
    
    def get_cases_for_character(self, character_name: str) -> List[Case]:
        """Get all cases linked to a character using character name"""
        native_cases = self._engine.get_cases_for_character(character_name)
        cases = []
        for native_case in native_cases:
            case = Case(native_case)
            self._case_cache[case.id] = case
            cases.append(case)
        return cases
    
    def get_cases_for_character_obj(self, character: Character) -> List[Case]:
        """Get all cases linked to a character using character object (convenience method)"""
        return self.get_cases_for_character(character.name)
    
    # Analysis & Reporting
    def get_statistics(self) -> Dict[str, Any]:
        """Get engine statistics"""
        stats = self._engine.get_statistics()
        return {
            'total_cases': stats.total_cases,
            'solved_cases': stats.solved_cases,
            'open_cases': stats.open_cases,
            'total_suspects': stats.total_suspects,
            'prime_suspects': stats.prime_suspects,
            'cleared_suspects': stats.cleared_suspects,
            'total_characters': stats.total_characters,
            'witnesses': stats.witnesses,
            'detectives': stats.detectives,
            'average_suspicion_level': stats.average_suspicion_level,
            'total_relationships': stats.total_relationships
        }
    
    def get_unsolved_cases(self) -> List[Case]:
        """Get all unsolved cases"""
        native_cases = self._engine.get_unsolved_cases()
        cases = []
        for native_case in native_cases:
            case = Case(native_case)
            self._case_cache[case.id] = case
            cases.append(case)
        return cases
    
    def get_high_priority_cases(self) -> List[Case]:
        """Get all high priority cases"""
        native_cases = self._engine.get_high_priority_cases()
        cases = []
        for native_case in native_cases:
            case = Case(native_case)
            self._case_cache[case.id] = case
            cases.append(case)
        return cases
    
    # Story Manager Methods - FIXED: Correct method signatures
    def generate_case_summary(self, case_title: str) -> str:
        """Generate a summary for a case using case title"""
        return self._story_manager.generate_case_summary(case_title)

    def generate_case_summary_obj(self, case: Case) -> str:
        """Generate a summary for a case using case object (convenience method)"""
        return self.generate_case_summary(case.title)

    def generate_suspect_profile(self, suspect_name: str) -> str:
        """Generate a profile for a suspect using suspect name"""
        return self._story_manager.generate_suspect_profile(suspect_name)

    def generate_suspect_profile_obj(self, suspect: Suspect) -> str:
        """Generate a profile for a suspect using suspect object (convenience method)"""
        return self.generate_suspect_profile(suspect.name)

    def generate_character_introduction(self, character_name: str) -> str:
        """Generate an introduction for a character using character name"""
        return self._story_manager.generate_character_introduction(character_name)

    def generate_character_introduction_obj(self, character: Character) -> str:
        """Generate an introduction for a character using character object (convenience method)"""
        return self.generate_character_introduction(character.name)

    def generate_case_analysis(self, case_title: str) -> str:
        """Generate a detailed analysis for a case"""
        return self._story_manager.generate_case_analysis(case_title)

    def generate_case_analysis_obj(self, case: Case) -> str:
        """Generate a detailed analysis for a case using case object"""
        return self.generate_case_analysis(case.title)

    # FIXED: These methods don't take parameters in the C++ engine
    def generate_investigation_timeline(self) -> str:
        """Generate an investigation timeline for all cases"""
        return self._story_manager.generate_investigation_timeline()

    def generate_investigation_timeline_obj(self, case: Case) -> str:
        """Generate an investigation timeline using case object (convenience method)"""
        # Note: The C++ method doesn't take parameters, so we can't filter by case
        # This is a limitation of the current C++ implementation
        return self.generate_investigation_timeline()

    def generate_suspicion_report(self) -> str:
        """Generate a suspicion report for all suspects"""
        return self._story_manager.generate_suspicion_report()

    def suggest_next_steps(self) -> List[str]:
        """Suggest next investigation steps for all cases"""
        return self._story_manager.suggest_next_steps()

    def find_missing_connections(self) -> List[str]:
        """Find missing connections in the investigation"""
        return self._story_manager.find_missing_connections()
    
    # Utility Methods
    def validate_data(self) -> bool:
        """Validate all data in the engine"""
        return self._engine.validate_data()
    
    def display_all_data(self):
        """Display all data in the engine"""
        self._engine.display_all_data()
    
    def cleanup(self):
        """Clean up resources"""
        self._case_cache.clear()
        self._suspect_cache.clear()
        self._character_cache.clear()
    
    def __enter__(self):
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        self.cleanup()


# Utility functions
def get_engine_info() -> Dict[str, str]:
    """Get engine version information"""
    return {
        'version': getattr(engine_native, '__version__', '1.0.0'),
        'author': getattr(engine_native, '__author__', 'WhoDunnitBro Team'),
        'description': getattr(engine_native, '__description__', 'Detective Engine')
    }


# Example usage
if __name__ == "__main__":
    # Demo the engine
    with DetectiveEngine() as engine:
        print("WhoDunnit Detective Engine - Python API")
        print("=" * 50)
        
        # Create a case
        case = engine.create_case(
            title="The Mysterious Disappearance",
            description="A prominent businessman has vanished without a trace",
            status=CaseStatus.OPEN,
            priority=CasePriority.HIGH
        )
        
        if case:
            print(f"✓ Case created: {case.title}")
            
            # Set additional properties
            case.location = "City Business District"
            case.notes = "Last seen leaving office at 8 PM"
            
            # Create a suspect
            suspect = engine.create_suspect(
                name="John Smith",
                background="Business rival",
                story="Had financial disputes with the victim",
                age=45,
                occupation="Business Owner"
            )
            
            if suspect:
                print(f"✓ Suspect created: {suspect.name}")
                
                # Link suspect to case using string names
                if engine.link_suspect_to_case(suspect.name, case.title):
                    print("✓ Suspect linked to case")
                
                # Create a character
                witness = engine.create_character(
                    name="Robert Wilson",
                    role=CharacterRole.WITNESS,
                    story="Saw the victim arguing with someone on the day of disappearance"
                )
                
                if witness:
                    print(f"✓ Character created: {witness.name}")
                    
                    # Link character to case using string names
                    if engine.link_character_to_case(witness.name, case.title):
                        print("✓ Character linked to case")
                    
                    # Generate reports using object convenience methods
                    print(f"\nCase Summary:\n{engine.generate_case_summary_obj(case)}")
                    
                    # Get statistics
                    stats = engine.get_statistics()
                    print(f"\nEngine Statistics:")
                    print(f"Total Cases: {stats['total_cases']}")
                    print(f"Total Suspects: {stats['total_suspects']}")
                    print(f"Total Characters: {stats['total_characters']}")
        else:
            print("✗ Failed to create case")