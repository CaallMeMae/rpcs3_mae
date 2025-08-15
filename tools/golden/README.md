# Golden Seeds

This folder will store curated runtime data used for regression detection.

## What we’ll record:
- Time to first frame (ms)
- First-frame image hash (CRC)
- Shader count at frame 1
- Emulated system metrics

## Purpose:
This ensures future performance or rendering changes do not silently break compatibility or user experience.

Golden seeds will be tracked per-title or per-test-case in minimal reproducible scenes.
