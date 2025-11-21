"""
Frontend Server - Serves the WhoDunnit frontend on port 8000
"""
import http.server
import socketserver
import webbrowser
import os

PORT = 8000
FRONTEND_DIR = os.path.dirname(os.path.abspath(__file__))

class FrontendHandler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=FRONTEND_DIR, **kwargs)
    
    def log_message(self, format, *args):
        # Custom log format
        print(f"🌐 Frontend - {self.address_string()} - {format % args}")

def start_frontend():
    os.chdir(FRONTEND_DIR)
    
    with socketserver.TCPServer(("", PORT), FrontendHandler) as httpd:
        print(f"🎨 Frontend server running on http://localhost:{PORT}")
        print(f"🔗 Backend API: http://localhost:5000")
        print("📖 Story generation endpoints available")
        print("Press Ctrl+C to stop the server")
        
        # Open browser automatically
        webbrowser.open(f'http://localhost:{PORT}')
        
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\n🛑 Frontend server stopped")

if __name__ == "__main__":
    start_frontend()