// Game state
const gameState = {
    currentScreen: 'mainMenu',
    currentLocation: null,
    player: {
        x: 1.5,
        y: 1.5,
        angle: 0,
        items: 0,
        alive: true,
        fov: Math.PI / 3
    },
    map: [],
    items: [],
    enemies: [],
    keys: {},
    mouse: { x: 0, y: 0 },
    raycastDistance: 16,
    showMiniMap: false,
    lastEnemyMoveTime: 0,
    enemyMoveInterval: 500, 
    lastRandomMoveTime: 0,
    randomMoveInterval: 2000 
};

// Canvas and context
const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');
const miniMapCanvas = document.getElementById('miniMapCanvas');
const miniMapCtx = miniMapCanvas.getContext('2d');

// Set canvas size
function resizeCanvas() {
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
    miniMapCanvas.width = 200;
    miniMapCanvas.height = 200;
}
resizeCanvas();
window.addEventListener('resize', resizeCanvas);

// Location definitions
const LOCATIONS = {
    mansion: {
        name: "Haunted Mansion",
        items: ["Bloody Knife", "Mysterious Note", "Hidden Key", "Secret Document"],
        enemies: ["The Butler", "The Gardener"],
        wallColor: "#8b4513",
        floorColor: "#654321",
        ceilingColor: "#5d4037"
    },
    garden: {
        name: "Mysterious Garden",
        items: ["Broken Locket", "Torn Letter", "Gardening Tool", "Strange Seed"],
        enemies: ["The Groundskeeper", "The Stranger"],
        wallColor: "#2d5a27",
        floorColor: "#4a7c59",
        ceilingColor: "#87ceeb"
    },
    library: {
        name: "Ancient Library",
        items: ["Old Diary", "Coded Message", "Rare Book", "Library Card"],
        enemies: ["The Librarian", "The Scholar"],
        wallColor: "#5d4037",
        floorColor: "#8d6e63",
        ceilingColor: "#3e2723"
    },
    dungeon: {
        name: "Forgotten Dungeon",
        items: ["Rusty Key", "Prisoner's Note", "Old Shackles", "Hidden Map"],
        enemies: ["The Jailer", "The Prisoner"],
        wallColor: "#37474f",
        floorColor: "#455a64",
        ceilingColor: "#263238"
    }
};

// Loading functions
function showLoading(message = 'Loading...') {
    const loadingElement = document.getElementById('loadingSpinner');
    const loadingText = document.getElementById('loadingText');
    if (loadingElement && loadingText) {
        loadingText.textContent = message;
        loadingElement.style.display = 'flex';
    }
}

function hideLoading() {
    const loadingElement = document.getElementById('loadingSpinner');
    if (loadingElement) {
        loadingElement.style.display = 'none';
    }
}

// Initialize the game
document.addEventListener('DOMContentLoaded', init);

function init() {
    setupEventListeners();
    createMap();
    checkForLoadedCase();
}

function checkForLoadedCase() {
    const caseId = localStorage.getItem("selectedCase");
    const caseInfo = document.getElementById('caseInfo');
    const caseDetails = document.getElementById('caseDetails');
    
    if (caseId) {
        caseInfo.style.display = 'block';
        caseDetails.textContent = `Case ID: ${caseId} - Click START to play`;
        fetchCaseDetails(caseId);
        
        // Check if this case was just solved
        const caseUpdated = localStorage.getItem('caseUpdated');
        if (caseUpdated) {
            caseDetails.textContent += ' (Recently solved!)';
            localStorage.removeItem('caseUpdated'); // Clear the flag
        }
    } else {
        caseInfo.style.display = 'none';
    }
}

async function fetchCaseDetails(caseId) {
    try {
        const response = await fetch(`${API_BASE}/api/cases`);
        if (response.ok) {
            const caseData = await response.json();
            document.getElementById('caseDetails').textContent = 
                `Case: "${caseData.title}" - Location: ${caseData.location || 'Unknown'}`;
        }
    } catch (error) {
        console.error('Failed to fetch case details:', error);
    }
}

