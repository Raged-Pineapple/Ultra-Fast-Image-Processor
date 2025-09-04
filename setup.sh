#!/bin/bash

set -e

echo "🚀 Setting up Ultra-Fast-Image-Processor on WSL..."

# ---------------------------
# STEP 1: Update and Upgrade
# ---------------------------
echo "🔄 Updating system..."
sudo apt update && sudo apt upgrade -y

# ---------------------------
# STEP 2: Install Dependencies
# ---------------------------
echo "📦 Installing required packages..."
sudo apt install -y git g++ make cmake pkg-config python3 python3-pip python3-venv python3-opencv

# ---------------------------
# STEP 3: Clone the Repository
# ---------------------------
REPO_URL="https://github.com/Raged-Pineapple/Ultra-Fast-Image-Processor.git"
PROJECT_DIR="$HOME/Ultra-Fast-Image-Processor"

if [ -d "$PROJECT_DIR" ]; then
    echo "✅ Repository already exists. Skipping clone."
else
    echo "📥 Cloning repository..."
    git clone "$REPO_URL" "$PROJECT_DIR"
fi

cd "$PROJECT_DIR"

# ---------------------------
# STEP 4: Create Virtual Environment
# ---------------------------
if [ ! -d "venv" ]; then
    echo "🐍 Creating Python virtual environment..."
    python3 -m venv venv
else
    echo "✅ Virtual environment already exists."
fi

echo "⚡ Activating virtual environment..."
source venv/bin/activate

# ---------------------------
# STEP 5: Install Python Dependencies
# ---------------------------
echo "📦 Installing matplotlib..."
pip install --upgrade pip --break-system-packages
pip install matplotlib --break-system-packages

# ---------------------------
# STEP 6: Compile the C++ Code
# ---------------------------
echo "⚙️ Compiling C++ code..."
g++ main.cpp -o processor `pkg-config --cflags --libs opencv4`

# ---------------------------
# STEP 7: Run the Processor
# ---------------------------
echo "🖼️ Running image processor..."
./processor || echo "⚠️ Warning: Make sure to update the image path inside main.cpp if needed."

# ---------------------------
# STEP 8: Generate Performance Graph
# ---------------------------
echo "📊 Generating performance graph..."
sed -i 's/plt.show()/plt.savefig("performance_plot.png")/g' graph_plotter.py
python3 graph_plotter.py

# ---------------------------
# STEP 9: Open Project Folder in Windows Explorer
# ---------------------------
echo "📂 Opening project folder in Windows Explorer..."
explorer.exe .

echo "🎉 Setup complete! Check the project folder for 'performance_plot.png'."
