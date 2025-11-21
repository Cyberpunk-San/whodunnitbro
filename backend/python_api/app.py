"""
WhoDunnit Detective Engine - API Only
Complete version with all story generation endpoints
Runs on port 5000 for frontend consumption
"""

import os
import sys
from flask import Flask, request, jsonify, send_from_directory
from flask_cors import CORS  
from typing import Dict, Any, List, Optional

sys.path.append(os.path.dirname(os.path.abspath(__file__)))

try:
    from whodunnit import (
        DetectiveEngine, Case, Suspect, Character, 
        CaseStatus, CasePriority, CharacterRole, SuspectStatus, AlibiStrength
    )
    from engine_context import initialize_global_engine, get_global_engine
    ENGINE_AVAILABLE = True
except ImportError as e:
    print(f"❌ Engine import failed: {e}")
    ENGINE_AVAILABLE = False

# Create Flask app for API only
app = Flask(__name__)
CORS(app)  # Enable CORS for frontend communication
app.config['SECRET_KEY'] = 'whodunnit-secret-key-2024'

# Initialize global engine on startup
if ENGINE_AVAILABLE:
    engine_initialized = initialize_global_engine()
    if engine_initialized:
        print("✅ Global engine initialized successfully")
    else:
        print("❌ Failed to initialize global engine")
else:
    engine_initialized = False
    print("❌ Engine not available")

# Use this helper function for all engine operations
def with_engine(func):
    """Decorator to provide engine instance to route functions"""
    def wrapper(*args, **kwargs):
        if not ENGINE_AVAILABLE:
            return jsonify({"error": "Engine not available"}), 500
        
        engine = get_global_engine()
        if not engine:
            return jsonify({"error": "Engine not initialized"}), 500
            
        return func(engine, *args, **kwargs)
    
    wrapper.__name__ = func.__name__
    return wrapper

# ========== API ROUTES ==========

@app.route('/api')
def api_home():
    """API home page"""
    return jsonify({
        "message": "WhoDunnit Detective Engine API",
        "version": "1.0.0",
        "endpoints": {
            "cases": "/api/cases",
            "suspects": "/api/suspects", 
            "characters": "/api/characters",
            "analysis": "/api/analysis/overview",
            "search": "/api/search?q=query",
            "demo": "/api/demo/setup (POST)",
            "story_generation": {
                "case_summary": "/api/story/case/<id>/summary",
                "case_analysis": "/api/story/case/<id>/analysis", 
                "case_timeline": "/api/story/case/<id>/timeline",
                "suspect_profile": "/api/story/suspect/<id>/profile",
                "character_intro": "/api/story/character/<id>/introduction",
                "suspicion_report": "/api/story/suspicion-report",
                "next_steps": "/api/story/next-steps",
                "missing_connections": "/api/story/missing-connections"
            }
        }
    })

# ========== CORE CRUD ENDPOINTS ==========

@app.route('/api/cases', methods=['GET', 'POST'])
@with_engine
def api_cases(engine):
    """API endpoint for cases"""
    if request.method == 'GET':
        cases = engine.get_all_cases()
        return jsonify([case.to_dict() for case in cases])
    
    elif request.method == 'POST':
        data = request.get_json()
        if not data:
            return jsonify({"error": "No JSON data provided"}), 400
        
        case = engine.create_case(
            title=data.get('title', ''),
            description=data.get('description', ''),
            status=CaseStatus[data.get('status', 'OPEN')],
            priority=CasePriority[data.get('priority', 'MEDIUM')]
        )
        
        if case:
            return jsonify(case.to_dict()), 201
        else:
            return jsonify({"error": "Failed to create case"}), 400

@app.route('/api/cases/<int:case_id>', methods=['GET', 'PUT', 'DELETE'])
@with_engine
def api_case_detail(engine, case_id):
    """API endpoint for specific case"""
    case = engine.get_case(case_id)
    if not case:
        return jsonify({"error": "Case not found"}), 404
    
    if request.method == 'GET':
        return jsonify(case.to_dict())
    
    elif request.method == 'PUT':
        data = request.get_json()
        if not data:
            return jsonify({"error": "No JSON data provided"}), 400
        
        # Update case properties
        if 'title' in data:
            case.title = data['title']
        if 'description' in data:
            case.description = data['description']
        if 'status' in data:
            case.status = CaseStatus[data['status']]
        if 'priority' in data:
            case.priority = CasePriority[data['priority']]
        if 'location' in data:
            case.location = data['location']
        if 'notes' in data:
            case.notes = data['notes']
        if 'solution' in data:
            case.solution = data['solution']
        
        success = engine.update_case_obj(case)
        if success:
            return jsonify(case.to_dict())
        else:
            return jsonify({"error": "Failed to update case"}), 400
    
    elif request.method == 'DELETE':
        success = engine.remove_case_by_id(case_id)
        if success:
            return jsonify({"message": "Case deleted successfully"})
        else:
            return jsonify({"error": "Failed to delete case"}), 400