function setupEventListeners() {
    // Menu buttons
    document.getElementById('startBtn').addEventListener('click', () => {
        const caseId = localStorage.getItem("selectedCase");
        
        if (caseId) {
            // Case is loaded from detective engine - start game directly
            showScreen('gameUI');
            startGame(); // This will load the case location automatically
        } else {
            // No case selected - show location selection
            showScreen('locationSelect');
        }
    });
    
    document.getElementById('backToMenu').addEventListener('click', () => {
        showScreen('mainMenu');
        checkForLoadedCase(); // Refresh case info display
    });
    
    // Update the restart button event listener
    document.getElementById('restartBtn').addEventListener('click', () => {
        resetGame();
        localStorage.removeItem("selectedCase");
        showScreen('locationSelect');
    });
        
    // Location selection
    document.querySelectorAll('.btn-location').forEach(btn => {
        btn.addEventListener('click', (e) => {
            const location = e.target.getAttribute('data-location');
            startGame(location);
        });
    });
    
    // Map toggle
    document.getElementById('mapToggle').addEventListener('click', () => {
        gameState.showMiniMap = !gameState.showMiniMap;
        const miniMap = document.getElementById('miniMap');
        const mapToggle = document.getElementById('mapToggle');
        
        if (gameState.showMiniMap) {
            miniMap.classList.remove('hidden');
            mapToggle.textContent = 'Hide Map';
        } else {
            miniMap.classList.add('hidden');
            mapToggle.textContent = 'Show Map';
        }
    });
    
    // Keyboard controls
    document.addEventListener('keydown', e => {
        gameState.keys[e.key.toLowerCase()] = true;
        
        // Space to interact
        if (e.key === ' ' && gameState.currentScreen === 'gameUI') {
            checkInteraction();
        }
        
        // M to toggle map
        if (e.key === 'm' && gameState.currentScreen === 'gameUI') {
            gameState.showMiniMap = !gameState.showMiniMap;
            const miniMap = document.getElementById('miniMap');
            const mapToggle = document.getElementById('mapToggle');
            
            if (gameState.showMiniMap) {
                miniMap.classList.remove('hidden');
                mapToggle.textContent = 'Hide Map';
            } else {
                miniMap.classList.add('hidden');
                mapToggle.textContent = 'Show Map';
            }
        }
    });
    
    document.addEventListener('keyup', e => {
        gameState.keys[e.key.toLowerCase()] = false;
    });
    
    // Mouse look
    document.addEventListener('mousemove', (e) => {
        if (gameState.currentScreen === 'gameUI') {
            const sensitivity = 0.002;
            gameState.player.angle += e.movementX * sensitivity;
            
            // Limit vertical look (simplified)
            gameState.mouse.y = e.clientY / canvas.height;
        }
    });
    
    // Lock pointer for first-person controls
    canvas.addEventListener('click', () => {
        if (gameState.currentScreen === 'gameUI') {
            canvas.requestPointerLock();
        }
    });
}

function showScreen(screenName) {
    // Hide all screens
    document.querySelectorAll('.screen').forEach(screen => {
        screen.classList.add('hidden');
    });
    
    // Show the requested screen
    document.getElementById(screenName).classList.remove('hidden');
    
    // Special cases
    if (screenName === 'gameUI') {
        document.getElementById('gameUI').classList.remove('hidden');
        document.getElementById('crosshair').style.display = 'block';
        document.getElementById('mapToggle').style.display = 'block';
    } else {
        document.getElementById('crosshair').style.display = 'none';
        document.getElementById('mapToggle').style.display = 'none';
        document.getElementById('miniMap').classList.add('hidden');
        document.getElementById('dangerAlert').style.display = 'none';
    }
    
    gameState.currentScreen = screenName;
}

function startGame(location = null) {
    if (location) {
        // Manual location selection
        gameState.currentLocation = location;
        initializeGameWithLocation();
    } else {
        // Load from case data - this will handle the automatic start
        loadCaseLocationData();
    }
}

async function loadCaseLocationData() {
    try {
        const caseId = localStorage.getItem("selectedCase");
        
        if (!caseId) {
            console.error('No case ID found in localStorage');
            showScreen('locationSelect');
            return;
        }

        showLoading('Loading case data from Detective Engine...');
        
        const response = await fetch(`http://localhost:5000/api/cases/${caseId}`);
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        const caseData = await response.json();
        
        if (caseData && caseData.location) {
            // Enhanced location mapping
            const locationMap = {
                'garden': 'garden',
                'mansion': 'mansion', 
                'library': 'library',
                'dungeon': 'dungeon',
                'mysterious garden': 'garden',
                'haunted mansion': 'mansion',
                'ancient library': 'library', 
                'forgotten dungeon': 'dungeon',
                // Add more variations
                'the garden': 'garden',
                'the mansion': 'mansion',
                'the library': 'library',
                'the dungeon': 'dungeon'
            };
            
            const locationLower = caseData.location.toLowerCase().trim();
            const gameLocation = locationMap[locationLower] || 'dungeon';
            
            console.log('Detected location:', caseData.location, '->', gameLocation);
            
            gameState.currentLocation = gameLocation;
            
            hideLoading();
            initializeGameWithLocation();
            
        } else {
            throw new Error('No location data found in case');
        }
        
    } catch (error) {
        console.error('Failed to load case location:', error);
        hideLoading();
        
        // Fallback: show location selection screen
        showScreen('locationSelect');
        const locationInfo = document.getElementById('locationInfo');
        if (locationInfo) {
            locationInfo.textContent = 'Failed to load case location. Please select a location manually. Error: ' + error.message;
        }
    }
}

