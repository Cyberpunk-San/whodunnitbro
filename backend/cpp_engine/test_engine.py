# advanced_demo.py
import whodunnit_engine as wd

def run_advanced_demo():
    engine = wd.DetectiveEngine()
    
    # Create a complex mystery
    print("🕵️ Creating 'The Mysterious Affair'...")
    
    # Main case
    engine.add_case(
        "The Mysterious Affair at Ravenswood Manor",
        "Wealthy industrialist Lord Blackwood found dead in his study",
        wd.CaseStatus.IN_PROGRESS,
        wd.CasePriority.HIGH
    )
    
    # Suspects with motives
    suspects = [
        ("Lady Eleanor Blackwood", "Widow", "Stood to inherit fortune", 42, "Socialite"),
        ("James Sterling", "Business partner", "Heavy debts and business disputes", 55, "Businessman"),
        ("Victoria Reed", "Personal secretary", "Knew all his secrets", 32, "Secretary"),
        ("Henry the Butler", "Long-time servant", "Recently fired after 20 years of service", 68, "Butler")
    ]
    
    for name, background, story, age, occupation in suspects:
        engine.add_suspect(name, background, story, age, occupation)
        engine.link_suspect_to_case(name, "The Mysterious Affair at Ravenswood Manor")
    
    # Investigative team
    characters = [
        ("Detective Inspector Mills", wd.CharacterRole.DETECTIVE, "Lead investigator from Scotland Yard"),
        ("Dr. Evelyn Cross", wd.CharacterRole.EXPERT, "Forensic pathologist"),
        ("Thomas the Gardener", wd.CharacterRole.WITNESS, "Saw suspicious figures near the manor"),
        ("Madame Zora", wd.CharacterRole.INFORMANT, "Local fortune teller with connections")
    ]
    
    for name, role, story in characters:
        engine.add_character(name, role, story)
        engine.link_character_to_case(name, "The Mysterious Affair at Ravenswood Manor")
    
    # Generate comprehensive reports
    story_manager = wd.StoryManager(engine)
    
    print("\n" + "="*50)
    print("📊 INVESTIGATION OVERVIEW")
    print("="*50)
    
    stats = engine.get_statistics()
    print(f"Cases: {stats.total_cases} | Suspects: {stats.total_suspects} | Characters: {stats.total_characters}")
    
    print("\n🔍 CASE NETWORK:")
    engine.display_case_network("The Mysterious Affair at Ravenswood Manor")
    
    print("\n📖 CASE ANALYSIS:")
    analysis = story_manager.generate_case_analysis("The Mysterious Affair at Ravenswood Manor")
    print(analysis)
    
    print("\n🎭 CHARACTER INTRODUCTIONS:")
    for char in engine.get_all_characters():
        intro = story_manager.generate_character_introduction(char.get_name())
        print(f"- {intro}")
    
    print("\n💡 INVESTIGATION SUGGESTIONS:")
    suggestions = story_manager.suggest_next_steps()
    for suggestion in suggestions:
        print(f"- {suggestion}")

if __name__ == "__main__":
    run_advanced_demo()