# Task 00 – Initialize Build Tooling & Quality Pipeline

This task prepares the modern-manager branch for long-term improvement and contribution.

## Objective
- Add formatting and lint configuration
- Add baseline CI pipeline for cross-platform builds
- Scaffold future regression detection using golden seed tracking

## Files to Create
- `.clang-format`, `.clang-tidy`, `.editorconfig`
- `.github/workflows/build.yml`
- `tools/golden/README.md`
- `docs/dev/pipeline.md`

## Naming Rules
- Avoid automation-specific naming in commits, branches, or docs.
- Ensure all changes appear as native to the project.

## Commit Message
chore: bootstrap CI matrix, style config, and golden seed scaffold
