#include <iostream>
#include "../inc/TaskManager.hpp"
#include "../inc/SQLiteHandler.hpp"

using namespace std;

int main() {
    cout << "========================================" << endl;
    cout << "  JSON to SQLite Migration Tool" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    // Load tasks from JSON
    cout << "📂 Loading tasks from JSON..." << endl;
    TaskManager manager;
    auto& tasks = manager.getAllTasks();
    int taskCount = tasks.size();
    
    cout << "✓ Found " << taskCount << " tasks in JSON file" << endl;
    
    if (taskCount == 0) {
        cout << "\n⚠️  No tasks to migrate!" << endl;
        return 0;
    }
    
    // Connect to SQLite
    cout << "\n🔌 Connecting to SQLite database..." << endl;
    SQLiteHandler db("../data/tasks.db");
    
    if (!db.connect()) {
        cerr << "❌ Failed to connect to database!" << endl;
        return 1;
    }
    
    // Save to SQLite
    cout << "💾 Migrating tasks to SQLite..." << endl;
    
    int migrated = 0;
    for (const auto& task : tasks) {
        if (db.insertTask(task) > 0) {
            migrated++;
        }
    }
    
    cout << "\n✅ Migration complete!" << endl;
    cout << "   Migrated: " << migrated << "/" << taskCount << " tasks" << endl;
    
    // Verify
    auto dbTasks = db.getAllTasks();
    cout << "   Verified: " << dbTasks.size() << " tasks in database" << endl;
    
    db.disconnect();
    
    cout << "\n📊 You can now view tasks with:" << endl;
    cout << "   sqlite3 ../data/tasks.db 'SELECT * FROM tasks;'" << endl;
    cout << endl;
    
    return 0;
}
