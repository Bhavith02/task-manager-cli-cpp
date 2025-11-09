# 🚀 Deployment Guide

## Quick Start - Local Testing

### 1. Start API Server
```bash
cd /data/home/sbhavith/practice/MyAIProject/build
./task_api_server
# API running on http://localhost:8080
```

### 2. Serve Web Frontend
```bash
cd /data/home/sbhavith/practice/MyAIProject/web
python3 -m http.server 3000
# Web UI at http://localhost:3000
```

### 3. Open Browser
Visit: http://localhost:3000

---

## Cloud Deployment Options

### Option 1: Heroku (Free Tier)
```bash
# Install Heroku CLI
# Create Procfile
echo "web: ./task_api_server" > Procfile

# Deploy
heroku create your-task-manager
git push heroku main
```

### Option 2: DigitalOcean ($5/month)
- Create Ubuntu droplet
- Install dependencies
- Run API server
- Use nginx for web frontend

### Option 3: AWS EC2 (Free tier available)
- Launch t2.micro instance
- Configure security groups
- Deploy and run

---

## Mobile App Integration

Your REST API is ready for mobile!

**React Native example:**
```javascript
const API_URL = 'https://your-api.com/api';

fetch(`${API_URL}/tasks`)
  .then(res => res.json())
  .then(tasks => console.log(tasks));
```

---

See full deployment scripts in `scripts/deploy/`
