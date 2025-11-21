
const API_BASE = 'http://localhost:5000';

// Global state
let currentData = {
    cases: [],
    suspects: [],
    characters: [],
    statistics: null
};

// initialize application
document.addEventListener('DOMContentLoaded', function() {
    initializeApp();
});

async function initializeApp() {
    showLoading('Initializing application...');
    
    await testApiConnection();
    
    await refreshAllData();
    
    setupEventListeners();
    
    hideLoading();
}

function setupEventListeners() {
    // link/unlink form submissions
    document.getElementById('linkForm').addEventListener('submit', function(e) {
        e.preventDefault();
        linkEntities();
    });
    
    document.getElementById('unlinkForm').addEventListener('submit', function(e) {
        e.preventDefault();
        unlinkEntities();
    });
    
    // entity type changes for link forms
    document.getElementById('linkEntityType').addEventListener('change', updateLinkEntityOptions);
    document.getElementById('unlinkEntityType').addEventListener('change', updateUnlinkEntityOptions);
}

// api Connection and Status
async function testApiConnection() {
    try {
        const response = await fetch(`${API_BASE}/health`);
        const data = await response.json();
        
        const statusElement = document.getElementById('engineStatus');
        if (data.status === 'healthy' && data.engine_available) {
            statusElement.className = 'badge bg-success';
            statusElement.innerHTML = '<i class="fas fa-check"></i> Engine Connected & Healthy';
        } else {
            statusElement.className = 'badge bg-danger';
            statusElement.innerHTML = '<i class="fas fa-exclamation-triangle"></i> Engine Issues';
        }
        
        return data;
    } catch (error) {
        document.getElementById('engineStatus').className = 'badge bg-danger';
        document.getElementById('engineStatus').innerHTML = '<i class="fas fa-times"></i> Cannot Connect to API';
        throw error;
    }
}