@app.route('/api/suspects', methods=['GET', 'POST'])
@with_engine
def api_suspects(engine):
    """API endpoint for suspects"""
    if request.method == 'GET':
        suspects = engine.get_all_suspects()
        return jsonify([suspect.to_dict() for suspect in suspects])
    
    elif request.method == 'POST':
        data = request.get_json()
        if not data:
            return jsonify({"error": "No JSON data provided"}), 400
        
        suspect = engine.create_suspect(
            name=data.get('name', ''),
            background=data.get('background', ''),
            story=data.get('story', ''),
            age=data.get('age', 0),
            occupation=data.get('occupation', 'Unknown')
        )
        
        if suspect:
            return jsonify(suspect.to_dict()), 201
        else:
            return jsonify({"error": "Failed to create suspect"}), 400

@app.route('/api/suspects/<int:suspect_id>', methods=['GET', 'PUT', 'DELETE'])
@with_engine
def api_suspect_detail(engine, suspect_id):
    """API endpoint for specific suspect"""
    suspect = engine.get_suspect(suspect_id)
    if not suspect:
        return jsonify({"error": "Suspect not found"}), 404
    
    if request.method == 'GET':
        return jsonify(suspect.to_dict())
    
    elif request.method == 'PUT':
        data = request.get_json()
        if not data:
            return jsonify({"error": "No JSON data provided"}), 400
        
        # Update suspect properties
        if 'name' in data:
            suspect.name = data['name']
        if 'background' in data:
            suspect.background = data['background']
        if 'story' in data:
            suspect.story = data['story']
        if 'age' in data:
            suspect.age = data['age']
        if 'occupation' in data:
            suspect.occupation = data['occupation']
        if 'motive' in data:
            suspect.motive = data['motive']
        if 'alibi' in data:
            suspect.alibi = data['alibi']
        if 'alibi_strength' in data:
            suspect.alibi_strength = AlibiStrength[data['alibi_strength']]
        if 'status' in data:
            suspect.status = SuspectStatus[data['status']]
        if 'suspicion_level' in data:
            suspect.suspicion_level = float(data['suspicion_level'])
        
        success = engine.update_suspect_obj(suspect)
        if success:
            return jsonify(suspect.to_dict())
        else:
            return jsonify({"error": "Failed to update suspect"}), 400
    
    elif request.method == 'DELETE':
        success = engine.remove_suspect_by_id(suspect_id)
        if success:
            return jsonify({"message": "Suspect deleted successfully"})
        else:
            return jsonify({"error": "Failed to delete suspect"}), 400

@app.route('/api/characters', methods=['GET', 'POST'])
@with_engine
def api_characters(engine):
    """API endpoint for characters"""
    if request.method == 'GET':
        characters = engine.get_all_characters()
        return jsonify([character.to_dict() for character in characters])
    
    elif request.method == 'POST':
        data = request.get_json()
        if not data:
            return jsonify({"error": "No JSON data provided"}), 400
        
        character = engine.create_character(
            name=data.get('name', ''),
            role=CharacterRole[data.get('role', 'OTHER')],
            story=data.get('story', '')
        )
        
        if character:
            return jsonify(character.to_dict()), 201
        else:
            return jsonify({"error": "Failed to create character"}), 400

@app.route('/api/characters/<int:character_id>', methods=['GET', 'PUT', 'DELETE'])
@with_engine
def api_character_detail(engine, character_id):
    """API endpoint for specific character"""
    character = engine.get_character(character_id)
    if not character:
        return jsonify({"error": "Character not found"}), 404
    
    if request.method == 'GET':
        return jsonify(character.to_dict())
    
    elif request.method == 'PUT':
        data = request.get_json()
        if not data:
            return jsonify({"error": "No JSON data provided"}), 400
        
        # Update character properties
        if 'name' in data:
            character.name = data['name']
        if 'role' in data:
            character.role = CharacterRole[data['role']]
        if 'story' in data:
            character.story = data['story']
        
        success = engine.update_character_obj(character)
        if success:
            return jsonify(character.to_dict())
        else:
            return jsonify({"error": "Failed to update character"}), 400
    
    elif request.method == 'DELETE':
        success = engine.remove_character_by_id(character_id)
        if success:
            return jsonify({"message": "Character deleted successfully"})
        else:
            return jsonify({"error": "Failed to delete character"}), 400

