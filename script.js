// Memory simulator state
let arenaBlocks = [];
let totalCapacity = 16;
let allocatedCount = 0;
let checkpoints = [];
let nextBlockId = 1;
let selectedBlockIndex = -1;

// Initialize empty arena
function initializeArena() {
    arenaBlocks = [];
    for (let i = 0; i < totalCapacity; i++) {
        arenaBlocks.push({
            id: null,
            size: 0,
            type: 'free',
            hasFinalizer: false,
            allocated: false
        });
    }
    updateMemoryDisplay();
}

function updateMemoryDisplay() {
    const container = document.getElementById('memoryBlocks');
    const stats = document.getElementById('memoryStats');

    container.innerHTML = '';

    arenaBlocks.forEach((block, index) => {
        const blockEl = document.createElement('div');
        blockEl.className = 'memory-block';
        blockEl.onclick = () => selectBlock(index);

        if (index === selectedBlockIndex) {
            blockEl.classList.add('selected');
        }

        if (checkpoints.includes(index)) {
            blockEl.className += ' block-checkpoint';
            blockEl.textContent = 'SNAP';
        } else if (block.allocated) {
            if (block.hasFinalizer) {
                blockEl.className += ' block-finalizer';
            } else {
                blockEl.className += ' block-allocated';
            }
            blockEl.textContent = `B${block.id}`;
        } else {
            blockEl.className += ' block-free';
            blockEl.textContent = 'FREE';
        }

        container.appendChild(blockEl);
    });

    const finalizerCount = arenaBlocks.filter(b => b.hasFinalizer).length;
    stats.textContent = `Arena: ${allocatedCount}/${totalCapacity} blocks allocated | ${checkpoints.length} checkpoints | ${finalizerCount} finalizers`;
}

function selectBlock(index) {
    selectedBlockIndex = index;
    const block = arenaBlocks[index];
    const details = document.getElementById('selectedBlock');

    if (block.allocated) {
        details.innerHTML = `
            <strong>Block ${block.id}</strong> | Size: ${block.size} bytes |
            ${block.hasFinalizer ? 'Has finalizer' : 'No finalizer'} |
            Address: 0x${(0x7fff0000 + index * 64).toString(16)}
        `;
    } else {
        details.innerHTML = `
            <strong>Free Block</strong> | Available for allocation |
            Address: 0x${(0x7fff0000 + index * 64).toString(16)}
        `;
    }

    updateMemoryDisplay();
}

function logAllocation(message, type = 'output') {
    const log = document.getElementById('allocationLog');
    const entry = document.createElement('div');
    entry.className = `log-entry ${type}`;
    entry.textContent = `[${new Date().toLocaleTimeString()}] ${message}`;
    log.appendChild(entry);
    log.scrollTop = log.scrollHeight;

    // Keep only last 20 entries
    while (log.children.length > 21) { // +1 for header
        log.removeChild(log.children[1]);
    }
}

function allocateSmall() {
    const freeIndex = arenaBlocks.findIndex(b => !b.allocated);
    if (freeIndex !== -1) {
        arenaBlocks[freeIndex] = {
            id: nextBlockId++,
            size: 32,
            type: 'allocated',
            hasFinalizer: false,
            allocated: true
        };
        allocatedCount++;
        logAllocation(`arena_alloc(arena, 32) -> Block ${arenaBlocks[freeIndex].id}`);
        updateMemoryDisplay();
    } else {
        logAllocation('arena_alloc() failed - arena full', 'error');
    }
}

function allocateLarge() {
    const freeIndex = arenaBlocks.findIndex(b => !b.allocated);
    if (freeIndex !== -1) {
        arenaBlocks[freeIndex] = {
            id: nextBlockId++,
            size: 128,
            type: 'allocated',
            hasFinalizer: false,
            allocated: true
        };
        allocatedCount++;
        logAllocation(`arena_alloc(arena, 128) -> Block ${arenaBlocks[freeIndex].id}`);
        updateMemoryDisplay();
    } else {
        logAllocation('arena_alloc() failed - arena full', 'error');
    }
}