function initializeGameWithLocation() {
    console.log('Initializing game with location:', gameState.currentLocation);
    
    if (!gameState.currentLocation) {
        console.error('No location set! Defaulting to mansion');
        gameState.currentLocation = 'mansion';
    }
    
    // Update UI with location info
    const locationInfo = LOCATIONS[gameState.currentLocation];
    if (!locationInfo) {
        console.error('Invalid location:', gameState.currentLocation, 'Defaulting to mansion');
        gameState.currentLocation = 'mansion';
    }
    
    document.getElementById('locationName').textContent = `Location: ${LOCATIONS[gameState.currentLocation].name}`;
    document.getElementById('itemsCount').textContent = `Clues Collected: 0/4`;
    document.getElementById('enemiesCount').textContent = `Enemies Remaining: 2`;
    
    // Initialize game state
    resetGameState();
    
    // Create map with random item/enemy placement
    createMap();
    
    // Start game loop
    gameLoop();
}

function resetGameState() {
    gameState.player = {
        x: 1.5,
        y: 1.5,
        angle: 0,
        items: 0,
        alive: true,
        fov: Math.PI / 3
    };
    
    gameState.map = [];
    gameState.items = [];
    gameState.enemies = [];
    gameState.showMiniMap = false;
    gameState.lastEnemyMoveTime = 0;
    gameState.lastRandomMoveTime = 0;
}

function createMap() {
    // Create a simple maze-like map (1 = wall, 0 = empty space)
    gameState.map = [
        [1, 1, 1, 1, 1, 1, 1, 1, 1],
        [1, 0, 0, 0, 0, 0, 0, 0, 1],
        [1, 0, 1, 0, 1, 1, 0, 1, 1],
        [1, 0, 1, 0, 0, 0, 0, 0, 1],
        [1, 0, 1, 1, 1, 0, 1, 1, 1],
        [1, 0, 0, 0, 0, 0, 0, 0, 1],
        [1, 0, 0, 0, 0, 0, 0, 0, 1],
        [1, 0, 1, 0, 1, 1, 0, 1, 1],
        [1, 0, 1, 0, 0, 0, 0, 0, 1],
        [1, 0, 1, 1, 1, 0, 1, 0, 1],
        [1, 0, 0, 0, 0, 0, 0, 1, 1],
        [1, 0, 1, 0, 1, 1, 0, 0, 1],
        [1, 1, 1, 1, 1, 1, 1, 1, 1]
    ];
    
    // Function to get random empty position
    function getRandomEmptyPosition() {
        let x, y;
        do {
            x = Math.floor(Math.random() * (gameState.map[0].length - 2)) + 1;
            y = Math.floor(Math.random() * (gameState.map.length - 2)) + 1;
        } while (gameState.map[y][x] !== 0 || 
                (x < 1.8 && y < 1.8) || // Too close to player start
                gameState.items.some(item => Math.abs(item.x - x) < 1 && Math.abs(item.y - y) < 1));
        return { x: x + 0.5, y: y + 0.5 };
    }
    
    // Place items at random positions
    gameState.items = [];
    for (let i = 0; i < 4; i++) {
        const pos = getRandomEmptyPosition();
        gameState.items.push({
            x: pos.x,
            y: pos.y,
            collected: false,
            type: i + 1
        });
    }
    
    // Place enemies at random positions
    gameState.enemies = [];
    for (let i = 0; i < 2; i++) {
        const pos = getRandomEmptyPosition();
        gameState.enemies.push({
            x: pos.x,
            y: pos.y,
            defeated: false,
            type: i + 1,
            speed: 0.1 + (Math.random() * 0.05),
            lastSeenPlayerX: 0,
            lastSeenPlayerY: 0,
            state: 'patrol'
        });
    }
}

