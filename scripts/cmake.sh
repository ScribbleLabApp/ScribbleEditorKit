#!/bin/bash
set -e

BOLD="\033[1m"
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
RED="\033[1;31m"
BLUE="\033[1;34m"
RESET="\033[0m"

log() {
    echo -e "${BOLD}${BLUE}==>${RESET} $1\n"
}

success() {
    echo -e "${BOLD}${GREEN}[SUCCESS]${RESET} $1\n"
}

warn() {
    echo -e "${BOLD}${YELLOW}[WARN]${RESET} $1\n"
}

error() {
    echo -e "${BOLD}${RED}[ERR]${RESET} $1\n"
}

echo -e "${BOLD}${YELLOW}"
cat << "EOF"
   _____           _ _     _     _      ______    _ _ _             _  ___ _   
  / ____|         (_) |   | |   | |    |  ____|  | (_) |           | |/ (_) |  
 | (___   ___ _ __ _| |__ | |__ | | ___| |__   __| |_| |_ ___  _ __| ' / _| |_ 
  \___ \ / __| '__| | '_ \| '_ \| |/ _ \  __| / _` | | __/ _ \| '__|  < | | __|
  ____) | (__| |  | | |_) | |_) | |  __/ |___| (_| | | || (_) | |  | . \| | |_ 
 |_____/ \___|_|  |_|_.__/|_.__/|_|\___|______\__,_|_|\__\___/|_|  |_|\_\_|\__|
EOF
echo -e "${RESET}"

START_TIME=$(date +%s%3N)

log "Starting build process..."
cd ..
if [ -d "build" ]; then
    warn "Existing 'build' directory found. Removing it..."
    rm -rf build
fi

log "Creating build directory..."
mkdir -p build && cd build

log "Running CMake to generate Xcode project..."
cmake .. -GXcode

log "Building fs and generating .tbd file..."
cmake --build . --target generate_tbd

log "Building fs and generating .xcframework..."
cmake --build . --target generate_xcframework

log "Building ScribbleEditorKit dynamic library..."
cmake --build . --target ScribbleEditorKit

END_TIME=$(date +%s%3N)
EXECUTION_TIME=$((END_TIME - START_TIME))
SECONDS=$((EXECUTION_TIME / 1000))
MILLISECONDS=$((EXECUTION_TIME % 1000))

success "✅ Build complete!"
log "Total execution time: ${BOLD}${SECONDS}.${MILLISECONDS}s${RESET}"