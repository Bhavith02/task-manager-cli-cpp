#!/bin/bash

echo "Quick JSON to SQLite Migration"
echo "==============================="
echo ""

# Export to CSV first (using existing app)
echo "Step 1: Exporting tasks to CSV..."
cd ../build
echo "8" | ./task_manager  # Export menu
echo "1" | ./task_manager  # Export all
echo ""

# Check if CSV exists
if [ -f "../data/tasks_export.csv" ]; then
    echo "✓ CSV export found"
    
    # Import to SQLite
    echo "Step 2: Importing to SQLite..."
    sqlite3 ../data/tasks.db << EOF
.mode csv
.import ../data/tasks_export.csv tasks_temp
INSERT INTO tasks SELECT * FROM tasks_temp;
DROP TABLE tasks_temp;
EOF
    
    echo "✅ Migration complete!"
    echo ""
    sqlite3 ../data/tasks.db "SELECT COUNT(*) as task_count FROM tasks;"
else
    echo "❌ CSV file not found. Please export tasks first."
fi