function updatePlayer() {
    if (!gameState.player.alive) return;
    
    const moveSpeed = 0.1;
    const rotSpeed = 0.1;
    
    // Rotation
    if (gameState.keys['arrowleft'] || gameState.keys['a']) {
        gameState.player.angle -= rotSpeed;
    }
    if (gameState.keys['arrowright'] || gameState.keys['d']) {
        gameState.player.angle += rotSpeed;
    }
    
    // Movement
    let newX = gameState.player.x;
    let newY = gameState.player.y;
    
    if (gameState.keys['arrowup'] || gameState.keys['w']) {
        newX += Math.cos(gameState.player.angle) * moveSpeed;
        newY += Math.sin(gameState.player.angle) * moveSpeed;
    }
    if (gameState.keys['arrowdown'] || gameState.keys['s']) {
        newX -= Math.cos(gameState.player.angle) * moveSpeed;
        newY -= Math.sin(gameState.player.angle) * moveSpeed;
    }
    
    // Collision detection with walls
    if (gameState.map[Math.floor(newY)][Math.floor(newX)] === 0) {
        gameState.player.x = newX;
        gameState.player.y = newY;
    }
    
    // Update enemies
    updateEnemies();
    
    // Check for interactions with items and enemies
    checkProximity();
}

function updateEnemies() {
    const currentTime = Date.now();
    
    // Move enemies periodically
    if (currentTime - gameState.lastEnemyMoveTime > gameState.enemyMoveInterval) {
        gameState.lastEnemyMoveTime = currentTime;
        
        for (const enemy of gameState.enemies) {
            if (!enemy.defeated) {
                moveEnemy(enemy);
            }
        }
    }
    
    // Random enemy state changes
    if (currentTime - gameState.lastRandomMoveTime > gameState.randomMoveInterval) {
        gameState.lastRandomMoveTime = currentTime;
        
        for (const enemy of gameState.enemies) {
            if (!enemy.defeated && Math.random() < 0.3) { // 30% chance
                enemy.state = 'random';
            }
        }
    }
}

function moveEnemy(enemy) {
    const playerX = gameState.player.x;
    const playerY = gameState.player.y;
    const distanceToPlayer = Math.sqrt(
        Math.pow(enemy.x - playerX, 2) + 
        Math.pow(enemy.y - playerY, 2)
    );
    
    // danger alert if enemy is close
    const dangerAlert = document.getElementById('dangerAlert');
    if (distanceToPlayer < 3) {
        dangerAlert.style.display = 'block';
    } else {
        dangerAlert.style.display = 'none';
    }
    
    // Enemy AI behavior
    if (distanceToPlayer < 5) {
        // Chase player if close enough
        enemy.state = 'chase';
        enemy.lastSeenPlayerX = playerX;
        enemy.lastSeenPlayerY = playerY;
        
        // Move toward player
        const angleToPlayer = Math.atan2(playerY - enemy.y, playerX - enemy.x);
        const newX = enemy.x + Math.cos(angleToPlayer) * enemy.speed;
        const newY = enemy.y + Math.sin(angleToPlayer) * enemy.speed;
        
        // Check if movement is valid
        if (gameState.map[Math.floor(newY)][Math.floor(newX)] === 0) {
            enemy.x = newX;
            enemy.y = newY;
        }
    } else if (enemy.state === 'random') {
        // Random movement
        const randomAngle = Math.random() * Math.PI * 2;
        const newX = enemy.x + Math.cos(randomAngle) * enemy.speed;
        const newY = enemy.y + Math.sin(randomAngle) * enemy.speed;
        
        // Check if movement is valid
        if (gameState.map[Math.floor(newY)][Math.floor(newX)] === 0) {
            enemy.x = newX;
            enemy.y = newY;
        }
        
        // Return to patrol after random move
        if (Math.random() < 0.5) {
            enemy.state = 'patrol';
        }
    } else {
        // Patrol behavior - move toward last seen position or wander
        enemy.state = 'patrol';
        
        if (enemy.lastSeenPlayerX !== 0 && enemy.lastSeenPlayerY !== 0) {
            // Move toward last seen position
            const angleToLastSeen = Math.atan2(
                enemy.lastSeenPlayerY - enemy.y, 
                enemy.lastSeenPlayerX - enemy.x
            );
            const newX = enemy.x + Math.cos(angleToLastSeen) * enemy.speed * 0.5;
            const newY = enemy.y + Math.sin(angleToLastSeen) * enemy.speed * 0.5;
            
            if (gameState.map[Math.floor(newY)][Math.floor(newX)] === 0) {
                enemy.x = newX;
                enemy.y = newY;
            }
        } else {
            // Wander randomly
            const randomAngle = Math.random() * Math.PI * 2;
            const newX = enemy.x + Math.cos(randomAngle) * enemy.speed * 0.3;
            const newY = enemy.y + Math.sin(randomAngle) * enemy.speed * 0.3;
            
            if (gameState.map[Math.floor(newY)][Math.floor(newX)] === 0) {
                enemy.x = newX;
                enemy.y = newY;
            }
        }
    }
    
    // Check if enemy caught player
    if (distanceToPlayer < 0.8 && !enemy.defeated) {
        const location = LOCATIONS[gameState.currentLocation];
        
        if (gameState.player.items >= 4) {
            // Player can defeat enemy
            enemy.defeated = true;
            showMessage(`You defeated ${location.enemies[enemy.type - 1]}!`);
            
            // Update UI
            const remainingEnemies = gameState.enemies.filter(e => !e.defeated).length;
            document.getElementById('enemiesCount').textContent = `Enemies Remaining: ${remainingEnemies}`;
            
            // Check if all enemies defeated
            if (remainingEnemies === 0) {
                endGame(true);
            }
        } else {
            // Player killed
            showMessage(`You were killed by ${location.enemies[enemy.type - 1]}! Game Over!`);
            gameState.player.alive = false;
            endGame(false);
        }
    }
}

