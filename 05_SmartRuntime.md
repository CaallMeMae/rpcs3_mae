# 05 Smart Runtime

## Goal
Introduce the Smart Runtime system for game fingerprint detection and preset selection.

## Background
- Detect game via hash/fingerprint.
- Load per-title configuration profiles (SPU/PPU/RSX).
- Auto-enable features: LLVM baking, async warmup, thread pinning.

## Tasks
1. **Fingerprint generator**
   - Hash `EBOOT.BIN` and `PARAM.SFO` to produce a 128-bit ID.
   - Store fingerprint in `cache/fingerprints/<id>.bin`.
2. **Preset database**
   - Maintain `presets/<fingerprint>.yml` with SPU/PPU/RSX tuning, thread pinning, and JIT options.
3. **Launch flow**
   - Compute fingerprint on boot.
   - Load matching preset and warm caches.
   - Apply thread pinning and async warmup.
4. **CLI options**
   - `--disable-smart-runtime` bypasses preset auto-load.
   - `--preset-override=/path/to/preset.yml` forces a specific preset.
5. **Telemetry**
   - Log fingerprint and preset choice to `logs/smart_runtime.log`.

## Validation
All commands must run headless; no prompts unless `--interactive` is passed.
```bash
# Build and run unit tests
cmake -B build && cmake --build build --target smart_runtime_tests
./build/smart_runtime_tests

# Dry-run fingerprint detection
./rpcs3 --dry-run /path/to/game | grep 'Smart runtime preset'

# Static validation of preset files
python3 tools/validate_presets.py presets/*.yml
```

## Success Criteria
- `--dry-run` outputs `Smart runtime preset: <name>` for known titles.
- Unit tests exit with code `0`.
- `validate_presets.py` reports no warnings.
