// API Configuration - Auto-detect environment
const API_BASE_URL = window.location.hostname === 'localhost' 
    ? 'http://localhost:8080/api'
    : 'https://YOUR-RENDER-APP.onrender.com/api'; // Replace after deploying API

// Global state
let allTasks = [];
let currentFilter = { status: 'all', priority: 'all' };

// Initialize
document.addEventListener('DOMContentLoaded', () => {
    loadTasks();
    setInterval(loadTasks, 30000); // Auto-refresh every 30 seconds
});

// Load all tasks from API
async function loadTasks() {
    try {
        const response = await fetch(`${API_BASE_URL}/tasks`);
        if (!response.ok) throw new Error('Failed to fetch tasks');
        
        allTasks = await response.json();
        displayTasks(allTasks);
        updateStatistics();
    } catch (error) {
        console.error('Error loading tasks:', error);
        showError('Failed to load tasks. Make sure API server is running.');
    }
}

// Display tasks
function displayTasks(tasks) {
    const container = document.getElementById('tasks-container');
    
    if (tasks.length === 0) {
        container.innerHTML = `
            <div class="loading">
                <h2>No tasks found</h2>
                <p>Click "Add Task" to create your first task!</p>
            </div>
        `;
        return;
    }
    
    container.innerHTML = tasks.map(task => `
        <div class="task-card" onclick="editTask(${task.id})">
            <h3>${escapeHtml(task.title)}</h3>
            <p>${escapeHtml(task.description)}</p>
            <div class="task-badges">
                <span class="badge priority-${task.priority}">${task.priority}</span>
                <span class="badge status-${task.status}">${task.status.replace('_', ' ')}</span>
            </div>
            <div class="task-actions" onclick="event.stopPropagation()">
                <button class="btn btn-primary" onclick="editTask(${task.id})">
                    ✏️ Edit
                </button>
                <button class="btn btn-danger" onclick="deleteTask(${task.id})">
                    🗑️ Delete
                </button>
            </div>
        </div>
    `).join('');
}

// Update statistics
function updateStatistics() {
    const stats = {
        total: allTasks.length,
        pending: allTasks.filter(t => t.status === 'PENDING').length,
        inProgress: allTasks.filter(t => t.status === 'IN_PROGRESS').length,
        completed: allTasks.filter(t => t.status === 'COMPLETED').length
    };
    
    document.getElementById('total-tasks').textContent = `Total: ${stats.total}`;
    document.getElementById('pending-tasks').textContent = `Pending: ${stats.pending}`;
    document.getElementById('completed-tasks').textContent = `Completed: ${stats.completed}`;
}

// Show add task modal
function showAddTaskModal() {
    document.getElementById('modal-title').textContent = 'Add New Task';
    document.getElementById('task-form').reset();
    document.getElementById('task-id').value = '';
    document.getElementById('task-modal').style.display = 'block';
}

// Edit task
async function editTask(id) {
    const task = allTasks.find(t => t.id === id);
    if (!task) return;
    
    document.getElementById('modal-title').textContent = 'Edit Task';
    document.getElementById('task-id').value = task.id;
    document.getElementById('task-title').value = task.title;
    document.getElementById('task-description').value = task.description;
    document.getElementById('task-priority').value = task.priority;
    document.getElementById('task-status').value = task.status;
    document.getElementById('task-modal').style.display = 'block';
}

// Save task (create or update)
async function saveTask(event) {
    event.preventDefault();
    
    const taskId = document.getElementById('task-id').value;
    const taskData = {
        title: document.getElementById('task-title').value,
        description: document.getElementById('task-description').value,
        priority: document.getElementById('task-priority').value,
        status: document.getElementById('task-status').value
    };
    
    try {
        let response;
        if (taskId) {
            // Update existing task
            response = await fetch(`${API_BASE_URL}/tasks/${taskId}`, {
                method: 'PUT',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(taskData)
            });
        } else {
            // Create new task
            response = await fetch(`${API_BASE_URL}/tasks`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(taskData)
            });
        }
        
        if (!response.ok) throw new Error('Failed to save task');
        
        closeModal();
        loadTasks();
        showSuccess(taskId ? 'Task updated!' : 'Task created!');
    } catch (error) {
        console.error('Error saving task:', error);
        showError('Failed to save task');
    }
}

// Delete task
async function deleteTask(id) {
    if (!confirm('Are you sure you want to delete this task?')) return;
    
    try {
        const response = await fetch(`${API_BASE_URL}/tasks/${id}`, {
            method: 'DELETE'
        });
        
        if (!response.ok) throw new Error('Failed to delete task');
        
        loadTasks();
        showSuccess('Task deleted!');
    } catch (error) {
        console.error('Error deleting task:', error);
        showError('Failed to delete task');
    }
}

// Search tasks
function searchTasks() {
    const searchTerm = document.getElementById('search-box').value.toLowerCase();
    const filtered = allTasks.filter(task => 
        task.title.toLowerCase().includes(searchTerm) ||
        task.description.toLowerCase().includes(searchTerm)
    );
    displayTasks(filtered);
}

// Filter tasks
function filterTasks() {
    const statusFilter = document.getElementById('filter-status').value;
    const priorityFilter = document.getElementById('filter-priority').value;
    
    let filtered = allTasks;
    
    if (statusFilter !== 'all') {
        filtered = filtered.filter(t => t.status === statusFilter);
    }
    
    if (priorityFilter !== 'all') {
        filtered = filtered.filter(t => t.priority === priorityFilter);
    }
    
    displayTasks(filtered);
}

// Show statistics
async function showStatistics() {
    try {
        const response = await fetch(`${API_BASE_URL}/stats`);
        const stats = await response.json();
        
        const content = `
            <h3>Task Statistics</h3>
            <p><strong>Total Tasks:</strong> ${stats.total}</p>
            <h4>By Status:</h4>
            <ul>
                <li>Pending: ${stats.byStatus.pending}</li>
                <li>In Progress: ${stats.byStatus.inProgress}</li>
                <li>Completed: ${stats.byStatus.completed}</li>
            </ul>
            <h4>By Priority:</h4>
            <ul>
                <li>Low: ${stats.byPriority.low}</li>
                <li>Medium: ${stats.byPriority.medium}</li>
                <li>High: ${stats.byPriority.high}</li>
            </ul>
            <p><strong>Completion Rate:</strong> ${
                stats.total > 0 
                    ? Math.round((stats.byStatus.completed / stats.total) * 100)
                    : 0
            }%</p>
        `;
        
        document.getElementById('stats-content').innerHTML = content;
        document.getElementById('stats-modal').style.display = 'block';
    } catch (error) {
        showError('Failed to load statistics');
    }
}

// Modal controls
function closeModal() {
    document.getElementById('task-modal').style.display = 'none';
}

function closeStatsModal() {
    document.getElementById('stats-modal').style.display = 'none';
}

// Utility functions
function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}

function showSuccess(message) {
    alert('✅ ' + message);
}

function showError(message) {
    alert('❌ ' + message);
}

// Close modals when clicking outside
window.onclick = function(event) {
    if (event.target.classList.contains('modal')) {
        event.target.style.display = 'none';
    }
}
