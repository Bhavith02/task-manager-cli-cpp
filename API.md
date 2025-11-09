# Task Manager REST API Documentation

Base URL: `http://localhost:8080`

## Endpoints

### 1. Get All Tasks

**GET** `/api/tasks`

Returns all tasks in the system.

**Response:**
```json
[
  {
    "id": 1,
    "title": "Complete project",
    "description": "Finish the task manager",
    "priority": "High",
    "status": "In Progress",
    "createdAt": 1704067200,
    "dueDate": 1704153600,
    "isCompleted": false,
    "isOverdue": false
  }
]
```

---

### 2. Get Task by ID

**GET** `/api/tasks/:id`

Returns a specific task.

**Parameters:**
- `id` (path) - Task ID

**Response:**
```json
{
  "id": 1,
  "title": "Complete project",
  "description": "Finish the task manager",
  "priority": "High",
  "status": "In Progress",
  "createdAt": 1704067200,
  "dueDate": 1704153600,
  "isCompleted": false,
  "isOverdue": false
}
```

**Error Response (404):**
```json
{
  "error": "Task not found"
}
```

---

### 3. Create Task

**POST** `/api/tasks`

Creates a new task.

**Request Body:**
```json
{
  "title": "New Task",
  "description": "Task description",
  "priority": "Medium"
}
```

**Response (201):**
```json
{
  "id": 2,
  "title": "New Task",
  "description": "Task description",
  "priority": "Medium",
  "status": "Pending",
  "createdAt": 1704067200,
  "dueDate": 0,
  "isCompleted": false,
  "isOverdue": false
}
```

---

### 4. Update Task

**PUT** `/api/tasks/:id`

Updates an existing task.

**Parameters:**
- `id` (path) - Task ID

**Request Body (partial update):**
```json
{
  "title": "Updated Title",
  "status": "COMPLETED"
}
```

**Response:**
```json
{
  "id": 1,
  "title": "Updated Title",
  "description": "Original description",
  "priority": "High",
  "status": "Completed",
  "createdAt": 1704067200,
  "dueDate": 0,
  "isCompleted": true,
  "isOverdue": false
}
```

---

### 5. Delete Task

**DELETE** `/api/tasks/:id`

Deletes a task.

**Parameters:**
- `id` (path) - Task ID

**Response:**
```json
{
  "success": true,
  "message": "Task deleted"
}
```

**Error Response (404):**
```json
{
  "error": "Task not found"
}
```

---

### 6. Get Statistics

**GET** `/api/stats`

Returns task statistics.

**Response:**
```json
{
  "total": 10,
  "byStatus": {
    "pending": 3,
    "inProgress": 5,
    "completed": 2
  },
  "byPriority": {
    "low": 2,
    "medium": 5,
    "high": 3
  }
}
```

---

## Example Usage

### cURL Examples

**Get all tasks:**
```bash
curl http://localhost:8080/api/tasks
```

**Create a task:**
```bash
curl -X POST http://localhost:8080/api/tasks \
  -H "Content-Type: application/json" \
  -d '{"title":"New Task","description":"Test","priority":"High"}'
```

**Update a task:**
```bash
curl -X PUT http://localhost:8080/api/tasks/1 \
  -H "Content-Type: application/json" \
  -d '{"status":"COMPLETED"}'
```

**Delete a task:**
```bash
curl -X DELETE http://localhost:8080/api/tasks/1
```

**Get statistics:**
```bash
curl http://localhost:8080/api/stats
```

---

## CORS

The API supports CORS and can be accessed from web applications.

All endpoints return JSON and include appropriate CORS headers.

---

## Error Handling

**400 Bad Request** - Invalid request format  
**404 Not Found** - Resource not found  
**500 Internal Server Error** - Server error

Error responses follow this format:
```json
{
  "error": "Error message description"
}
```
