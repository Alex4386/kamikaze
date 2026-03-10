#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
CACHE_DIR="${CACHE_DIR:-$PROJECT_DIR/ci/.cache}"
NETBOOT_URL="https://releases.ubuntu.com/releases/noble/netboot/amd64"
KERNEL_FILE="$CACHE_DIR/linux"
QEMU_TIMEOUT="${QEMU_TIMEOUT:-10}"
QEMU_LOG="$PROJECT_DIR/ci/qemu.log"

info() { echo "=> $*"; }
fail() { echo "!! $*" >&2; exit 1; }

# ---------------------------------------------------------------------------
# 1. Download the netboot kernel (cached)
# ---------------------------------------------------------------------------
download_kernel() {
    mkdir -p "$CACHE_DIR"

    if [ -f "$KERNEL_FILE" ]; then
        info "Using cached kernel: $KERNEL_FILE"
        return
    fi

    info "Downloading Ubuntu Noble netboot kernel..."
    curl -fSL -o "$KERNEL_FILE" "$NETBOOT_URL/linux" \
        || fail "Failed to download kernel from $NETBOOT_URL/linux"
    info "Kernel downloaded."
}

# ---------------------------------------------------------------------------
# 2. Build the project
# ---------------------------------------------------------------------------
build() {
    info "Building kamikaze..."
    make -C "$PROJECT_DIR" clean
    make -C "$PROJECT_DIR" cpio
    [ -f "$PROJECT_DIR/kamikaze.cpio" ] || fail "cpio build failed"
    info "Build complete."
}

# ---------------------------------------------------------------------------
# 3. Boot QEMU and verify output
# ---------------------------------------------------------------------------
run_qemu() {
    info "Booting QEMU (timeout=${QEMU_TIMEOUT}s)..."
    rm -f "$QEMU_LOG"

    timeout "$QEMU_TIMEOUT" \
        qemu-system-x86_64 \
            -kernel "$KERNEL_FILE" \
            -initrd "$PROJECT_DIR/kamikaze.cpio" \
            -append "console=ttyS0 earlyprintk=serial,ttyS0 panic=-1 printk.synchronous=1" \
            -nographic \
            -no-reboot \
            -m 256 \
        > "$QEMU_LOG" 2>&1 || true   # QEMU exits non-zero on guest crash

    info "QEMU exited. Checking log..."

    if grep -q "kamikaze" "$QEMU_LOG"; then
        info "SUCCESS: Found kamikaze output in kernel log."
        grep "kamikaze" "$QEMU_LOG"
    else
        info "QEMU log (last 30 lines):"
        tail -30 "$QEMU_LOG"
        fail "kamikaze message not found in QEMU output."
    fi
}

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
download_kernel
build
run_qemu

info "CI passed."
