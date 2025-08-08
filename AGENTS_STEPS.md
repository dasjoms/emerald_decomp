# Implementation Steps

The following guide expands the project roadmap into actionable tasks. Follow the steps sequentially; later steps assume earlier ones are complete.

## 0. Baseline
- Keep the original ROM build working. All PC-specific code lives under `platform/pc/` and is compiled only when `PLATFORM_PC` is defined.
- Use the existing upstream layout so that rebasing remains possible. Avoid modifying files outside of the documented changes.

## 1. Native PC Port

### 1.1 Prepare build environment
1. Install development libraries: `SDL2`, `SDL2_image`, `SDL2_mixer`.
2. Add a top-level make variable `PLATFORM` defaulting to `gba`. When set to `pc`, define `-DPLATFORM_PC` in `CFLAGS`.
3. Ensure the host compiler (`cc` or `gcc`) is used instead of `arm-none-eabi-gcc` when `PLATFORM=pc`.

### 1.2 Create platform layer
1. Create `platform/pc/` with the following files:
   - `platform.h` – declarations shared by platform modules.
   - `main.c` – entry point with `int main(int argc, char **argv)`.
   - `video.c` / `video.h` – manages 240×160 framebuffer, palettes, and blitting.
   - `audio.c` / `audio.h` – initialises SDL audio and mixes PCM streams.
   - `input.c` / `input.h` – translates SDL keyboard/gamepad events to GBA key masks.
   - `timer.c` / `timer.h` – 60 FPS VBlank emulation using SDL timers.
   - `fs.c` / `fs.h` – file helpers for asset and save paths.
2. Provide `PlatformInit`, `PlatformRunFrame`, and `PlatformShutdown` functions orchestrating the subsystems.

### 1.3 Extract game loop
1. In `src/main.c` lines 89‑168, split `AgbMain` so that the core loop becomes `GameLoop` callable from both platforms.
2. For the GBA build, keep the existing `AgbMain` wrapper.
3. In `platform/pc/main.c`, after `PlatformInit`, call `GameLoop` each frame until it returns.

### 1.4 Header shims
1. Under `platform/pc/`, create lightweight replacements for `gba/gba.h` and related headers (`types.h`, `defines.h`, `io_reg.h`).
2. Provide typedefs (`u8`, `u16`, `u32`) and macros required by `include/global.h` lines 4‑17 without mapping to real hardware.
3. Update `include/global.h` to include these shims when `PLATFORM_PC` is defined.

### 1.5 Abstraction hooks
1. Search for direct register writes in `src/main.c` (e.g., `REG_DISPCNT`, `REG_WAITCNT`, palette setup at lines 96‑107).
2. Replace each with a call into the platform layer (`PlatformSetVideoMode`, `PlatformLoadPalette`, etc.).
3. Redirect functions like `ReadKeys`, `WaitForVBlank`, and interrupt initialisation to their SDL-backed counterparts in `platform/pc/`.

### 1.6 System call replacements
1. Re‑implement routines from `libagbsyscall/libagbsyscall.s` in C within `platform/pc/syscalls.c`:
   - `IntrWait` → wait for `PlatformWaitForVBlank` or timer events.
   - `VBlankIntrWait` → same as `IntrWait`.
   - `RegisterRamReset` → memory reset or stub.
   - `CpuFastSet`/`CpuSet` → `memcpy`/`memset` equivalents.
   - `LZ77UnCompVram`/`LZ77UnCompWram` → reuse existing decompression code.
2. Exclude the original assembly file from the PC build.

### 1.7 Runtime asset loading
1. Identify `INCBIN` usages (e.g., `src/battle_factory_screen.c` and `src/anim_mon_front_pics.c`).
2. Replace with loader functions in `platform/pc/assets.c` that read from `graphics/`, `sound/`, and `data/` at runtime.
3. Use `SDL_image` to decode PNGs; for legacy `.4bpp`/`.pal` assets, decompress and convert to textures in memory.
4. Cache loaded assets to avoid redundant disk I/O.

### 1.8 Input and save handling
1. Map GBA button constants (`A_BUTTON`, `B_BUTTON`, etc.) to SDL keys or controller buttons inside `input.c`.
2. Update the game’s `gMain` structure each frame based on SDL events.
3. Implement save routines in `fs.c` that write to `saves/slot#.sav` instead of flash memory.

### 1.9 Host build system
1. Extend `Makefile` with a `pc` target:
   - Use host compiler and flags from `sdl2-config`.
   - Compile existing C sources while excluding `.s` files.
   - Add `platform/pc/*.c` to the source list.
   - Link against SDL2, SDL2_image, and SDL2_mixer.
2. Output `pokeemerald` (or `pokeemerald.exe` on Windows) in the repository root.
3. Document the new build in `README.md`.

## 2. Streamlined Pokémon Addition

### 2.1 PNG‑based sprites
1. Treat `front.png`, `back.png`, and `icon.png` in each species folder as authoritative.
2. In the absence of PNGs, fall back to existing `.4bpp`/`.pal` files and convert at runtime.
3. Expose helper `LoadPokemonSprite(speciesId, form, view)` returning textures usable by the rendering code.

### 2.2 Dynamic discovery
1. On startup, scan `graphics/pokemon/` and `mods/pokemon/` to discover species directories.
2. Build a registry mapping species names to asset paths and later to numeric IDs.
3. Log any missing required files so mod creators can diagnose problems.

### 2.3 External metadata
1. Define a JSON or YAML schema describing stats, types, abilities, movesets, and evolutions for each species.
2. Place metadata files alongside sprites (`graphics/pokemon/<name>/species.json`).
3. Write `tools/gen_species_data.py` to parse all metadata, validate fields, and emit:
   - `include/constants/species.h` with generated `SPECIES_*` macros.
   - `src/data/pokemon/species_info.c` and other tables consumed by the game.

### 2.4 Automatic ID allocation
1. During generation, assign IDs sequentially based on discovery order when none are specified.
2. Preserve existing IDs for canonical species to maintain save compatibility.
3. Resize generated arrays automatically so adding species does not require manual edits.

### 2.5 Modular extension support
1. Support a secondary `mods/pokemon/` tree loaded after core assets.
2. Give mod species IDs after the canonical range or within a configurable offset.
3. Provide a way to enable/disable mods via a config file or command‑line switch.

### 2.6 Documented workflow
1. Create `docs/adding_pokemon.md` describing the mod workflow:
   - Create species folder with PNGs and metadata.
   - Run `python tools/gen_species_data.py` to regenerate headers and tables.
   - Build the PC executable (`make pc`).
   - Launch the game to test the new Pokémon.

## 3. Maintenance Notes
- Keep GBA and PC code paths separated with `#ifdef PLATFORM_PC`/`#else`/`#endif`.
- Mirror upstream changes when possible; avoid diverging formatting or style.
- Expand this document as new hurdles appear during implementation.