function allocateWithFinalizer() {
    const freeIndex = arenaBlocks.findIndex(b => !b.allocated);
    if (freeIndex !== -1) {
        arenaBlocks[freeIndex] = {
            id: nextBlockId++,
            size: 64,
            type: 'allocated',
            hasFinalizer: true,
            allocated: true
        };
        allocatedCount++;
        logAllocation(`arena_alloc(arena, 64) + arena_set_destructor() -> Block ${arenaBlocks[freeIndex].id}`);
        updateMemoryDisplay();
    } else {
        logAllocation('arena_alloc() failed - arena full', 'error');
    }
}

function reallocateBlock() {
    const allocatedIndex = arenaBlocks.findIndex(b => b.allocated && !b.hasFinalizer);
    if (allocatedIndex !== -1) {
        const block = arenaBlocks[allocatedIndex];
        const oldSize = block.size;
        block.size = Math.min(block.size * 2, 256);
        logAllocation(`arena_realloc(Block ${block.id}, ${oldSize} -> ${block.size})`);
        updateMemoryDisplay();
    } else {
        logAllocation('No suitable block for reallocation', 'error');
    }
}

function createCheckpoint() {
    const checkpointPos = allocatedCount;
    if (!checkpoints.includes(checkpointPos)) {
        checkpoints.push(checkpointPos);
        logAllocation(`arena_snapshot() -> checkpoint at position ${checkpointPos}`);
        updateMemoryDisplay();
    } else {
        logAllocation('Checkpoint already exists at this position', 'comment');
    }
}

function restoreCheckpoint() {
    if (checkpoints.length > 0) {
        const lastCheckpoint = checkpoints.pop();

        // Free blocks beyond checkpoint
        for (let i = arenaBlocks.length - 1; i >= 0; i--) {
            if (arenaBlocks[i].allocated && arenaBlocks[i].id > lastCheckpoint) {
                if (arenaBlocks[i].hasFinalizer) {
                    logAllocation(`Calling finalizer for Block ${arenaBlocks[i].id}`, 'comment');
                }
                arenaBlocks[i] = {
                    id: null,
                    size: 0,
                    type: 'free',
                    hasFinalizer: false,
                    allocated: false
                };
                allocatedCount--;
            }
        }

        logAllocation(`arena_restore() -> restored to checkpoint ${lastCheckpoint}`);
        updateMemoryDisplay();
    } else {
        logAllocation('No checkpoints to restore', 'error');
    }
}

function freeSpecific() {
    if (selectedBlockIndex !== -1 && arenaBlocks[selectedBlockIndex].allocated) {
        const block = arenaBlocks[selectedBlockIndex];
        if (block.hasFinalizer) {
            logAllocation(`Calling finalizer for Block ${block.id}`, 'comment');
        }
        logAllocation(`arena_free(Block ${block.id})`);

        arenaBlocks[selectedBlockIndex] = {
            id: null,
            size: 0,
            type: 'free',
            hasFinalizer: false,
            allocated: false
        };
        allocatedCount--;
        selectedBlockIndex = -1;
        updateMemoryDisplay();
    } else {
        logAllocation('Select an allocated block first', 'error');
    }
}

function clearArena() {
    let finalizerCount = 0;
    arenaBlocks.forEach(block => {
        if (block.allocated && block.hasFinalizer) {
            finalizerCount++;
        }
    });

    if (finalizerCount > 0) {
        logAllocation(`Calling ${finalizerCount} finalizers...`, 'comment');
    }

    initializeArena();
    allocatedCount = 0;
    checkpoints = [];
    selectedBlockIndex = -1;
    logAllocation('arena_destroy() - all memory freed');
    updateMemoryDisplay();
}

function showTab(tabName) {
    // Hide all tab contents
    document.querySelectorAll('.tab-content').forEach(content => {
        content.classList.remove('active');
    });

    // Remove active from all tabs
    document.querySelectorAll('.tab').forEach(tab => {
        tab.classList.remove('active');
    });

    // Show selected tab
    document.getElementById(tabName).classList.add('active');
    event.target.classList.add('active');

    // Refresh the editor for the active tab
    setTimeout(() => {
        const editorId = tabName + 'Code';
        if (editors[editorId]) {
            editors[editorId].refresh();
        }
    }, 100);

    // Add simulate button for each use case
    const demoBtn = document.getElementById('simulateBtn');
    if (demoBtn) {
        demoBtn.onclick = () => simulateUseCase(tabName);
        demoBtn.textContent = `Simulate ${tabName.charAt(0).toUpperCase() + tabName.slice(1)} Demo`;
    }
}

