# Project Goal

This fork aims to transform the Pokémon Emerald decompilation into a desktop application and streamline the addition of custom Pokémon. Two broad objectives drive the work:

1. **Native PC build** – Compile the existing game sources for a host platform instead of generating a Game Boy Advance ROM. A new platform layer (using SDL or a similar library) will supply video, audio, input, and timing services so that most of the original gameplay code can run unchanged.
2. **Flexible Pokémon pipeline** – Replace the ROM‐centric asset workflow with data and image files that can be discovered at runtime. Loading PNG graphics and external metadata will remove the need to hand‑edit large tables and convert palettes, making it easy to add or modify species.

All new platform and tooling code lives outside of the original GBA build, preserving the ability to assemble the ROM while enabling experimentation on desktop systems.

# Tests

Do NOT try to "make" the ROM yourself as it goes against the project's goal.

Your tests should be:

'cc -Iinclude -DPLATFORM_PC -c src/main.c -o /tmp/main.o'

and:

'make PLATFORM=pc'

and:

`cc $(find build/pc -name '*.o') $(sdl2-config --libs) -lSDL2_image -lSDL2_mixer -o pokeemerald.exe`

If any of them fail, investigate the issue and implement a full fix without compromising any of the original game's features. Rerun tests afterwards to verify your fix worked. If it didn't, reinvestigate why and implement the proper solution.
