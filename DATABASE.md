# Database Documentation

## SQLite Integration

The Task Manager uses SQLite - a lightweight, file-based database requiring no server setup.

## Setup

### 1. Install SQLite (Usually Pre-installed)

```bash
# Check if installed
sqlite3 --version

# If needed (most systems have it)
apt-get install sqlite3 libsqlite3-dev  # Linux
brew install sqlite3  # macOS
```

### 2. Initialize Database

```bash
cd scripts
./setup_sqlite.sh
```

This creates:
- Database file: `data/tasks.db`
- Tables: `tasks`, `settings`

### 3. Verify Setup

```bash
sqlite3 data/tasks.db ".tables"
```

## Database Schema

### tasks Table

| Column | Type | Description |
|--------|------|-------------|
| id | INTEGER | Primary key (auto-increment) |
| title | TEXT | Task title |
| description | TEXT | Task description |
| priority | TEXT | LOW, MEDIUM, or HIGH |
| status | TEXT | PENDING, IN_PROGRESS, or COMPLETED |
| created_at | INTEGER | Unix timestamp |
| due_date | INTEGER | Unix timestamp (0 if no date) |

### settings Table

| Column | Type | Description |
|--------|------|-------------|
| key | TEXT | Setting key (primary key) |
| value | TEXT | Setting value |

## Advantages

✅ **No Server** - File-based, no daemon  
✅ **No Permissions** - Works as regular user  
✅ **Portable** - Single file database  
✅ **Fast** - Excellent performance for CLI apps  
✅ **Reliable** - ACID compliant  
✅ **Zero Configuration** - Just works!

## Usage

The application automatically uses SQLite when SQLiteHandler is configured.

Database location: `data/tasks.db`

## SQL Queries

### View All Tasks
```bash
sqlite3 data/tasks.db "SELECT * FROM tasks ORDER BY id;"
```

### Get Statistics
```bash
sqlite3 data/tasks.db "
SELECT 
    COUNT(*) as total,
    SUM(CASE WHEN status='PENDING' THEN 1 ELSE 0 END) as pending,
    SUM(CASE WHEN status='COMPLETED' THEN 1 ELSE 0 END) as completed
FROM tasks;"
```

### Backup
```bash
cp data/tasks.db data/tasks_backup_$(date +%Y%m%d).db
```

## Troubleshooting

**Database locked**: Close other connections  
**Permission denied**: Check file permissions on data/ directory  
**Not found**: Run setup script first
