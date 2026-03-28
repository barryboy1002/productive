# Build Instructions for Productive

This document provides detailed instructions on how to build and set up the Productive application on various operating systems, including Ubuntu, Fedora, and macOS. Additionally, it includes steps for setting up the PostgreSQL database required by the application.

## Table of Contents
- [Ubuntu](#ubuntu)
- [Fedora](#fedora)
- [macOS](#macos)
- [PostgreSQL Setup](#postgresql-setup)

---

## Ubuntu
1. **Install Dependencies**:
   ```bash
   sudo apt update
   sudo apt install build-essential libpq-dev python3-pip python3-dev
   ```

2. **Clone the Repository**:
   ```bash
   git clone https://github.com/barryboy1002/productive.git
   cd productive
   ```

3. **Install Python Packages**:
   ```bash
   pip3 install -r requirements.txt
   ```

4. **Run the Application**:
   ```bash
   python3 app.py
   ```

## Fedora
1. **Install Dependencies**:
   ```bash
   sudo dnf install gcc gcc-c++ postgresql-devel python3-pip python3-devel
   ```

2. **Clone the Repository**:
   ```bash
   git clone https://github.com/barryboy1002/productive.git
   cd productive
   ```

3. **Install Python Packages**:
   ```bash
   pip3 install -r requirements.txt
   ```

4. **Run the Application**:
   ```bash
   python3 app.py
   ```

## macOS
1. **Install Homebrew** (if not installed):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. **Install Dependencies**:
   ```bash
   brew install postgresql python
   ```

3. **Clone the Repository**:
   ```bash
   git clone https://github.com/barryboy1002/productive.git
   cd productive
   ```

4. **Install Python Packages**:
   ```bash
   pip3 install -r requirements.txt
   ```

5. **Run the Application**:
   ```bash
   python3 app.py
   ```

## PostgreSQL Setup
1. **Install PostgreSQL**:
   - For **Ubuntu** and **Fedora**, use the instructions above.
   - For **macOS**, use Homebrew as mentioned above.

2. **Start PostgreSQL Service**:
   ```bash
   sudo service postgresql start   # For Ubuntu
   sudo systemctl start postgresql   # For Fedora
   brew services start postgresql     # For macOS
   ```

3. **Create the Database**:
   ```bash
   psql postgres
   CREATE DATABASE productive;
   CREATE USER productive_user WITH ENCRYPTED PASSWORD 'your_password';
   GRANT ALL PRIVILEGES ON DATABASE productive TO productive_user;
   \q
   ```

4. **Update Database Configuration**:
   Update your database configuration in the application files to use the database you created above.

---

Make sure to adapt paths, database names, and other settings according to your installation preferences. Happy building!