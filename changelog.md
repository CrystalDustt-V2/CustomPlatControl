# 1.0.2
- Updated for Geode v5.0.0-beta.4.
- Updated mod metadata to beta.4 schema and added `requires-patching`.
- Removed obsolete beta.3 MSVC compatibility workaround after beta.4 dependency updates.
- Verified input/state handling remains stable and unchanged (platformer-only override, held-input persistence).

# 1.0.1
- Updated for Geode v5.0.0-beta.3.
- Migrated to the new keybind category system.
- Implemented listener priority support.
- Improved input handling compatibility.
- Replaced deprecated key modifier API usage with beta.3 input types.
- Added keybind migration metadata to preserve old custom keybind configs.

# 1.0.0
- Initial release of CustomPlatControl.
- Added Platformer-only remapping for Player 1 and Player 2 (Left, Jump, Right).
- Added persistent Geode keybind settings for all 6 controls.
- Added held-key tracking so input keeps working through death and respawn.
- Classic mode controls are unchanged.