# ========== RELATIONSHIP ENDPOINTS ==========

@app.route('/api/relationships/link', methods=['POST'])
@with_engine
def api_link_entities(engine):
    """API endpoint for linking entities"""
    data = request.get_json()
    if not data:
        return jsonify({"error": "No JSON data provided"}), 400
    
    entity_type = data.get('entity_type')
    entity_name = data.get('entity_name')
    case_title = data.get('case_title')
    
    if not all([entity_type, entity_name, case_title]):
        return jsonify({"error": "Missing required fields"}), 400
    
    if entity_type == 'suspect':
        success = engine.link_suspect_to_case(entity_name, case_title)
    elif entity_type == 'character':
        success = engine.link_character_to_case(entity_name, case_title)
    else:
        return jsonify({"error": "Invalid entity type"}), 400
    
    if success:
        return jsonify({"message": f"Successfully linked {entity_name} to {case_title}"})
    else:
        return jsonify({"error": "Failed to link entities"}), 400

@app.route('/api/relationships/unlink', methods=['POST'])
@with_engine
def api_unlink_entities(engine):
    """API endpoint for unlinking entities"""
    data = request.get_json()
    if not data:
        return jsonify({"error": "No JSON data provided"}), 400
    
    entity_type = data.get('entity_type')
    entity_name = data.get('entity_name')
    case_title = data.get('case_title')
    
    if not all([entity_type, entity_name, case_title]):
        return jsonify({"error": "Missing required fields"}), 400
    
    if entity_type == 'suspect':
        success = engine.unlink_suspect_from_case(entity_name, case_title)
    elif entity_type == 'character':
        success = engine.unlink_character_from_case(entity_name, case_title)
    else:
        return jsonify({"error": "Invalid entity type"}), 400
    
    if success:
        return jsonify({"message": f"Successfully unlinked {entity_name} from {case_title}"})
    else:
        return jsonify({"error": "Failed to unlink entities"}), 400

# ========== STORY GENERATION ENDPOINTS ==========

@app.route('/api/story/case/<int:case_id>/summary')
@with_engine
def api_case_summary(engine, case_id):
    """Generate a summary for a case"""
    case = engine.get_case(case_id)
    if not case:
        return jsonify({"error": "Case not found"}), 404
    
    try:
        summary = engine.generate_case_summary_obj(case)
        return jsonify({
            "case_id": case_id,
            "case_title": case.title,
            "summary": summary
        })
    except Exception as e:
        return jsonify({"error": f"Failed to generate summary: {str(e)}"}), 500

@app.route('/api/story/case/<int:case_id>/analysis')
@with_engine
def api_case_analysis(engine, case_id):
    """Generate detailed analysis for a case"""
    case = engine.get_case(case_id)
    if not case:
        return jsonify({"error": "Case not found"}), 404
    
    try:
        analysis = engine.generate_case_analysis_obj(case)
        return jsonify({
            "case_id": case_id,
            "case_title": case.title,
            "analysis": analysis
        })
    except Exception as e:
        return jsonify({"error": f"Failed to generate analysis: {str(e)}"}), 500

@app.route('/api/story/case/<int:case_id>/timeline')
@with_engine
def api_case_timeline(engine, case_id):
    """Generate investigation timeline for a case"""
    case = engine.get_case(case_id)
    if not case:
        return jsonify({"error": "Case not found"}), 404
    
    try:
        timeline = engine.generate_investigation_timeline_obj(case)
        return jsonify({
            "case_id": case_id,
            "case_title": case.title,
            "timeline": timeline
        })
    except Exception as e:
        return jsonify({"error": f"Failed to generate timeline: {str(e)}"}), 500

@app.route('/api/story/suspect/<int:suspect_id>/profile')
@with_engine
def api_suspect_profile(engine, suspect_id):
    """Generate profile for a suspect"""
    suspect = engine.get_suspect(suspect_id)
    if not suspect:
        return jsonify({"error": "Suspect not found"}), 404
    
    try:
        profile = engine.generate_suspect_profile_obj(suspect)
        return jsonify({
            "suspect_id": suspect_id,
            "suspect_name": suspect.name,
            "profile": profile
        })
    except Exception as e:
        return jsonify({"error": f"Failed to generate profile: {str(e)}"}), 500