function checkProximity() {
    const interactionHint = document.getElementById('interactionHint');
    let showHint = false;
    let hintText = '';
    
    // Check items
    for (const item of gameState.items) {
        if (!item.collected) {
            const dist = Math.sqrt(
                Math.pow(gameState.player.x - item.x, 2) + 
                Math.pow(gameState.player.y - item.y, 2)
            );
            
            if (dist < 1.5) {
                showHint = true;
                hintText = 'Press SPACE to collect clue';
                break;
            }
        }
    }
    
    // Check enemies
    for (const enemy of gameState.enemies) {
        if (!enemy.defeated) {
            const dist = Math.sqrt(
                Math.pow(gameState.player.x - enemy.x, 2) + 
                Math.pow(gameState.player.y - enemy.y, 2)
            );
            
            if (dist < 1.5) {
                showHint = true;
                if (gameState.player.items >= 4) {
                    hintText = 'Press SPACE to confront enemy';
                } else {
                    hintText = 'DANGER: Enemy nearby!';
                }
                break;
            }
        }
    }
    
    if (showHint) {
        interactionHint.textContent = hintText;
        interactionHint.style.display = 'block';
    } else {
        interactionHint.style.display = 'none';
    }
}

function checkInteraction() {
    // Check items
    for (const item of gameState.items) {
        if (!item.collected) {
            const dist = Math.sqrt(
                Math.pow(gameState.player.x - item.x, 2) + 
                Math.pow(gameState.player.y - item.y, 2)
            );
            
            if (dist < 1.5) {
                item.collected = true;
                gameState.player.items++;
                
                // Update UI
                document.getElementById('itemsCount').textContent = `Clues Collected: ${gameState.player.items}/4`;
                
                // Show message
                const location = LOCATIONS[gameState.currentLocation];
                showMessage(`You found a clue: ${location.items[gameState.player.items - 1]}!`);
                
                // Check if all items collected
                if (gameState.player.items >= 4) {
                    showMessage("Mystery solved! You can now confront the enemies!");
                }
                return;
            }
        }
    }
}

function castRay(angle) {
    // Simplified raycasting for first-person view
    let rayX = gameState.player.x;
    let rayY = gameState.player.y;
    
    const rayDirX = Math.cos(angle);
    const rayDirY = Math.sin(angle);
    
    let distance = 0;
    let hitWall = false;
    
    while (!hitWall && distance < gameState.raycastDistance) {
        rayX += rayDirX * 0.05;
        rayY += rayDirY * 0.05;
        distance += 0.05;
        
        // Check if ray hit a wall
        if (gameState.map[Math.floor(rayY)][Math.floor(rayX)] === 1) {
            hitWall = true;
        }
        
        // Check if ray hit an item or enemy
        for (const item of gameState.items) {
            if (!item.collected && 
                Math.abs(rayX - item.x) < 0.3 && 
                Math.abs(rayY - item.y) < 0.3) {
                return { distance, hit: 'item', type: item.type };
            }
        }
        
        for (const enemy of gameState.enemies) {
            if (!enemy.defeated && 
                Math.abs(rayX - enemy.x) < 0.3 && 
                Math.abs(rayY - enemy.y) < 0.3) {
                return { distance, hit: 'enemy', type: enemy.type };
            }
        }
    }
    
    return { distance, hit: hitWall ? 'wall' : 'none' };
}

