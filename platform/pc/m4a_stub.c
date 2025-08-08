#include "global.h"
#include "gba/m4a_internal.h"

// Minimal stubs for the m4a sound engine to satisfy the linker on PC builds.
// These perform no audio logic.

// Tables expected by m4a.c
const struct Song gSongTable[] = { {0} };
const struct MusicPlayer gMPlayTable[] = { {0} };
const struct ToneData voicegroup_dummy = {0};

// Core engine routines
void MPlayMain(struct MusicPlayerInfo *mplayInfo) { (void)mplayInfo; }
void MPlayJumpTableCopy(MPlayFunc *table) { (void)table; }
void TrackStop(struct MusicPlayerInfo *mplayInfo, struct MusicPlayerTrack *track) { (void)mplayInfo; (void)track; }
void SoundMain(void) {}
char SoundMainRAM[1];
void RealClearChain(void *x) { (void)x; }
void SoundMainBTM(void) {}

// Utility helpers
u32 umul3232H32(u32 a, u32 b) { (void)a; (void)b; return 0; }

// Player command handlers
#define STUB_PLY(name, ...) void name(__VA_ARGS__) { (void)0; }

STUB_PLY(ply_fine, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_goto, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_patt, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_pend, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_rept, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_prio, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_tempo, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_keysh, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_voice, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_vol, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_pan, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_bend, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_bendr, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_lfos, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_lfodl, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_mod, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_modt, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_tune, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_port, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_endtie, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
STUB_PLY(ply_note, u32 note_cmd, struct MusicPlayerInfo *a, struct MusicPlayerTrack *b);