// Data Management
async function refreshAllData() {
    showLoading('Refreshing all data...');
    
    try {
        await Promise.all([
            loadCases(),
            loadSuspects(),
            loadCharacters(),
            loadStatistics()
        ]);
        
        updateDropdowns();
        updateDashboard();
        
        addActivity('All data refreshed successfully');
    } catch (error) {
        showError('Failed to refresh data: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function loadCases() {
    try {
        const response = await fetch(`${API_BASE}/api/cases`);
        currentData.cases = await response.json();
        displayCases(currentData.cases);
        updateCaseCount();
        return currentData.cases;
    } catch (error) {
        showError('Failed to load cases: ' + error.message);
        return [];
    }
}

async function loadSuspects() {
    try {
        const response = await fetch(`${API_BASE}/api/suspects`);
        currentData.suspects = await response.json();
        displaySuspects(currentData.suspects);
        updateSuspectCount();
        return currentData.suspects;
    } catch (error) {
        showError('Failed to load suspects: ' + error.message);
        return [];
    }
}

async function loadCharacters() {
    try {
        const response = await fetch(`${API_BASE}/api/characters`);
        currentData.characters = await response.json();
        displayCharacters(currentData.characters);
        updateCharacterCount();
        return currentData.characters;
    } catch (error) {
        showError('Failed to load characters: ' + error.message);
        return [];
    }
}

async function loadStatistics() {
    try {
        const response = await fetch(`${API_BASE}/api/analysis/overview`);
        const data = await response.json();
        currentData.statistics = data.statistics;
        return data;
    } catch (error) {
        console.error('Failed to load statistics:', error);
        return null;
    }
}

function displaySuspects(suspects) {
    const container = document.getElementById('suspectsList');
    if (!container) return;
    
    if (suspects.length === 0) {
        container.innerHTML = '<div class="col-12 text-center text-muted">No suspects found.</div>';
        return;
    }
    
    container.innerHTML = suspects.map(suspect => `
        <div class="col-md-6 col-lg-4">
            <div class="card entity-card" onclick="viewSuspectDetail(${suspect.id})">
                <div class="card-body">
                    <h6 class="card-title">${suspect.name}</h6>
                    <p class="card-text small">${suspect.occupation || 'Unknown occupation'} • ${suspect.age || 'Unknown age'}</p>
                    <div class="d-flex justify-content-between align-items-center">
                        <span class="badge ${getSuspectStatusBadgeClass(suspect.status)}">${suspect.status}</span>
                        <span class="badge ${getAlibiStrengthBadgeClass(suspect.alibi_strength)}">${suspect.alibi_strength}</span>
                    </div>
                    <div class="mt-2 small">
                        Suspicion: ${suspect.suspicion_level || 0}% • 
                        ${suspect.is_prime_suspect ? '<i class="fas fa-star text-warning"></i> Prime' : ''}
                    </div>
                </div>
            </div>
        </div>
    `).join('');
}

function displayCharacters(characters) {
    const container = document.getElementById('charactersList');
    if (!container) return;
    
    if (characters.length === 0) {
        container.innerHTML = '<div class="col-12 text-center text-muted">No characters found.</div>';
        return;
    }
    
    container.innerHTML = characters.map(character => `
        <div class="col-md-6 col-lg-4">
            <div class="card entity-card" onclick="viewCharacterDetail(${character.id})">
                <div class="card-body">
                    <h6 class="card-title">${character.name}</h6>
                    <p class="card-text small">${character.role}</p>
                    <span class="badge ${getRoleBadgeClass(character.role)}">${character.role}</span>
                    <div class="mt-2 small text-muted">
                        ${character.story ? character.story.substring(0, 80) + (character.story.length > 80 ? '...' : '') : 'No description'}
                    </div>
                </div>
            </div>
        </div>
    `).join('');
}

// Creation Functions
async function createCase() {
    const form = document.getElementById('createCaseForm');
    const formData = new FormData(form);
    const data = Object.fromEntries(formData);
    
    try {
        showLoading('Creating case...');
        const response = await fetch(`${API_BASE}/api/cases`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data)
        });
        
        if (response.ok) {
            const newCase = await response.json();
            addActivity(`Case created: "${newCase.title}"`);
            $('#createCaseModal').modal('hide');
            form.reset();
            await refreshAllData();
            showSuccess('Case created successfully!');
        } else {
            throw new Error('Failed to create case');
        }
    } catch (error) {
        showError('Failed to create case: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function createSuspect() {
    const form = document.getElementById('createSuspectForm');
    const formData = new FormData(form);
    const data = Object.fromEntries(formData);
    
    const processedData = {
        name: data.name,
        background: data.background,
        story: data.story || '',
        age: data.age ? parseInt(data.age) : 0,
        occupation: data.occupation || 'Unknown'
    };
    
    try {
        showLoading('Creating suspect...');
        const response = await fetch(`${API_BASE}/api/suspects`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(processedData)
        });
        
        if (response.ok) {
            const newSuspect = await response.json();
            addActivity(`Suspect created: "${newSuspect.name}"`);
            $('#createSuspectModal').modal('hide');
            form.reset();
            await refreshAllData();
            showSuccess('Suspect created successfully!');
        } else {
            const errorData = await response.json();
            throw new Error(errorData.error || 'Failed to create suspect');
        }
    } catch (error) {
        showError('Failed to create suspect: ' + error.message);
    } finally {
        hideLoading();
    }
}
// Edit/Update Func
async function editCase(caseId) {
    const caseItem = currentData.cases.find(c => c.id === caseId);
    if (!caseItem) {
        showError('Case not found');
        return;
    }

    console.log('Editing case:', caseItem);

    document.getElementById('editCaseId').value = caseItem.id;
    document.getElementById('editCaseTitle').value = caseItem.title;
    document.getElementById('editCaseDescription').value = caseItem.description;
    document.getElementById('editCaseStatus').value = caseItem.status;
    document.getElementById('editCasePriority').value = caseItem.priority;
    document.getElementById('editCaseLocation').value = caseItem.location || '';
    document.getElementById('editCaseNotes').value = caseItem.notes || '';
    document.getElementById('editCaseSolution').value = caseItem.solution || '';

    const editModal = new bootstrap.Modal(document.getElementById('editCaseModal'));
    editModal.show();
}

async function updateCase() {
    const caseId = document.getElementById('editCaseId').value;
    if (!caseId) {
        showError('No case ID found');
        return;
    }

    const data = {
        title: document.getElementById('editCaseTitle').value,
        description: document.getElementById('editCaseDescription').value,
        status: document.getElementById('editCaseStatus').value,
        priority: document.getElementById('editCasePriority').value,
        location: document.getElementById('editCaseLocation').value,
        notes: document.getElementById('editCaseNotes').value,
        solution: document.getElementById('editCaseSolution').value
    };

    try {
        showLoading('Updating case...');
        const response = await fetch(`${API_BASE}/api/cases/${caseId}`, {
            method: 'PUT',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data)
        });

        if (response.ok) {
            const updatedCase = await response.json();
            addActivity(`Case updated: "${updatedCase.title}"`);
            
            const editModal = bootstrap.Modal.getInstance(document.getElementById('editCaseModal'));
            editModal.hide();
            
            await refreshAllData();
            showSuccess('Case updated successfully!');
        } else {
            const errorData = await response.json();
            throw new Error(errorData.error || 'Failed to update case');
        }
    } catch (error) {
        showError('Failed to update case: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function editSuspect(suspectId) {
    const suspect = currentData.suspects.find(s => s.id === suspectId);
    if (!suspect) {
        showError('Suspect not found');
        return;
    }

    console.log('Editing suspect:', suspect);

    document.getElementById('editSuspectId').value = suspect.id;
    document.getElementById('editSuspectName').value = suspect.name;
    document.getElementById('editSuspectAge').value = suspect.age || '';
    document.getElementById('editSuspectOccupation').value = suspect.occupation || '';
    document.getElementById('editSuspectBackground').value = suspect.background;
    document.getElementById('editSuspectStory').value = suspect.story || '';
    document.getElementById('editSuspectMotive').value = suspect.motive || '';
    document.getElementById('editSuspectAlibi').value = suspect.alibi || '';
    document.getElementById('editSuspectAlibiStrength').value = suspect.alibi_strength || 'NONE';
    document.getElementById('editSuspectStatus').value = suspect.status || 'UNINVESTIGATED';
    document.getElementById('editSuspectSuspicionLevel').value = suspect.suspicion_level || 0;

    const editModal = new bootstrap.Modal(document.getElementById('editSuspectModal'));
    editModal.show();
}

async function updateSuspect() {
    const suspectId = document.getElementById('editSuspectId').value;
    if (!suspectId) {
        showError('No suspect ID found');
        return;
    }

    const data = {
        name: document.getElementById('editSuspectName').value,
        background: document.getElementById('editSuspectBackground').value,
        story: document.getElementById('editSuspectStory').value,
        age: document.getElementById('editSuspectAge').value ? parseInt(document.getElementById('editSuspectAge').value) : 0,
        occupation: document.getElementById('editSuspectOccupation').value,
        motive: document.getElementById('editSuspectMotive').value,
        alibi: document.getElementById('editSuspectAlibi').value,
        alibi_strength: document.getElementById('editSuspectAlibiStrength').value,
        status: document.getElementById('editSuspectStatus').value,
        suspicion_level: parseFloat(document.getElementById('editSuspectSuspicionLevel').value) || 0
    };

    try {
        showLoading('Updating suspect...');
        const response = await fetch(`${API_BASE}/api/suspects/${suspectId}`, {
            method: 'PUT',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data)
        });

        if (response.ok) {
            const updatedSuspect = await response.json();
            addActivity(`Suspect updated: "${updatedSuspect.name}"`);
            
            const editModal = bootstrap.Modal.getInstance(document.getElementById('editSuspectModal'));
            editModal.hide();
            
            await refreshAllData();
            showSuccess('Suspect updated successfully!');
        } else {
            const errorData = await response.json();
            throw new Error(errorData.error || 'Failed to update suspect');
        }
    } catch (error) {
        showError('Failed to update suspect: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function editCharacter(characterId) {
    const character = currentData.characters.find(c => c.id === characterId);
    if (!character) {
        showError('Character not found');
        return;
    }

    console.log('Editing character:', character); // Debug log

    document.getElementById('editCharacterId').value = character.id;
    document.getElementById('editCharacterName').value = character.name;
    document.getElementById('editCharacterRole').value = character.role;
    document.getElementById('editCharacterStory').value = character.story || '';

    const editModal = new bootstrap.Modal(document.getElementById('editCharacterModal'));
    editModal.show();
}

async function updateCharacter() {
    const characterId = document.getElementById('editCharacterId').value;
    if (!characterId) {
        showError('No character ID found');
        return;
    }

    const data = {
        name: document.getElementById('editCharacterName').value,
        role: document.getElementById('editCharacterRole').value,
        story: document.getElementById('editCharacterStory').value
    };

    try {
        showLoading('Updating character...');
        const response = await fetch(`${API_BASE}/api/characters/${characterId}`, {
            method: 'PUT',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data)
        });

        if (response.ok) {
            const updatedCharacter = await response.json();
            addActivity(`Character updated: "${updatedCharacter.name}"`);
            
            const editModal = bootstrap.Modal.getInstance(document.getElementById('editCharacterModal'));
            editModal.hide();
            
            await refreshAllData();
            showSuccess('Character updated successfully!');
        } else {
            const errorData = await response.json();
            throw new Error(errorData.error || 'Failed to update character');
        }
    } catch (error) {
        showError('Failed to update character: ' + error.message);
    } finally {
        hideLoading();
    }
}
async function createCharacter() {
    const form = document.getElementById('createCharacterForm');
    const formData = new FormData(form);
    const data = Object.fromEntries(formData);
    
    try {
        showLoading('Creating character...');
        const response = await fetch(`${API_BASE}/api/characters`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data)
        });
        
        if (response.ok) {
            const newCharacter = await response.json();
            addActivity(`Character created: "${newCharacter.name}"`);
            $('#createCharacterModal').modal('hide');
            form.reset();
            await refreshAllData();
            showSuccess('Character created successfully!');
        } else {
            throw new Error('Failed to create character');
        }
    } catch (error) {
        showError('Failed to create character: ' + error.message);
    } finally {
        hideLoading();
    }
}