// Store editor instances
let editors = {};

// Initialize CodeMirror editors
function initializeEditors() {
    const editorConfigs = {
        mode: 'text/x-csrc',
        theme: 'default',
        lineNumbers: true,
        readOnly: true,
        viewportMargin: Infinity,
        lineWrapping: true,
        styleActiveLine: true
    };

    // Initialize all editors
    const editorIds = ['basicCode', 'compilerCode', 'gameCode', 'serverCode'];
    editorIds.forEach(id => {
        const textarea = document.getElementById(id);
        if (textarea) {
            editors[id] = CodeMirror.fromTextArea(textarea, editorConfigs);
            editors[id].setSize(null, 300);
        }
    });
}

function simulateUseCase(caseType) {
    clearArena();

    switch(caseType) {
        case 'basic':
            setTimeout(() => allocateSmall(), 500);
            setTimeout(() => allocateLarge(), 1000);
            setTimeout(() => allocateWithFinalizer(), 1500);
            logAllocation('Simulating basic usage pattern...', 'comment');
            break;

        case 'compiler':
            logAllocation('Simulating compiler parse phase...', 'comment');
            for(let i = 0; i < 6; i++) {
                setTimeout(() => {
                    allocateSmall();
                    if (i === 2) createCheckpoint();
                }, i * 300);
            }
            setTimeout(() => {
                logAllocation('Parse complete - freeing AST nodes', 'comment');
                clearArena();
            }, 3000);
            break;

        case 'game':
            logAllocation('Simulating game frame allocation...', 'comment');
            setTimeout(() => createCheckpoint(), 200);
            setTimeout(() => allocateSmall(), 400); // entities
            setTimeout(() => allocateLarge(), 600);  // particles
            setTimeout(() => allocateSmall(), 800);  // render commands
            setTimeout(() => {
                logAllocation('Frame complete - restoring checkpoint', 'comment');
                restoreCheckpoint();
            }, 2500);
            break;

        case 'server':
            logAllocation('Simulating HTTP request handling...', 'comment');
            setTimeout(() => allocateSmall(), 300);  // request struct
            setTimeout(() => allocateLarge(), 600);  // headers
            setTimeout(() => allocateWithFinalizer(), 900); // file handle
            setTimeout(() => {
                logAllocation('Request complete - cleaning up', 'comment');
                clearArena();
            }, 2500);
            break;
    }
}

// Initialize everything
initializeArena();

// Initialize CodeMirror when DOM is ready
document.addEventListener('DOMContentLoaded', function() {
    setTimeout(initializeEditors, 100);
});

// Auto-demo sequence
setTimeout(() => {
    logAllocation('LibArena memory simulator ready', 'comment');
    logAllocation('Try allocating blocks and creating checkpoints!', 'comment');
    logAllocation('Click blocks for details, switch tabs for different use cases', 'comment');
}, 1000);

// Additional interactive functions
function stressTest() {
    logAllocation('Running stress test...', 'comment');
    clearArena();

    for (let i = 0; i < 8; i++) {
        setTimeout(() => {
            if (i % 3 === 0) {
                allocateWithFinalizer();
            } else if (i % 2 === 0) {
                allocateLarge();
            } else {
                allocateSmall();
            }
        }, i * 200);
    }

    setTimeout(() => {
        logAllocation('Stress test complete - arena at capacity', 'comment');
    }, 2000);
}

function fragmentationDemo() {
    logAllocation('Demonstrating fragmentation handling...', 'comment');
    clearArena();

    // Allocate several blocks
    setTimeout(() => allocateSmall(), 200);
    setTimeout(() => allocateLarge(), 400);
    setTimeout(() => allocateSmall(), 600);
    setTimeout(() => allocateLarge(), 800);

    // Free middle blocks to create fragmentation
    setTimeout(() => {
        if (arenaBlocks[1].allocated) {
            selectedBlockIndex = 1;
            freeSpecific();
            logAllocation('Freed middle block - creating fragmentation', 'comment');
        }
    }, 1200);

    setTimeout(() => {
        if (arenaBlocks[3].allocated) {
            selectedBlockIndex = 3;
            freeSpecific();
            logAllocation('Arena handles fragmentation automatically', 'comment');
        }
    }, 1600);
}
