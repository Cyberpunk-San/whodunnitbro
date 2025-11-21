"""
Engine Context Manager - FIXED VERSION
Provides a clean interface for managing the detective engine lifecycle.
"""

import os
import sys
from typing import Optional, Dict, Any
from whodunnit import DetectiveEngine, Case, Suspect, Character, CaseStatus, CasePriority, CharacterRole

# Global engine instance for persistence
_global_engine: Optional[DetectiveEngine] = None

class EngineContext:
    """Context manager for the detective engine - FIXED to use persistent engine"""
    
    def __init__(self, auto_cleanup: bool = False):  # Changed to False to persist
        self.engine = None
        self.auto_cleanup = auto_cleanup
        self._is_initialized = False
        
    def __enter__(self):
        """Use global engine instance"""
        global _global_engine
        try:
            if _global_engine is None:
                _global_engine = DetectiveEngine()
                print("🔍 Global Detective Engine Initialized")
            self.engine = _global_engine
            self._is_initialized = True
            return self.engine
        except Exception as e:
            print(f"❌ Failed to initialize engine: {e}")
            raise
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        """Don't cleanup by default to persist data"""
        if self.auto_cleanup and self.engine:
            try:
                self.engine.cleanup()
                print("🔍 Detective Engine Shutdown")
            except Exception as e:
                print(f"⚠ Warning during engine cleanup: {e}")
        
        # Don't suppress exceptions
        return False
    
    def is_initialized(self) -> bool:
        """Check if engine is properly initialized"""
        return self._is_initialized and self.engine is not None

def get_global_engine() -> Optional[DetectiveEngine]:
    """Get the global engine instance"""
    return _global_engine

def initialize_global_engine() -> bool:
    """Initialize the global engine instance"""
    global _global_engine
    try:
        if _global_engine is None:
            _global_engine = DetectiveEngine()
            print("🌍 Global Detective Engine Initialized")
        return True
    except Exception as e:
        print(f"❌ Failed to initialize global engine: {e}")
        return False

def shutdown_global_engine():
    """Shutdown the global engine instance"""
    global _global_engine
    if _global_engine:
        try:
            _global_engine.cleanup()
            _global_engine = None
            print("🌍 Global Detective Engine Shutdown")
        except Exception as e:
            print(f"⚠ Warning during global engine shutdown: {e}")

def reset_global_engine():
    """Reset the global engine (for testing)"""
    global _global_engine
    if _global_engine:
        _global_engine.cleanup()
        _global_engine = None
    return initialize_global_engine()