"""
Probe data validation to understand why it's failing
"""

import whodunnit_engine as native

def probe_validation():
    print("Probing Data Validation")
    print("=" * 40)
    
    # Create engine
    engine = native.DetectiveEngine()
    
    # Create some test data
    engine.add_case("Test Case", "Test Description", native.CaseStatus.OPEN, native.CasePriority.MEDIUM)
    engine.add_suspect("Test Suspect", "Test Background")
    engine.add_character("Test Character", native.CharacterRole.WITNESS)
    
    # Test validation
    print("Testing data validation...")
    is_valid = engine.validate_data()
    print(f"Validation result: {is_valid}")
    
    if not is_valid:
        print("Getting data issues...")
        try:
            issues = engine.get_data_issues()
            print(f"Data issues type: {type(issues)}")
            if isinstance(issues, list):
                print(f"Number of issues: {len(issues)}")
                for i, issue in enumerate(issues, 1):
                    print(f"  {i}. {issue}")
            else:
                print(f"Issues: {issues}")
        except Exception as e:
            print(f"Could not get data issues: {e}")
    
    # Test with linked data
    print("\nTesting with linked data...")
    engine.link_suspect_to_case("Test Suspect", "Test Case")
    engine.link_character_to_case("Test Character", "Test Case")
    
    is_valid_linked = engine.validate_data()
    print(f"Validation with links: {is_valid_linked}")
    
    if not is_valid_linked:
        try:
            issues = engine.get_data_issues()
            if isinstance(issues, list):
                print(f"Linked data issues: {len(issues)}")
                for i, issue in enumerate(issues, 1):
                    print(f"  {i}. {issue}")
        except Exception as e:
            print(f"Could not get linked data issues: {e}")

if __name__ == "__main__":
    probe_validation()