function renderFirstPerson() {
    const location = LOCATIONS[gameState.currentLocation];
    const width = canvas.width;
    const height = canvas.height;
    
    /* COMMENTED OUT - COLOR-BASED RENDERING
    // Draw ceiling
    ctx.fillStyle = location.ceilingColor;
    ctx.fillRect(0, 0, width, height / 2);
    
    // Draw floor
    ctx.fillStyle = location.floorColor;
    ctx.fillRect(0, height / 2, width, height / 2);
    */
    
    // TODO: UNCOMMENT AND IMPLEMENT IMAGE-BASED RENDERING
    // Example of how you would use images:
    /*
    // Draw ceiling texture
    if (location.ceilingTexture) {
        ctx.drawImage(location.ceilingTexture, 0, 0, width, height / 2);
    }
    
    // Draw floor texture  
    if (location.floorTexture) {
        ctx.drawImage(location.floorTexture, 0, height / 2, width, height / 2);
    }
    */
    
    // For now, keeping color-based as fallback
    ctx.fillStyle = location.ceilingColor || '#5d4037';
    ctx.fillRect(0, 0, width, height / 2);
    ctx.fillStyle = location.floorColor || '#654321';
    ctx.fillRect(0, height / 2, width, height / 2);
    
    // Raycasting
    const numRays = width;
    const fov = gameState.player.fov;
    
    for (let x = 0; x < numRays; x++) {
        const rayAngle = gameState.player.angle - fov / 2 + (x / numRays) * fov;
        const ray = castRay(rayAngle);
        
        // Calculate wall height based on distance
        const wallHeight = Math.min(height / (ray.distance + 0.0001), height);
        
        // Draw wall slice
        if (ray.hit === 'wall') {
            const brightness = Math.max(0.3, 1 - ray.distance / gameState.raycastDistance);
            
            /* COMMENTED OUT - COLOR-BASED WALLS
            ctx.fillStyle = shadeColor(location.wallColor, brightness);
            ctx.fillRect(x, (height - wallHeight) / 2, 1, wallHeight);
            */
            
            // TODO: UNCOMMENT FOR TEXTURE-BASED WALLS
            /*
            if (location.wallTexture) {
                // Calculate texture coordinates and draw textured wall slice
                const textureX = Math.floor((x / numRays) * location.wallTexture.width);
                ctx.drawImage(
                    location.wallTexture,
                    textureX, 0, 1, location.wallTexture.height,
                    x, (height - wallHeight) / 2, 1, wallHeight
                );
            }
            */
            
            // Fallback to color-based
            ctx.fillStyle = shadeColor(location.wallColor, brightness);
            ctx.fillRect(x, (height - wallHeight) / 2, 1, wallHeight);
        }
        
        // Draw item/enemy sprites
        if (ray.hit === 'item' || ray.hit === 'enemy') {
            const spriteHeight = Math.min(height / (ray.distance + 0.0001) * 0.8, height);
            const spriteY = (height - spriteHeight) / 2;
            
            if (ray.hit === 'item') {
                ctx.fillStyle = ray.distance < 3 ? '#ffeb3b' : '#b2a429';
                ctx.fillRect(x - 1, spriteY, 3, spriteHeight);
                
                // Add glow effect for nearby items
                if (ray.distance < 2) {
                    ctx.fillStyle = 'rgba(255, 235, 59, 0.3)';
                    ctx.fillRect(x - 3, spriteY - 5, 7, spriteHeight + 10);
                }
            } else if (ray.hit === 'enemy') {
                // Enemy color changes based on state
                let enemyColor;
                if (ray.distance < 3) {
                    enemyColor = '#ff0000'; // Red when close
                } else if (ray.distance < 5) {
                    enemyColor = '#ff5252'; // Medium red
                } else {
                    enemyColor = '#b33939'; // Dark red when far
                }
                
                ctx.fillStyle = enemyColor;
                ctx.fillRect(x - 1, spriteY, 3, spriteHeight);
                
                // Add danger effect for nearby enemies
                if (ray.distance < 2) {
                    ctx.fillStyle = 'rgba(255, 82, 82, 0.3)';
                    ctx.fillRect(x - 3, spriteY - 5, 7, spriteHeight + 10);
                }
            }
        }
    }
    
    // Render mini-map if enabled
    if (gameState.showMiniMap) {
        renderMiniMap();
    }
}

