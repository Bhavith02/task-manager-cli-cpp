#!/bin/bash

echo "======================================"
echo "  Task Manager - Database Setup"
echo "======================================"
echo ""

# Database configuration
DB_NAME="taskmanager"
DB_USER="postgres"
DB_PASSWORD=""

echo "Setting up PostgreSQL database..."

# Create database
sudo -u postgres psql -c "CREATE DATABASE $DB_NAME;" 2>/dev/null || echo "Database already exists"

# Create tables
sudo -u postgres psql -d $DB_NAME << EOF
CREATE TABLE IF NOT EXISTS tasks (
    id SERIAL PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    description TEXT NOT NULL,
    priority VARCHAR(20) NOT NULL,
    status VARCHAR(20) NOT NULL,
    created_at BIGINT NOT NULL,
    due_date BIGINT DEFAULT 0
);

CREATE TABLE IF NOT EXISTS settings (
    key VARCHAR(50) PRIMARY KEY,
    value VARCHAR(255)
);

-- Insert default next_id
INSERT INTO settings (key, value) VALUES ('next_id', '1')
ON CONFLICT (key) DO NOTHING;

-- Grant permissions
GRANT ALL PRIVILEGES ON DATABASE $DB_NAME TO $DB_USER;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO $DB_USER;
GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public TO $DB_USER;

EOF

echo ""
echo "✅ Database setup complete!"
echo ""
echo "Database name: $DB_NAME"
echo "Connection string: host=localhost port=5432 dbname=$DB_NAME user=$DB_USER"
echo ""
echo "To test connection:"
echo "  psql -d $DB_NAME -U $DB_USER"
echo ""