// relationship management
async function linkEntities() {
    const entityType = document.getElementById('linkEntityType').value;
    const entityName = document.getElementById('linkEntityName').value;
    const caseTitle = document.getElementById('linkCaseTitle').value;
    
    if (!entityType || !entityName || !caseTitle) {
        showError('Please fill all fields');
        return;
    }
    
    try {
        showLoading('Linking entities...');
        const response = await fetch(`${API_BASE}/api/relationships/link`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ entity_type: entityType, entity_name: entityName, case_title: caseTitle })
        });
        
        if (response.ok) {
            addActivity(`Linked ${entityType} "${entityName}" to case "${caseTitle}"`);
            document.getElementById('linkForm').reset();
            await refreshAllData();
            showSuccess('Entities linked successfully!');
        } else {
            throw new Error('Failed to link entities');
        }
    } catch (error) {
        showError('Failed to link entities: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function unlinkEntities() {
    const entityType = document.getElementById('unlinkEntityType').value;
    const entityName = document.getElementById('unlinkEntityName').value;
    const caseTitle = document.getElementById('unlinkCaseTitle').value;
    
    if (!entityType || !entityName || !caseTitle) {
        showError('Please fill all fields');
        return;
    }
    
    try {
        showLoading('Unlinking entities...');
        const response = await fetch(`${API_BASE}/api/relationships/unlink`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ entity_type: entityType, entity_name: entityName, case_title: caseTitle })
        });
        
        if (response.ok) {
            addActivity(`Unlinked ${entityType} "${entityName}" from case "${caseTitle}"`);
            document.getElementById('unlinkForm').reset();
            await refreshAllData();
            showSuccess('Entities unlinked successfully!');
        } else {
            throw new Error('Failed to unlink entities');
        }
    } catch (error) {
        showError('Failed to unlink entities: ' + error.message);
    } finally {
        hideLoading();
    }
}