function renderMiniMap() {
    const mapWidth = gameState.map[0].length;
    const mapHeight = gameState.map.length;
    const cellSize = Math.min(miniMapCanvas.width / mapWidth, miniMapCanvas.height / mapHeight);
    
    // Clear mini-map
    miniMapCtx.fillStyle = 'rgba(0, 0, 0, 0.8)';
    miniMapCtx.fillRect(0, 0, miniMapCanvas.width, miniMapCanvas.height);
    
    // Calculate offset to center the map
    const offsetX = (miniMapCanvas.width - mapWidth * cellSize) / 2;
    const offsetY = (miniMapCanvas.height - mapHeight * cellSize) / 2;
    
    // Draw walls
    miniMapCtx.fillStyle = '#8b4513';
    for (let y = 0; y < mapHeight; y++) {
        for (let x = 0; x < mapWidth; x++) {
            if (gameState.map[y][x] === 1) {
                miniMapCtx.fillRect(
                    offsetX + x * cellSize,
                    offsetY + y * cellSize,
                    cellSize,
                    cellSize
                );
            }
        }
    }
    
    // Draw items
    for (const item of gameState.items) {
        if (!item.collected) {
            miniMapCtx.fillStyle = '#ffeb3b';
            miniMapCtx.beginPath();
            miniMapCtx.arc(
                offsetX + item.x * cellSize,
                offsetY + item.y * cellSize,
                cellSize / 3,
                0,
                Math.PI * 2
            );
            miniMapCtx.fill();
        }
    }
    
    // Draw enemies with state-based colors
    for (const enemy of gameState.enemies) {
        if (!enemy.defeated) {
            let enemyColor;
            switch(enemy.state) {
                case 'chase':
                    enemyColor = '#ff0000'; // Red when chasing
                    break;
                case 'random':
                    enemyColor = '#ff9900'; // Orange when moving randomly
                    break;
                default:
                    enemyColor = '#ff5252'; // Pink when patrolling
            }
            
            miniMapCtx.fillStyle = enemyColor;
            miniMapCtx.beginPath();
            miniMapCtx.arc(
                offsetX + enemy.x * cellSize,
                offsetY + enemy.y * cellSize,
                cellSize / 3,
                0,
                Math.PI * 2
            );
            miniMapCtx.fill();
            
            // Draw enemy direction line
            const enemyToPlayerAngle = Math.atan2(
                gameState.player.y - enemy.y,
                gameState.player.x - enemy.x
            );
            const directionLength = cellSize * 1.2;
            miniMapCtx.strokeStyle = enemyColor;
            miniMapCtx.lineWidth = 1;
            miniMapCtx.beginPath();
            miniMapCtx.moveTo(
                offsetX + enemy.x * cellSize,
                offsetY + enemy.y * cellSize
            );
            miniMapCtx.lineTo(
                offsetX + enemy.x * cellSize + Math.cos(enemyToPlayerAngle) * directionLength,
                offsetY + enemy.y * cellSize + Math.sin(enemyToPlayerAngle) * directionLength
            );
            miniMapCtx.stroke();
        }
    }
    
    // Draw player
    miniMapCtx.fillStyle = '#4ecca3';
    miniMapCtx.beginPath();
    miniMapCtx.arc(
        offsetX + gameState.player.x * cellSize,
        offsetY + gameState.player.y * cellSize,
        cellSize / 2,
        0,
        Math.PI * 2
    );
    miniMapCtx.fill();
    
    // Draw player direction
    const directionLength = cellSize * 1.5;
    miniMapCtx.strokeStyle = '#4ecca3';
    miniMapCtx.lineWidth = 2;
    miniMapCtx.beginPath();
    miniMapCtx.moveTo(
        offsetX + gameState.player.x * cellSize,
        offsetY + gameState.player.y * cellSize
    );
    miniMapCtx.lineTo(
        offsetX + gameState.player.x * cellSize + Math.cos(gameState.player.angle) * directionLength,
        offsetY + gameState.player.y * cellSize + Math.sin(gameState.player.angle) * directionLength
    );
    miniMapCtx.stroke();
    
    // Draw mini-map border
    miniMapCtx.strokeStyle = '#4ecca3';
    miniMapCtx.lineWidth = 2;
    miniMapCtx.strokeRect(0, 0, miniMapCanvas.width, miniMapCanvas.height);
}