@app.route('/api/story/character/<int:character_id>/introduction')
@with_engine
def api_character_introduction(engine, character_id):
    """Generate introduction for a character"""
    character = engine.get_character(character_id)
    if not character:
        return jsonify({"error": "Character not found"}), 404
    
    try:
        introduction = engine.generate_character_introduction_obj(character)
        return jsonify({
            "character_id": character_id,
            "character_name": character.name,
            "introduction": introduction
        })
    except Exception as e:
        return jsonify({"error": f"Failed to generate introduction: {str(e)}"}), 500

@app.route('/api/story/suspicion-report')
@with_engine
def api_suspicion_report(engine):
    """Generate suspicion report for all suspects"""
    try:
        report = engine.generate_suspicion_report()
        return jsonify({
            "report": report,
            "total_suspects": len(engine.get_all_suspects())
        })
    except Exception as e:
        return jsonify({"error": f"Failed to generate suspicion report: {str(e)}"}), 500

@app.route('/api/story/next-steps')
@with_engine
def api_next_steps(engine):
    """Suggest next investigation steps"""
    try:
        steps = engine.suggest_next_steps()
        return jsonify({
            "next_steps": steps,
            "total_steps": len(steps)
        })
    except Exception as e:
        return jsonify({"error": f"Failed to generate next steps: {str(e)}"}), 500

@app.route('/api/story/missing-connections')
@with_engine
def api_missing_connections(engine):
    """Find missing connections in the investigation"""
    try:
        connections = engine.find_missing_connections()
        return jsonify({
            "missing_connections": connections,
            "total_missing": len(connections)
        })
    except Exception as e:
        return jsonify({"error": f"Failed to find missing connections: {str(e)}"}), 500

# ========== ANALYSIS & SEARCH ENDPOINTS ==========

@app.route('/api/analysis/case/<int:case_id>')
@with_engine
def api_case_full_analysis(engine, case_id):
    """Comprehensive analysis for a case (all story elements)"""
    case = engine.get_case(case_id)
    if not case:
        return jsonify({"error": "Case not found"}), 404
    
    try:
        analysis = {
            "case": case.to_dict(),
            "summary": engine.generate_case_summary_obj(case),
            "detailed_analysis": engine.generate_case_analysis_obj(case),
            "timeline": engine.generate_investigation_timeline_obj(case),
            "suspects": [s.to_dict() for s in engine.get_suspects_for_case_obj(case)],
            "characters": [c.to_dict() for c in engine.get_characters_for_case_obj(case)],
            "suspect_profiles": [
                {
                    "suspect_id": s.id,
                    "suspect_name": s.name,
                    "profile": engine.generate_suspect_profile_obj(s)
                } for s in engine.get_suspects_for_case_obj(case)
            ]
        }
        
        return jsonify(analysis)
    except Exception as e:
        return jsonify({"error": f"Failed to generate comprehensive analysis: {str(e)}"}), 500

@app.route('/api/analysis/overview')
@with_engine
def api_overview_analysis(engine):
    """Overview analysis for all data"""
    try:
        overview = {
            "statistics": engine.get_statistics(),
            "suspicion_report": engine.generate_suspicion_report(),
            "next_steps": engine.suggest_next_steps(),
            "missing_connections": engine.find_missing_connections()
        }
        
        return jsonify(overview)
    except Exception as e:
        return jsonify({"error": f"Failed to generate overview analysis: {str(e)}"}), 500

@app.route('/api/search')
@with_engine
def api_search(engine):
    """Search across all entities"""
    query = request.args.get('q', '')
    if not query:
        return jsonify({"error": "No search query provided"}), 400
    
    try:
        results = {
            "query": query,
            "cases": [case.to_dict() for case in engine.search_cases(query)],
            "suspects": [suspect.to_dict() for suspect in engine.search_suspects(query)],
            "characters": [character.to_dict() for character in engine.search_characters(query)]
        }
        
        return jsonify(results)
    except Exception as e:
        return jsonify({"error": f"Search failed: {str(e)}"}), 500

# ========== UTILITY ENDPOINTS ==========

