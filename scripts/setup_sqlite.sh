#!/bin/bash

echo "======================================"
echo "  Task Manager - SQLite Setup"
echo "======================================"
echo ""

DB_PATH="../data/tasks.db"

echo "Creating SQLite database at: $DB_PATH"

# Create database and tables
sqlite3 $DB_PATH << EOF
CREATE TABLE IF NOT EXISTS tasks (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL,
    description TEXT NOT NULL,
    priority TEXT NOT NULL,
    status TEXT NOT NULL,
    created_at INTEGER NOT NULL,
    due_date INTEGER DEFAULT 0
);

CREATE TABLE IF NOT EXISTS settings (
    key TEXT PRIMARY KEY,
    value TEXT
);

INSERT OR IGNORE INTO settings (key, value) VALUES ('next_id', '1');

EOF

echo ""
echo "✅ SQLite database setup complete!"
echo ""
echo "Database file: $DB_PATH"
echo "No server needed - file-based storage!"
echo ""
echo "To view data:"
echo "  sqlite3 $DB_PATH 'SELECT * FROM tasks;'"
echo ""