function shadeColor(color, percent) {
    // Simple color shading for lighting effect
    const num = parseInt(color.slice(1), 16);
    const amt = Math.round(2.55 * percent * 100);
    const R = (num >> 16) + amt;
    const G = (num >> 8 & 0x00FF) + amt;
    const B = (num & 0x0000FF) + amt;
    return "#" + (0x1000000 + (R < 255 ? R < 1 ? 0 : R : 255) * 0x10000 +
        (G < 255 ? G < 1 ? 0 : G : 255) * 0x100 +
        (B < 255 ? B < 1 ? 0 : B : 255)).toString(16).slice(1);
}

function showMessage(message) {
    const messageElement = document.getElementById('gameMessage');
    messageElement.textContent = message;
    messageElement.style.display = 'block';
    
    setTimeout(() => {
        messageElement.style.display = 'none';
    }, 3000);
}

function endGame(victory) {
    const gameOverScreen = document.getElementById('gameOver');
    const messageElement = document.getElementById('gameOverMessage');
    
    if (victory) {
        messageElement.textContent = "Congratulations! You solved the murder mystery!";
        messageElement.style.color = "#4ecca3";
    } else {
        messageElement.textContent = "You were killed before solving the mystery!";
        messageElement.style.color = "#e94560";
    }
    
    showScreen('gameOver');
}

function resetGame() {
    // Reset game state
    gameState.player = {
        x: 1.5,
        y: 1.5,
        angle: 0,
        items: 0,
        alive: true,
        fov: Math.PI / 3
    };
    
    gameState.map = [];
    gameState.items = [];
    gameState.enemies = [];
}

function gameLoop() {
    updatePlayer();
    renderFirstPerson();
    
    if (gameState.currentScreen === 'gameUI') {
        requestAnimationFrame(gameLoop);
    }
}
async function endGame(victory) {
    const gameOverScreen = document.getElementById('gameOver');
    const messageElement = document.getElementById('gameOverMessage');
    
    if (victory) {
        const location = LOCATIONS[gameState.currentLocation];
        messageElement.textContent = "Congratulations! You solved the murder mystery!";
        messageElement.style.color = "#4ecca3";
        
        // Update the case status in the backend with detailed information
        await updateCaseStatus('SOLVED', {
            location: location.name,
            cluesFound: gameState.player.items,
            enemiesDefeated: gameState.enemies.filter(e => e.defeated).length,
            completionTime: new Date().toISOString()
        });
    } else {
        messageElement.textContent = "You were killed before solving the mystery!";
        messageElement.style.color = "#e94560";
    }
    
    showScreen('gameOver');
}

// Enhanced function to update case status with game details
async function updateCaseStatus(status, gameDetails = {}) {
    try {
        const caseId = localStorage.getItem("selectedCase");
        
        if (!caseId) {
            console.log('No case ID found - skipping status update');
            return;
        }

        // Create solution text based on game details
        let solutionText = "Mystery solved through investigation in the 3D game environment.";
        
        if (gameDetails.cluesFound) {
            solutionText += ` Collected ${gameDetails.cluesFound} clues.`;
        }
        if (gameDetails.enemiesDefeated) {
            solutionText += ` Defeated ${gameDetails.enemiesDefeated} enemies.`;
        }
        if (gameDetails.location) {
            solutionText += ` Investigation conducted at ${gameDetails.location}.`;
        }

        const updateData = {
            status: status,
            solution: solutionText,
            notes: `Case completed in 3D game on ${new Date().toLocaleString()}. ${solutionText}`
        };

        const response = await fetch(`http://localhost:5000/api/cases/${caseId}`, {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(updateData)
        });

        if (response.ok) {
            console.log('Case status updated to:', status);
            
            // Update the game over message
            const gameOverMessage = document.getElementById('gameOverMessage');
            if (gameOverMessage) {
                const originalMessage = gameOverMessage.textContent;
                gameOverMessage.textContent = originalMessage + " ✓ Case marked as SOLVED in Detective Engine!";
            }
            
            // Refresh the case data in the main app if it's open
            notifyMainApp();
        } else {
            console.error('Failed to update case status:', response.status);
        }
    } catch (error) {
        console.error('Error updating case status:', error);
    }
}

// Function to notify the main app that the case was updated
function notifyMainApp() {
    // This could be used to refresh the main app if it's open in another tab
    if (window.opener) {
        // If the game was opened from the main app
        window.opener.postMessage({ type: 'caseUpdated', caseId: localStorage.getItem("selectedCase") }, '*');
    }
    
    // Also update localStorage to signal the main app
    localStorage.setItem('caseUpdated', Date.now().toString());

}