@app.route('/api/demo/setup', methods=['POST'])
@with_engine
def setup_demo_data(engine):
    """Setup demo data for testing"""
    try:
        # Clear existing data first
        cases = engine.get_all_cases()
        for case in cases:
            engine.remove_case_by_id(case.id)
        
        suspects = engine.get_all_suspects()
        for suspect in suspects:
            engine.remove_suspect_by_id(suspect.id)
        
        characters = engine.get_all_characters()
        for character in characters:
            engine.remove_character_by_id(character.id)
        
        # Create demo cases
        case1 = engine.create_case(
            "The Museum Heist",
            "Priceless diamond stolen during gala event",
            CaseStatus.OPEN,
            CasePriority.HIGH
        )
        
        case2 = engine.create_case(
            "Bank Embezzlement", 
            "Internal funds misappropriation",
            CaseStatus.IN_PROGRESS,
            CasePriority.URGENT
        )
        
        # Create demo suspects
        suspect1 = engine.create_suspect(
            "Vincent Moreau",
            "Known art thief with previous convictions",
            "Specializes in high-value heists",
            52,
            "Professional Thief"
        )
        
        suspect2 = engine.create_suspect(
            "Isabella Chen",
            "Event planner with access to security",
            "Financial difficulties recently",
            38,
            "Event Coordinator"
        )
        
        # Create demo characters
        detective = engine.create_character(
            "Detective Parker",
            CharacterRole.DETECTIVE,
            "Lead investigator with 15 years experience"
        )
        
        expert = engine.create_character(
            "Dr. Evans",
            CharacterRole.EXPERT,
            "Forensic specialist and gemology expert"
        )
        
        # Link entities
        if case1 and suspect1:
            engine.link_suspect_to_case_obj(suspect1, case1)
        if case1 and suspect2:
            engine.link_suspect_to_case_obj(suspect2, case1)
        if case1 and detective:
            engine.link_character_to_case_obj(detective, case1)
        if case1 and expert:
            engine.link_character_to_case_obj(expert, case1)
        
        return jsonify({
            "message": "Demo data created successfully",
            "cases_created": 2,
            "suspects_created": 2,
            "characters_created": 2,
            "links_created": 4
        })
    except Exception as e:
        return jsonify({"error": f"Failed to setup demo data: {str(e)}"}), 500

# Health check
@app.route('/api/health')
def health_check():
    """Health check endpoint"""
    status = {
        "status": "healthy" if ENGINE_AVAILABLE else "unhealthy",
        "engine_available": ENGINE_AVAILABLE,
        "engine_initialized": engine_initialized,
        "global_engine_exists": get_global_engine() is not None
    }
    return jsonify(status)

# Reset endpoint for testing
@app.route('/api/reset', methods=['POST'])
def reset_engine():
    """Reset the engine (for testing)"""
    if not ENGINE_AVAILABLE:
        return jsonify({"error": "Engine not available"}), 500
    
    try:
        from engine_context import reset_global_engine
        success = reset_global_engine()
        
        if success:
            return jsonify({"message": "Engine reset successfully"})
        else:
            return jsonify({"error": "Failed to reset engine"}), 500
    except Exception as e:
        return jsonify({"error": f"Reset failed: {str(e)}"}), 500

# Error handlers
@app.errorhandler(404)
def not_found_error(error):
    return jsonify({"error": "Endpoint not found"}), 404

@app.errorhandler(500)
def internal_error(error):
    return jsonify({"error": "Internal server error"}), 500

@app.errorhandler(405)
def method_not_allowed(error):
    return jsonify({"error": "Method not allowed"}), 405

FRONTEND_DIR = os.path.join(os.path.dirname(__file__), "../../frontend")

@app.route("/")
def index():
    return send_from_directory(FRONTEND_DIR, "index.html")

@app.route("/<path:path>")
def static_files(path):
    # serve frontend files ONLY if the path has no /api prefix
    if not path.startswith("api"):
        return send_from_directory(FRONTEND_DIR, path)
    
    return jsonify({"error": "API endpoint not found"}), 404


if __name__ == '__main__':
    print("🚀 Starting WhoDunnit Detective Engine API...")
    print(f"🔍 Engine Available: {ENGINE_AVAILABLE}")
    print(f"🔍 Engine Initialized: {engine_initialized}")
    print("🌐 API running on port 5000")
    print("📡 Ready for frontend connections on port 8000")
    print("📖 Story generation endpoints available at /api/story/*")
    
    if ENGINE_AVAILABLE and engine_initialized:
        print("✅ Detective engine loaded successfully!")
        print("📊 Using PERSISTENT engine instance")
    else:
        print("❌ Detective engine not available - running in limited mode")
    
    port = int(os.environ.get("PORT", 5000))

    app.run(host="0.0.0.0", port=port)

