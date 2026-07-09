#!/bin/bash
# Usage: build.sh [--cuda] [--no-cache]

set -e

TARGET="cpu"
NO_CACHE=""

while [[ $# -gt 0 ]]; do
  case $1 in
    --base)     TARGET="base"; shift ;;
    --cuda)     TARGET="cuda"; shift ;;
    --no-cache) NO_CACHE="--no-cache"; shift ;;
    *) echo "Unknown option: $1"; exit 1 ;;
  esac
done

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TAG="kd_slam2:$TARGET"

echo "=== Building $TAG ==="

docker build \
    $NO_CACHE \
    --target $TARGET \
    -t $TAG \
    -f "$SCRIPT_DIR/Dockerfile" \
    "$SCRIPT_DIR/.."

echo "=== Done: $TAG ==="