// stry gen func
async function generateCaseSummary() {
    const caseId = document.getElementById('storyCaseSelect').value;
    if (!caseId) {
        showError('Please select a case');
        return;
    }
    
    try {
        showLoading('Generating case summary...');
        const response = await fetch(`${API_BASE}/api/story/case/${caseId}/summary`);
        const data = await response.json();
        
        if (response.ok) {
            displayStoryOutput(`CASE SUMMARY: ${data.case_title}\n\n${data.summary}`);
            addActivity(`Generated summary for case: "${data.case_title}"`);
        } else {
            throw new Error(data.error || 'Failed to generate summary');
        }
    } catch (error) {
        showError('Failed to generate case summary: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function generateCaseAnalysis() {
    const caseId = document.getElementById('storyCaseSelect').value;
    if (!caseId) {
        showError('Please select a case');
        return;
    }
    
    try {
        showLoading('Generating case analysis...');
        const response = await fetch(`${API_BASE}/api/story/case/${caseId}/analysis`);
        const data = await response.json();
        
        if (response.ok) {
            displayStoryOutput(`CASE ANALYSIS: ${data.case_title}\n\n${data.analysis}`);
            addActivity(`Generated analysis for case: "${data.case_title}"`);
        } else {
            throw new Error(data.error || 'Failed to generate analysis');
        }
    } catch (error) {
        showError('Failed to generate case analysis: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function generateCaseTimeline() {
    const caseId = document.getElementById('storyCaseSelect').value;
    if (!caseId) {
        showError('Please select a case');
        return;
    }
    
    try {
        showLoading('Generating investigation timeline...');
        const response = await fetch(`${API_BASE}/api/story/case/${caseId}/timeline`);
        const data = await response.json();
        
        if (response.ok) {
            displayStoryOutput(`INVESTIGATION TIMELINE: ${data.case_title}\n\n${data.timeline}`);
            addActivity(`Generated timeline for case: "${data.case_title}"`);
        } else {
            throw new Error(data.error || 'Failed to generate timeline');
        }
    } catch (error) {
        showError('Failed to generate timeline: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function generateSuspectProfile() {
    const suspectId = document.getElementById('storySuspectSelect').value;
    if (!suspectId) {
        showError('Please select a suspect');
        return;
    }
    
    try {
        showLoading('Generating suspect profile...');
        const response = await fetch(`${API_BASE}/api/story/suspect/${suspectId}/profile`);
        const data = await response.json();
        
        if (response.ok) {
            displayStoryOutput(`SUSPECT PROFILE: ${data.suspect_name}\n\n${data.profile}`);
            addActivity(`Generated profile for suspect: "${data.suspect_name}"`);
        } else {
            throw new Error(data.error || 'Failed to generate profile');
        }
    } catch (error) {
        showError('Failed to generate suspect profile: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function generateCharacterIntroduction() {
    const characterId = document.getElementById('storyCharacterSelect').value;
    if (!characterId) {
        showError('Please select a character');
        return;
    }
    
    try {
        showLoading('Generating character introduction...');
        const response = await fetch(`${API_BASE}/api/story/character/${characterId}/introduction`);
        const data = await response.json();
        
        if (response.ok) {
            displayStoryOutput(`CHARACTER INTRODUCTION: ${data.character_name}\n\n${data.introduction}`);
            addActivity(`Generated introduction for character: "${data.character_name}"`);
        } else {
            throw new Error(data.error || 'Failed to generate introduction');
        }
    } catch (error) {
        showError('Failed to generate character introduction: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function generateSuspicionReport() {
    try {
        showLoading('Generating suspicion report...');
        const response = await fetch(`${API_BASE}/api/story/suspicion-report`);
        const data = await response.json();
        
        if (response.ok) {
            displayStoryOutput(`SUSPICION REPORT\n\nTotal Suspects: ${data.total_suspects}\n\n${data.report}`);
            addActivity('Generated suspicion report');
        } else {
            throw new Error(data.error || 'Failed to generate suspicion report');
        }
    } catch (error) {
        showError('Failed to generate suspicion report: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function generateNextSteps() {
    try {
        showLoading('Generating next steps...');
        const response = await fetch(`${API_BASE}/api/story/next-steps`);
        const data = await response.json();
        
        if (response.ok) {
            const steps = data.next_steps.map((step, index) => `${index + 1}. ${step}`).join('\n');
            displayStoryOutput(`NEXT INVESTIGATION STEPS\n\nTotal Steps: ${data.total_steps}\n\n${steps}`);
            addActivity('Generated next investigation steps');
        } else {
            throw new Error(data.error || 'Failed to generate next steps');
        }
    } catch (error) {
        showError('Failed to generate next steps: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function generateMissingConnections() {
    try {
        showLoading('Finding missing connections...');
        const response = await fetch(`${API_BASE}/api/story/missing-connections`);
        const data = await response.json();
        
        if (response.ok) {
            const connections = data.missing_connections.map((conn, index) => `${index + 1}. ${conn}`).join('\n');
            displayStoryOutput(`MISSING CONNECTIONS\n\nTotal Missing: ${data.total_missing}\n\n${connections}`);
            addActivity('Generated missing connections report');
        } else {
            throw new Error(data.error || 'Failed to find missing connections');
        }
    } catch (error) {
        showError('Failed to find missing connections: ' + error.message);
    } finally {
        hideLoading();
    }
}

// Analysis Functions
async function performSearch() {
    const query = document.getElementById('searchQuery').value.trim();
    if (!query) {
        showError('Please enter a search term');
        return;
    }
    
    try {
        showLoading('Searching...');
        const response = await fetch(`${API_BASE}/api/search?q=${encodeURIComponent(query)}`);
        const data = await response.json();
        
        if (response.ok) {
            displaySearchResults(data);
            addActivity(`Searched for: "${query}"`);
        } else {
            throw new Error(data.error || 'Search failed');
        }
    } catch (error) {
        showError('Search failed: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function generateFullCaseAnalysis() {
    const caseId = document.getElementById('analysisCaseSelect').value;
    if (!caseId) {
        showError('Please select a case');
        return;
    }
    
    try {
        showLoading('Generating comprehensive analysis...');
        const response = await fetch(`${API_BASE}/api/analysis/case/${caseId}`);
        const data = await response.json();
        
        if (response.ok) {
            displayFullCaseAnalysis(data);
            addActivity(`Generated full analysis for case: "${data.case.title}"`);
        } else {
            throw new Error(data.error || 'Failed to generate analysis');
        }
    } catch (error) {
        showError('Failed to generate case analysis: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function generateSystemOverview() {
    try {
        showLoading('Generating system overview...');
        const response = await fetch(`${API_BASE}/api/analysis/overview`);
        const data = await response.json();
        
        if (response.ok) {
            displaySystemOverview(data);
            addActivity('Generated system overview');
        } else {
            throw new Error(data.error || 'Failed to generate overview');
        }
    } catch (error) {
        showError('Failed to generate system overview: ' + error.message);
    } finally {
        hideLoading();
    }
}

// Utility Functions
function updateDropdowns() {
    updateSelectOptions('storyCaseSelect', currentData.cases, 'id', 'title');
    updateSelectOptions('storySuspectSelect', currentData.suspects, 'id', 'name');
    updateSelectOptions('storyCharacterSelect', currentData.characters, 'id', 'name');
    updateSelectOptions('analysisCaseSelect', currentData.cases, 'id', 'title');
    updateSelectOptions('playCaseSelect', currentData.cases, 'id', 'title');
    
    updateSelectOptions('linkCaseTitle', currentData.cases, 'title', 'title');
    updateSelectOptions('unlinkCaseTitle', currentData.cases, 'title', 'title');
}

function updateLinkEntityOptions() {
    const type = document.getElementById('linkEntityType').value;
    const select = document.getElementById('linkEntityName');
    
    if (type === 'suspect') {
        updateSelectOptions('linkEntityName', currentData.suspects, 'name', 'name');
    } else if (type === 'character') {
        updateSelectOptions('linkEntityName', currentData.characters, 'name', 'name');
    } else {
        select.innerHTML = '<option value="">Select entity...</option>';
    }
}

function updateUnlinkEntityOptions() {
    const type = document.getElementById('unlinkEntityType').value;
    const select = document.getElementById('unlinkEntityName');
    
    if (type === 'suspect') {
        updateSelectOptions('unlinkEntityName', currentData.suspects, 'name', 'name');
    } else if (type === 'character') {
        updateSelectOptions('unlinkEntityName', currentData.characters, 'name', 'name');
    } else {
        select.innerHTML = '<option value="">Select entity...</option>';
    }
}

function updateSelectOptions(selectId, items, valueKey, textKey) {
    const select = document.getElementById(selectId);
    if (!select) return;
    
    const currentValue = select.value;
    select.innerHTML = '<option value="">Select...</option>';
    
    items.forEach(item => {
        const option = document.createElement('option');
        option.value = item[valueKey];
        option.textContent = item[textKey];
        select.appendChild(option);
    });
    
    if (currentValue) {
        select.value = currentValue;
    }
}

function updateDashboard() {
    document.getElementById('statCases').textContent = currentData.cases.length;
    document.getElementById('statSuspects').textContent = currentData.suspects.length;
    document.getElementById('statCharacters').textContent = currentData.characters.length;
    
    const solvedCases = currentData.cases.filter(c => c.status === 'SOLVED').length;
    document.getElementById('statSolved').textContent = solvedCases;
    
    document.getElementById('caseCount').textContent = `Cases: ${currentData.cases.length}`;
    document.getElementById('suspectCount').textContent = `Suspects: ${currentData.suspects.length}`;
    document.getElementById('characterCount').textContent = `Characters: ${currentData.characters.length}`;
}

function updateCaseCount() {
    document.getElementById('caseCount').textContent = `Cases: ${currentData.cases.length}`;
    document.getElementById('statCases').textContent = currentData.cases.length;
}

function updateSuspectCount() {
    document.getElementById('suspectCount').textContent = `Suspects: ${currentData.suspects.length}`;
    document.getElementById('statSuspects').textContent = currentData.suspects.length;
}

function updateCharacterCount() {
    document.getElementById('characterCount').textContent = `Characters: ${currentData.characters.length}`;
    document.getElementById('statCharacters').textContent = currentData.characters.length;
}

// search func
function searchCases() {
    const query = document.getElementById('caseSearch').value.toLowerCase();
    const filteredCases = currentData.cases.filter(caseItem => 
        caseItem.title.toLowerCase().includes(query) || 
        caseItem.description.toLowerCase().includes(query)
    );
    displayCases(filteredCases);
}

function searchSuspects() {
    const query = document.getElementById('suspectSearch').value.toLowerCase();
    const filteredSuspects = currentData.suspects.filter(suspect => 
        suspect.name.toLowerCase().includes(query) || 
        suspect.occupation.toLowerCase().includes(query) ||
        suspect.background.toLowerCase().includes(query)
    );
    displaySuspects(filteredSuspects);
}

function searchCharacters() {
    const query = document.getElementById('characterSearch').value.toLowerCase();
    const filteredCharacters = currentData.characters.filter(character => 
        character.name.toLowerCase().includes(query) || 
        character.role.toLowerCase().includes(query) ||
        character.story.toLowerCase().includes(query)
    );
    displayCharacters(filteredCharacters);
}

// Helper Functions
function displayStoryOutput(content) {
    document.getElementById('storyOutput').textContent = content;
}

function clearStoryOutput() {
    document.getElementById('storyOutput').textContent = 'Select a story generation option to see the output here.';
}

function displaySearchResults(results) {
    const container = document.getElementById('searchResults');
    
    let html = '<div class="search-results">';
    
    if (results.cases.length > 0) {
        html += '<h6>Cases:</h6>';
        results.cases.forEach(caseItem => {
            html += `<div class="search-result-item">
                <strong>${caseItem.title}</strong><br>
                <small class="text-muted">${caseItem.description.substring(0, 100)}...</small>
            </div>`;
        });
    }
    
    if (results.suspects.length > 0) {
        html += '<h6 class="mt-3">Suspects:</h6>';
        results.suspects.forEach(suspect => {
            html += `<div class="search-result-item">
                <strong>${suspect.name}</strong> - ${suspect.occupation}<br>
                <small class="text-muted">${suspect.background.substring(0, 100)}...</small>
            </div>`;
        });
    }
    
    if (results.characters.length > 0) {
        html += '<h6 class="mt-3">Characters:</h6>';
        results.characters.forEach(character => {
            html += `<div class="search-result-item">
                <strong>${character.name}</strong> - ${character.role}<br>
                <small class="text-muted">${character.story.substring(0, 100)}...</small>
            </div>`;
        });
    }
    
    if (results.cases.length === 0 && results.suspects.length === 0 && results.characters.length === 0) {
        html += '<div class="text-muted">No results found.</div>';
    }
    
    html += '</div>';
    container.innerHTML = html;
}

function displayFullCaseAnalysis(data) {
    const output = document.getElementById('storyOutput');
    let content = `COMPREHENSIVE CASE ANALYSIS: ${data.case.title}\n\n`;
    content += `SUMMARY:\n${data.summary}\n\n`;
    content += `DETAILED ANALYSIS:\n${data.detailed_analysis}\n\n`;
    content += `TIMELINE:\n${data.timeline}\n\n`;
    
    if (data.suspects.length > 0) {
        content += `SUSPECTS (${data.suspects.length}):\n`;
        data.suspects.forEach(suspect => {
            content += `- ${suspect.name} (${suspect.status}, Suspicion: ${suspect.suspicion_level}%)\n`;
        });
        content += '\n';
    }
    
    if (data.characters.length > 0) {
        content += `CHARACTERS (${data.characters.length}):\n`;
        data.characters.forEach(character => {
            content += `- ${character.name} (${character.role})\n`;
        });
    }
    
    output.textContent = content;
}

function displaySystemOverview(data) {
    const container = document.getElementById('systemOverview');
    
    let html = '<div class="system-overview">';
    
    if (data.statistics) {
        html += `<h6>Statistics:</h6>
        <div class="row">
            <div class="col-6">Total Cases: ${data.statistics.total_cases}</div>
            <div class="col-6">Solved Cases: ${data.statistics.solved_cases}</div>
            <div class="col-6">Open Cases: ${data.statistics.open_cases}</div>
            <div class="col-6">Total Suspects: ${data.statistics.total_suspects}</div>
            <div class="col-6">Prime Suspects: ${data.statistics.prime_suspects}</div>
            <div class="col-6">Total Characters: ${data.statistics.total_characters}</div>
        </div>`;
    }
    
    if (data.next_steps && data.next_steps.length > 0) {
        html += `<h6 class="mt-3">Next Steps:</h6>
        <ul>`;
        data.next_steps.forEach(step => {
            html += `<li>${step}</li>`;
        });
        html += '</ul>';
    }
    
    if (data.missing_connections && data.missing_connections.length > 0) {
        html += `<h6 class="mt-3">Missing Connections:</h6>
        <ul>`;
        data.missing_connections.forEach(conn => {
            html += `<li>${conn}</li>`;
        });
        html += '</ul>';
    }
    
    html += '</div>';
    container.innerHTML = html;
}

// Badge Helper Functions
function getStatusBadgeClass(status) {
    const classes = {
        'OPEN': 'bg-primary',
        'IN_PROGRESS': 'bg-info',
        'SOLVED': 'bg-success',
        'COLD': 'bg-secondary',
        'UNSOLVED': 'bg-warning'
    };
    return classes[status] || 'bg-secondary';
}

function getPriorityBadgeClass(priority) {
    const classes = {
        'LOW': 'bg-success',
        'MEDIUM': 'bg-info',
        'HIGH': 'bg-warning',
        'URGENT': 'bg-danger'
    };
    return classes[priority] || 'bg-secondary';
}

function getSuspectStatusBadgeClass(status) {
    const classes = {
        'UNINVESTIGATED': 'bg-secondary',
        'UNDER_INVESTIGATION': 'bg-warning',
        'CLEARED': 'bg-success',
        'PRIME_SUSPECT': 'bg-danger',
        'CONVICTED': 'bg-dark',
        'ACQUITTED': 'bg-info'
    };
    return classes[status] || 'bg-secondary';
}

function getAlibiStrengthBadgeClass(strength) {
    const classes = {
        'NONE': 'bg-danger',
        'WEAK': 'bg-warning',
        'MODERATE': 'bg-info',
        'STRONG': 'bg-success',
        'CONFIRMED': 'bg-success'
    };
    return classes[strength] || 'bg-secondary';
}

function getRoleBadgeClass(role) {
    const classes = {
        'WITNESS': 'bg-info',
        'INFORMANT': 'bg-primary',
        'VICTIM': 'bg-danger',
        'OFFICER': 'bg-warning',
        'DETECTIVE': 'bg-dark',
        'EXPERT': 'bg-success',
        'OTHER': 'bg-secondary'
    };
    return classes[role] || 'bg-secondary';
}

// UI Helper Functions
function showLoading(message = 'Loading...') {
    document.getElementById('loadingSpinner').style.display = 'block';
    document.getElementById('loadingText').textContent = message;
}

function hideLoading() {
    document.getElementById('loadingSpinner').style.display = 'none';
}

function showSuccess(message) {
    
    alert('Success: ' + message);
}

function showError(message) {
    
    alert('Error: ' + message);
}

function addActivity(message) {
    const activityElement = document.getElementById('recentActivity');
    const timestamp = new Date().toLocaleTimeString();
    const activityItem = `<div class="small mb-1"><i class="fas fa-circle text-success me-1" style="font-size: 0.5rem;"></i> [${timestamp}] ${message}</div>`;
    
    
    activityElement.innerHTML = activityItem + activityElement.innerHTML;
    
    const activities = activityElement.innerHTML.split('</div>').slice(0, 10);
    activityElement.innerHTML = activities.join('</div>') + (activities.length === 10 ? '' : '</div>');
}
// Delete/Remove Functions
async function deleteCase(caseId) {
    const caseItem = currentData.cases.find(c => c.id === caseId);
    if (!caseItem) return;

    if (confirm(`Are you sure you want to delete the case "${caseItem.title}"? This action cannot be undone.`)) {
        try {
            showLoading('Deleting case...');
            const response = await fetch(`${API_BASE}/api/cases/${caseId}`, {
                method: 'DELETE'
            });

            if (response.ok) {
                addActivity(`Case deleted: "${caseItem.title}"`);
                await refreshAllData();
                showSuccess('Case deleted successfully!');
            } else {
                const errorData = await response.json();
                throw new Error(errorData.error || 'Failed to delete case');
            }
        } catch (error) {
            showError('Failed to delete case: ' + error.message);
        } finally {
            hideLoading();
        }
    }
}

async function deleteSuspect(suspectId) {
    const suspect = currentData.suspects.find(s => s.id === suspectId);
    if (!suspect) return;

    if (confirm(`Are you sure you want to delete the suspect "${suspect.name}"? This action cannot be undone.`)) {
        try {
            showLoading('Deleting suspect...');
            const response = await fetch(`${API_BASE}/api/suspects/${suspectId}`, {
                method: 'DELETE'
            });

            if (response.ok) {
                addActivity(`Suspect deleted: "${suspect.name}"`);
                await refreshAllData();
                showSuccess('Suspect deleted successfully!');
            } else {
                const errorData = await response.json();
                throw new Error(errorData.error || 'Failed to delete suspect');
            }
        } catch (error) {
            showError('Failed to delete suspect: ' + error.message);
        } finally {
            hideLoading();
        }
    }
}

async function deleteCharacter(characterId) {
    const character = currentData.characters.find(c => c.id === characterId);
    if (!character) return;

    if (confirm(`Are you sure you want to delete the character "${character.name}"? This action cannot be undone.`)) {
        try {
            showLoading('Deleting character...');
            const response = await fetch(`${API_BASE}/api/characters/${characterId}`, {
                method: 'DELETE'
            });

            if (response.ok) {
                addActivity(`Character deleted: "${character.name}"`);
                await refreshAllData();
                showSuccess('Character deleted successfully!');
            } else {
                const errorData = await response.json();
                throw new Error(errorData.error || 'Failed to delete character');
            }
        } catch (error) {
            showError('Failed to delete character: ' + error.message);
        } finally {
            hideLoading();
        }
    }
}
// Demo and Testing Functions
async function setupDemoData() {
    try {
        showLoading('Setting up demo data...');
        const response = await fetch(`${API_BASE}/api/demo/setup`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        });
        
        const data = await response.json();
        
        if (response.ok) {
            await refreshAllData();
            showSuccess('Demo data created successfully!');
            addActivity('Demo data setup completed');
        } else {
            throw new Error(data.error || 'Failed to setup demo data');
        }
    } catch (error) {
        showError('Failed to setup demo data: ' + error.message);
    } finally {
        hideLoading();
    }
}

async function testAllEndpoints() {
    showLoading('Testing all API endpoints...');
    
    const endpoints = [
        '/api/cases',
        '/api/suspects',
        '/api/characters',
        '/api/analysis/overview',
        '/api/story/next-steps',
        '/api/story/suspicion-report'
    ];
    
    const results = [];
    
    for (const endpoint of endpoints) {
        try {
            const response = await fetch(`${API_BASE}${endpoint}`);
            if (response.ok) {
                results.push(`✅ ${endpoint}`);
            } else {
                results.push(`❌ ${endpoint}`);
            }
        } catch (error) {
            results.push(`❌ ${endpoint} - ${error.message}`);
        }
    }
    
    displayStoryOutput('ENDPOINT TEST RESULTS:\n\n' + results.join('\n'));
    addActivity('Tested all API endpoints');
    hideLoading();
}
// View Functions 
async function viewCaseDetail(caseId) {
    console.log('viewCaseDetail called with ID:', caseId);
    
    const caseItem = currentData.cases.find(c => c.id === caseId);
    if (!caseItem) {
        console.error('Case not found for ID:', caseId);
        showError('Case not found');
        return;
    }

    console.log('Found case:', caseItem);
    //analysis
    try {
        showLoading('Loading case details...');
        const response = await fetch(`${API_BASE}/api/analysis/case/${caseId}`);
        const caseData = await response.json();
        
        console.log('Case analysis response:', caseData);
        
        if (response.ok) {
            displayCaseDetailModal(caseData);
        } else {
            console.log('Using fallback case data');
            displayCaseDetailModal({ case: caseItem });
        }
    } catch (error) {
        console.error('Error loading case details:', error);
        displayCaseDetailModal({ case: caseItem });
    } finally {
        hideLoading();
    }
}
function displayCaseDetailModal(caseData) {
    const caseItem = caseData.case;
    
    console.log('Displaying case modal for:', caseItem);
    
    let modalContent = `
        <div class="modal-header">
            <h5 class="modal-title">🔍 Case Details: ${caseItem.title}</h5>
            <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
        </div>
        <div class="modal-body" style="max-height: 70vh; overflow-y: auto;">
            <div class="row mb-3">
                <div class="col-md-6">
                    <strong>Status:</strong> <span class="badge ${getStatusBadgeClass(caseItem.status)}">${formatStatusText(caseItem.status)}</span>
                </div>
                <div class="col-md-6">
                    <strong>Priority:</strong> <span class="badge ${getPriorityBadgeClass(caseItem.priority)}">${formatPriorityText(caseItem.priority)}</span>
                </div>
            </div>
            
            <div class="mb-3">
                <strong>Description:</strong>
                <div class="description-box mt-2">${caseItem.description}</div>
            </div>
            
            ${caseItem.location ? `
            <div class="mb-2">
                <strong>📍 Location:</strong> ${caseItem.location}
            </div>
            ` : ''}
            
            ${caseItem.notes ? `
            <div class="mb-2">
                <strong>📝 Notes:</strong>
                <p class="text-muted p-2 bg-light rounded">${caseItem.notes}</p>
            </div>
            ` : ''}
            
            ${caseItem.solution ? `
            <div class="mb-2">
                <strong>✅ Solution:</strong>
                <p class="text-success p-2 bg-light rounded">${caseItem.solution}</p>
            </div>
            ` : ''}
            
            <div class="row mt-3">
                <div class="col-md-6">
                    <strong>👥 Suspects:</strong> ${caseItem.suspect_count || 0}
                </div>
                <div class="col-md-6">
                    <strong>🎭 Characters:</strong> ${caseItem.character_count || 0}
                </div>
            </div>
            
            ${caseData.summary ? `
            <div class="mt-3">
                <strong>📊 Generated Summary:</strong>
                <div class="story-output small mt-1 p-2">${caseData.summary}</div>
            </div>
            ` : ''}
            
            ${caseData.detailed_analysis ? `
            <div class="mt-3">
                <strong>🔍 Detailed Analysis:</strong>
                <div class="story-output small mt-1 p-2">${caseData.detailed_analysis}</div>
            </div>
            ` : ''}
        </div>
        <div class="modal-footer">
            <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
            <button type="button" class="btn btn-primary" onclick="editCase(${caseItem.id})">Edit Case</button>
            <button type="button" class="btn btn-info" onclick="generateCaseStories(${caseItem.id})">Generate Stories</button>
        </div>
    `;
    
    // Create or update modal
    let modal = document.getElementById('detailModal');
    if (!modal) {
        modal = document.createElement('div');
        modal.className = 'modal fade';
        modal.id = 'detailModal';
        modal.tabIndex = -1;
        modal.setAttribute('aria-hidden', 'true');
        modal.innerHTML = `
            <div class="modal-dialog modal-lg">
                <div class="modal-content">
                    ${modalContent}
                </div>
            </div>
        `;
        document.body.appendChild(modal);
    } else {
        modal.querySelector('.modal-content').innerHTML = modalContent;
    }
    
    const detailModal = new bootstrap.Modal(modal);
    detailModal.show();
    
    console.log('Modal should be visible now');
}

async function viewSuspectDetail(suspectId) {
    const suspect = currentData.suspects.find(s => s.id === suspectId);
    if (!suspect) {
        showError('Suspect not found');
        return;
    }

    try {
        showLoading('Loading suspect profile...');
        const response = await fetch(`${API_BASE}/api/story/suspect/${suspectId}/profile`);
        const profileData = await response.json();
        
        displaySuspectDetailModal(suspect, profileData);
    } catch (error) {
        displaySuspectDetailModal(suspect, null);
    } finally {
        hideLoading();
    }
}

function displaySuspectDetailModal(suspect, profileData) {
    let modalContent = `
        <div class="modal-header">
            <h5 class="modal-title">Suspect Details: ${suspect.name}</h5>
            <button type="button" class="btn-close" data-bs-dismiss="modal"></button>
        </div>
        <div class="modal-body">
            <div class="row">
                <div class="col-md-6">
                    <strong>Age:</strong> ${suspect.age || 'Unknown'}
                </div>
                <div class="col-md-6">
                    <strong>Occupation:</strong> ${suspect.occupation || 'Unknown'}
                </div>
            </div>
            
            <div class="row mt-2">
                <div class="col-md-6">
                    <strong>Status:</strong> <span class="badge ${getSuspectStatusBadgeClass(suspect.status)}">${suspect.status}</span>
                </div>
                <div class="col-md-6">
                    <strong>Alibi Strength:</strong> <span class="badge ${getAlibiStrengthBadgeClass(suspect.alibi_strength)}">${suspect.alibi_strength}</span>
                </div>
            </div>
            
            <div class="mt-2">
                <strong>Suspicion Level:</strong> ${suspect.suspicion_level || 0}%
            </div>
            
            ${suspect.background ? `
            <div class="mt-3">
                <strong>Background:</strong>
                <p>${suspect.background}</p>
            </div>
            ` : ''}
            
            ${suspect.story ? `
            <div class="mt-2">
                <strong>Story:</strong>
                <p class="text-muted">${suspect.story}</p>
            </div>
            ` : ''}
            
            ${suspect.motive ? `
            <div class="mt-2">
                <strong>Motive:</strong>
                <p>${suspect.motive}</p>
            </div>
            ` : ''}
            
            ${suspect.alibi ? `
            <div class="mt-2">
                <strong>Alibi:</strong>
                <p class="text-muted">${suspect.alibi}</p>
            </div>
            ` : ''}
            
            ${profileData && profileData.profile ? `
            <div class="mt-3">
                <strong>Generated Profile:</strong>
                <div class="story-output small mt-1">${profileData.profile}</div>
            </div>
            ` : ''}
        </div>
        <div class="modal-footer">
            <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
            <button type="button" class="btn btn-primary" onclick="editSuspect(${suspect.id})">Edit Suspect</button>
        </div>
    `;
    
    displayDetailModal(modalContent);
}

async function viewCharacterDetail(characterId) {
    const character = currentData.characters.find(c => c.id === characterId);
    if (!character) {
        showError('Character not found');
        return;
    }

    try {
        showLoading('Loading character introduction...');
        const response = await fetch(`${API_BASE}/api/story/character/${characterId}/introduction`);
        const introData = await response.json();
        
        displayCharacterDetailModal(character, introData);
    } catch (error) {
        displayCharacterDetailModal(character, null);
    } finally {
        hideLoading();
    }
}

function displayCharacterDetailModal(character, introData) {
    let modalContent = `
        <div class="modal-header">
            <h5 class="modal-title">Character Details: ${character.name}</h5>
            <button type="button" class="btn-close" data-bs-dismiss="modal"></button>
        </div>
        <div class="modal-body">
            <div class="mb-3">
                <strong>Role:</strong> <span class="badge ${getRoleBadgeClass(character.role)}">${character.role}</span>
            </div>
            
            ${character.story ? `
            <div class="mt-2">
                <strong>Description:</strong>
                <p>${character.story}</p>
            </div>
            ` : ''}
            
            ${introData && introData.introduction ? `
            <div class="mt-3">
                <strong>Generated Introduction:</strong>
                <div class="story-output small mt-1">${introData.introduction}</div>
            </div>
            ` : ''}
        </div>
        <div class="modal-footer">
            <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
            <button type="button" class="btn btn-primary" onclick="editCharacter(${character.id})">Edit Character</button>
        </div>
    `;
    
    displayDetailModal(modalContent);
}

// Helper function for detail modals
function displayDetailModal(content) {
    let modal = document.getElementById('detailModal');
    if (!modal) {
        modal = document.createElement('div');
        modal.className = 'modal fade';
        modal.id = 'detailModal';
        modal.innerHTML = `
            <div class="modal-dialog modal-lg">
                <div class="modal-content">
                    ${content}
                </div>
            </div>
        `;
        document.body.appendChild(modal);
    } else {
        modal.querySelector('.modal-content').innerHTML = content;
    }
    
    const detailModal = new bootstrap.Modal(modal);
    detailModal.show();
}
async function generateOverviewReport() {
    await generateSystemOverview();
    const analysisTab = new bootstrap.Tab(document.getElementById('analysis-tab'));
    analysisTab.show();
}

function displayCases(cases) {
    const container = document.getElementById('casesList');
    if (!container) return;
    
    if (cases.length === 0) {
        container.innerHTML = '<div class="col-12 text-center text-muted">No cases found. Create your first case to get started!</div>';
        return;
    }
    
    container.innerHTML = cases.map(caseItem => `
        <div class="col-md-6 col-lg-4">
            <div class="card entity-card">
                <div class="card-body">
                    <h6 class="card-title">${caseItem.title}</h6>
                    <p class="card-text small text-muted">${caseItem.description.substring(0, 100)}${caseItem.description.length > 100 ? '...' : ''}</p>
                    <div class="d-flex justify-content-between align-items-center">
                        <span class="badge ${getStatusBadgeClass(caseItem.status)}">${caseItem.status}</span>
                        <span class="badge ${getPriorityBadgeClass(caseItem.priority)}">${caseItem.priority}</span>
                    </div>
                    <div class="mt-2 small">
                        <i class="fas fa-user-secret"></i> ${caseItem.suspect_count} suspects • 
                        <i class="fas fa-users"></i> ${caseItem.character_count} characters
                    </div>
                    <div class="mt-3 d-flex gap-2">
                        <button class="btn btn-sm btn-outline-primary" onclick="editCase(${caseItem.id})">
                            <i class="fas fa-edit"></i> Edit
                        </button>
                        <button class="btn btn-sm btn-outline-danger" onclick="deleteCase(${caseItem.id})">
                            <i class="fas fa-trash"></i> Delete
                        </button>
                        <button class="btn btn-sm btn-outline-info" onclick="viewCaseDetail(${caseItem.id})">
                            <i class="fas fa-eye"></i> View
                        </button>
                    </div>
                </div>
            </div>
        </div>
    `).join('');
}
function displaySuspects(suspects) {
    const container = document.getElementById('suspectsList');
    if (!container) return;
    
    if (suspects.length === 0) {
        container.innerHTML = '<div class="col-12 text-center text-muted">No suspects found.</div>';
        return;
    }
    
    container.innerHTML = suspects.map(suspect => `
        <div class="col-md-6 col-lg-4">
            <div class="card entity-card">
                <div class="card-body">
                    <h6 class="card-title">${suspect.name}</h6>
                    <p class="card-text small">${suspect.occupation || 'Unknown occupation'} • ${suspect.age || 'Unknown age'}</p>
                    <div class="d-flex justify-content-between align-items-center">
                        <span class="badge ${getSuspectStatusBadgeClass(suspect.status)}">${suspect.status}</span>
                        <span class="badge ${getAlibiStrengthBadgeClass(suspect.alibi_strength)}">${suspect.alibi_strength}</span>
                    </div>
                    <div class="mt-2 small">
                        Suspicion: ${suspect.suspicion_level || 0}% • 
                        ${suspect.is_prime_suspect ? '<i class="fas fa-star text-warning"></i> Prime' : ''}
                    </div>
                    <div class="mt-3 d-flex gap-2">
                        <button class="btn btn-sm btn-outline-primary" onclick="editSuspect(${suspect.id})">
                            <i class="fas fa-edit"></i> Edit
                        </button>
                        <button class="btn btn-sm btn-outline-danger" onclick="deleteSuspect(${suspect.id})">
                            <i class="fas fa-trash"></i> Delete
                        </button>
                        <button class="btn btn-sm btn-outline-info" onclick="viewSuspectDetail(${suspect.id})">
                            <i class="fas fa-eye"></i> View
                        </button>
                    </div>
                </div>
            </div>
        </div>
    `).join('');
}

function formatStatusText(status) {
    const statusMap = {
        'OPEN': 'Open',
        'IN_PROGRESS': 'In Progress', 
        'SOLVED': 'Solved',
        'COLD': 'Cold Case',
        'UNSOLVED': 'Unsolved'
    };
    return statusMap[status] || status;
}

function formatPriorityText(priority) {
    const priorityMap = {
        'LOW': 'Low',
        'MEDIUM': 'Medium',
        'HIGH': 'High', 
        'URGENT': 'Urgent'
    };
    return priorityMap[priority] || priority;
}
function displayCharacters(characters) {
    const container = document.getElementById('charactersList');
    if (!container) return;
    
    if (characters.length === 0) {
        container.innerHTML = '<div class="col-12 text-center text-muted">No characters found.</div>';
        return;
    }
    
    container.innerHTML = characters.map(character => `
        <div class="col-md-6 col-lg-4">
            <div class="card entity-card">
                <div class="card-body">
                    <h6 class="card-title">${character.name}</h6>
                    <p class="card-text small">${character.role}</p>
                    <span class="badge ${getRoleBadgeClass(character.role)}">${character.role}</span>
                    <div class="mt-2 small text-muted">
                        ${character.story ? character.story.substring(0, 80) + (character.story.length > 80 ? '...' : '') : 'No description'}
                    </div>
                    <div class="mt-3 d-flex gap-2">
                        <button class="btn btn-sm btn-outline-primary" onclick="editCharacter(${character.id})">
                            <i class="fas fa-edit"></i> Edit
                        </button>
                        <button class="btn btn-sm btn-outline-danger" onclick="deleteCharacter(${character.id})">
                            <i class="fas fa-trash"></i> Delete
                        </button>
                        <button class="btn btn-sm btn-outline-info" onclick="viewCharacterDetail(${character.id})">
                            <i class="fas fa-eye"></i> View
                        </button>
                    </div>
                </div>
            </div>
        </div>
    `).join('');
}


function startGame() {
    const caseId = document.getElementById("playCaseSelect").value;

    if (!caseId) {
        alert("Please select a case first!");
        return;
    }
    localStorage.setItem("selectedCase", caseId);

    window.location.href = "game.html";
}
window.addEventListener('message', function(event) {
    if (event.data && event.data.type === 'caseUpdated') {
        loadCases();
        showSuccess('Case status updated!');
    }
});


document.addEventListener('DOMContentLoaded', function() {
    if (localStorage.getItem('caseUpdated')) {
        loadCases();
        localStorage.removeItem('caseUpdated');
    }
});
