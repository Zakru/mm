/*
 * File: z_en_pm.c
 * Overlay: ovl_En_Pm
 * Description: Postman
 */

#include "z_en_pm.h"
#include "overlays/actors/ovl_En_Door/z_en_door.h"
#include "objects/object_mm/object_mm.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8 | ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((EnPm*)thisx)

void EnPm_Init(Actor* thisx, PlayState* play);
void EnPm_Destroy(Actor* thisx, PlayState* play);
void EnPm_Update(Actor* thisx, PlayState* play);
void EnPm_Draw(Actor* thisx, PlayState* play);

void func_80AFA4D0(EnPm* this, PlayState* play);
void func_80AFA5FC(EnPm* this, PlayState* play);

static u8 D_80AFAD80[] = {
    /* 0x000 */ SCHEDULE_CMD_CHECK_NOT_IN_DAY_L(1, 0x240 - 0x005),
    /* 0x005 */ SCHEDULE_CMD_CHECK_TIME_RANGE_L(9, 0, 12, 0, 0x117 - 0x00C),
    /* 0x00C */ SCHEDULE_CMD_CHECK_TIME_RANGE_L(12, 0, 13, 1, 0x10C - 0x013),
    /* 0x013 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 1, 15, 0, 0x02B - 0x019),
    /* 0x019 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(15, 0, 0, 5, 0x020 - 0x01F),
    /* 0x01F */ SCHEDULE_CMD_RET_NONE(),
    /* 0x020 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_POSTHOUSE, 0x02A - 0x024),
    /* 0x024 */ SCHEDULE_CMD_RET_TIME(15, 0, 0, 5, 8),
    /* 0x02A */ SCHEDULE_CMD_RET_NONE(),
    /* 0x02B */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_ICHIBA, 0x060 - 0x02F),
    /* 0x02F */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 1, 13, 4, 0x05A - 0x035),
    /* 0x035 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 4, 13, 21, 0x054 - 0x03B),
    /* 0x03B */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 48, 14, 56, 0x04E - 0x041),
    /* 0x041 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 56, 15, 0, 0x048 - 0x047),
    /* 0x047 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x048 */ SCHEDULE_CMD_RET_TIME(14, 56, 15, 0, 2),
    /* 0x04E */ SCHEDULE_CMD_RET_TIME(14, 48, 14, 56, 62),
    /* 0x054 */ SCHEDULE_CMD_RET_TIME(13, 4, 13, 21, 53),
    /* 0x05A */ SCHEDULE_CMD_RET_TIME(13, 1, 13, 4, 1),
    /* 0x060 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_CLOCKTOWER, 0x095 - 0x064),
    /* 0x064 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 4, 13, 21, 0x08F - 0x06A),
    /* 0x06A */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 21, 13, 39, 0x089 - 0x070),
    /* 0x070 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 38, 14, 48, 0x083 - 0x076),
    /* 0x076 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 48, 14, 56, 0x07D - 0x07C),
    /* 0x07C */ SCHEDULE_CMD_RET_NONE(),
    /* 0x07D */ SCHEDULE_CMD_RET_TIME(14, 48, 14, 56, 61),
    /* 0x083 */ SCHEDULE_CMD_RET_TIME(14, 38, 14, 48, 60),
    /* 0x089 */ SCHEDULE_CMD_RET_TIME(13, 21, 13, 39, 55),
    /* 0x08F */ SCHEDULE_CMD_RET_TIME(13, 4, 13, 21, 54),
    /* 0x095 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_TOWN, 0x0CA - 0x099),
    /* 0x099 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 39, 13, 56, 0x0C4 - 0x09F),
    /* 0x09F */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 56, 14, 0, 0x0BE - 0x0A5),
    /* 0x0A5 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 26, 14, 30, 0x0B8 - 0x0AB),
    /* 0x0AB */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 30, 14, 38, 0x0B2 - 0x0B1),
    /* 0x0B1 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x0B2 */ SCHEDULE_CMD_RET_TIME(14, 30, 14, 38, 59),
    /* 0x0B8 */ SCHEDULE_CMD_RET_TIME(14, 26, 14, 30, 11),
    /* 0x0BE */ SCHEDULE_CMD_RET_TIME(13, 56, 14, 0, 10),
    /* 0x0C4 */ SCHEDULE_CMD_RET_TIME(13, 39, 13, 56, 56),
    /* 0x0CA */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_YADOYA, 0x10B - 0x0CE),
    /* 0x0CE */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 56, 14, 0, 0x105 - 0x0D4),
    /* 0x0D4 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 0, 14, 8, 0x0FF - 0x0DA),
    /* 0x0DA */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 8, 14, 18, 0x0F9 - 0x0E0),
    /* 0x0E0 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 18, 14, 26, 0x0F3 - 0x0E6),
    /* 0x0E6 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 26, 14, 30, 0x0ED - 0x0EC),
    /* 0x0EC */ SCHEDULE_CMD_RET_NONE(),
    /* 0x0ED */ SCHEDULE_CMD_RET_TIME(14, 26, 14, 30, 13),
    /* 0x0F3 */ SCHEDULE_CMD_RET_TIME(14, 18, 14, 26, 58),
    /* 0x0F9 */ SCHEDULE_CMD_RET_TIME(14, 8, 14, 18, 16),
    /* 0x0FF */ SCHEDULE_CMD_RET_TIME(14, 0, 14, 8, 57),
    /* 0x105 */ SCHEDULE_CMD_RET_TIME(13, 56, 14, 0, 12),
    /* 0x10B */ SCHEDULE_CMD_RET_NONE(),
    /* 0x10C */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_POSTHOUSE, 0x116 - 0x110),
    /* 0x110 */ SCHEDULE_CMD_RET_TIME(12, 0, 13, 1, 9),
    /* 0x116 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x117 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_ICHIBA, 0x14C - 0x11B),
    /* 0x11B */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(9, 0, 9, 3, 0x146 - 0x121),
    /* 0x121 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(9, 3, 9, 17, 0x140 - 0x127),
    /* 0x127 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(11, 43, 11, 57, 0x13A - 0x12D),
    /* 0x12D */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(11, 57, 12, 0, 0x134 - 0x133),
    /* 0x133 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x134 */ SCHEDULE_CMD_RET_TIME(11, 57, 12, 0, 2),
    /* 0x13A */ SCHEDULE_CMD_RET_TIME(11, 43, 11, 57, 52),
    /* 0x140 */ SCHEDULE_CMD_RET_TIME(9, 3, 9, 17, 38),
    /* 0x146 */ SCHEDULE_CMD_RET_TIME(9, 0, 9, 3, 1),
    /* 0x14C */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_CLOCKTOWER, 0x1B1 - 0x150),
    /* 0x150 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(9, 3, 9, 17, 0x1AB - 0x156),
    /* 0x156 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(9, 17, 9, 31, 0x1A5 - 0x15C),
    /* 0x15C */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(9, 31, 9, 35, 0x19F - 0x162),
    /* 0x162 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(9, 35, 9, 49, 0x199 - 0x168),
    /* 0x168 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(10, 57, 11, 11, 0x193 - 0x16E),
    /* 0x16E */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(11, 11, 11, 25, 0x18D - 0x174),
    /* 0x174 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(11, 25, 11, 29, 0x187 - 0x17A),
    /* 0x17A */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(11, 29, 11, 43, 0x181 - 0x180),
    /* 0x180 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x181 */ SCHEDULE_CMD_RET_TIME(11, 29, 11, 43, 50),
    /* 0x187 */ SCHEDULE_CMD_RET_TIME(11, 25, 11, 29, 7),
    /* 0x18D */ SCHEDULE_CMD_RET_TIME(11, 11, 11, 25, 48),
    /* 0x193 */ SCHEDULE_CMD_RET_TIME(10, 57, 11, 11, 47),
    /* 0x199 */ SCHEDULE_CMD_RET_TIME(9, 35, 9, 49, 41),
    /* 0x19F */ SCHEDULE_CMD_RET_TIME(9, 31, 9, 35, 3),
    /* 0x1A5 */ SCHEDULE_CMD_RET_TIME(9, 17, 9, 31, 40),
    /* 0x1AB */ SCHEDULE_CMD_RET_TIME(9, 3, 9, 17, 39),
    /* 0x1B1 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_BACKTOWN, 0x1DA - 0x1B5),
    /* 0x1B5 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(9, 49, 10, 3, 0x1D4 - 0x1BB),
    /* 0x1BB */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(10, 3, 10, 7, 0x1CE - 0x1C1),
    /* 0x1C1 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(10, 7, 10, 21, 0x1C8 - 0x1C7),
    /* 0x1C7 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x1C8 */ SCHEDULE_CMD_RET_TIME(10, 7, 10, 21, 43),
    /* 0x1CE */ SCHEDULE_CMD_RET_TIME(10, 3, 10, 7, 4),
    /* 0x1D4 */ SCHEDULE_CMD_RET_TIME(9, 49, 10, 3, 42),
    /* 0x1DA */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_TOWN, 0x23F - 0x1DE),
    /* 0x1DE */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(10, 21, 10, 35, 0x239 - 0x1E4),
    /* 0x1E4 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(10, 35, 10, 39, 0x233 - 0x1EA),
    /* 0x1EA */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(10, 39, 10, 53, 0x22D - 0x1F0),
    /* 0x1F0 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(10, 53, 10, 57, 0x227 - 0x1F6),
    /* 0x1F6 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(10, 57, 11, 11, 0x221 - 0x1FC),
    /* 0x1FC */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(11, 11, 11, 25, 0x21B - 0x202),
    /* 0x202 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(11, 25, 11, 29, 0x215 - 0x208),
    /* 0x208 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(11, 29, 11, 43, 0x20F - 0x20E),
    /* 0x20E */ SCHEDULE_CMD_RET_NONE(),
    /* 0x20F */ SCHEDULE_CMD_RET_TIME(11, 29, 11, 43, 51),
    /* 0x215 */ SCHEDULE_CMD_RET_TIME(11, 25, 11, 29, 7),
    /* 0x21B */ SCHEDULE_CMD_RET_TIME(11, 11, 11, 25, 49),
    /* 0x221 */ SCHEDULE_CMD_RET_TIME(10, 57, 11, 11, 46),
    /* 0x227 */ SCHEDULE_CMD_RET_TIME(10, 53, 10, 57, 6),
    /* 0x22D */ SCHEDULE_CMD_RET_TIME(10, 39, 10, 53, 45),
    /* 0x233 */ SCHEDULE_CMD_RET_TIME(10, 35, 10, 39, 5),
    /* 0x239 */ SCHEDULE_CMD_RET_TIME(10, 21, 10, 35, 44),
    /* 0x23F */ SCHEDULE_CMD_RET_NONE(),
    /* 0x240 */ SCHEDULE_CMD_CHECK_NOT_IN_DAY_L(2, 0x414 - 0x245),
    /* 0x245 */ SCHEDULE_CMD_CHECK_FLAG_L(WEEKEVENTREG_28_08, 0x2EF - 0x24A),
    /* 0x24A */ SCHEDULE_CMD_CHECK_TIME_RANGE_L(9, 0, 12, 0, 0x117 - 0x251),
    /* 0x251 */ SCHEDULE_CMD_CHECK_TIME_RANGE_L(12, 0, 13, 1, 0x2E4 - 0x258),
    /* 0x258 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 1, 15, 0, 0x2DC - 0x25E),
    /* 0x25E */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 0, 2, 0, 0x265 - 0x264),
    /* 0x264 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x265 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_ICHIBA, 0x29A - 0x269),
    /* 0x269 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 0, 0, 4, 0x294 - 0x26F),
    /* 0x26F */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 4, 0, 31, 0x28E - 0x275),
    /* 0x275 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(1, 29, 1, 56, 0x288 - 0x27B),
    /* 0x27B */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(1, 56, 2, 0, 0x282 - 0x281),
    /* 0x281 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x282 */ SCHEDULE_CMD_RET_TIME(1, 56, 2, 0, 2),
    /* 0x288 */ SCHEDULE_CMD_RET_TIME(1, 29, 1, 56, 79),
    /* 0x28E */ SCHEDULE_CMD_RET_TIME(0, 4, 0, 31, 74),
    /* 0x294 */ SCHEDULE_CMD_RET_TIME(0, 0, 0, 4, 1),
    /* 0x29A */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_CLOCKTOWER, 0x2DB - 0x29E),
    /* 0x29E */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 4, 0, 31, 0x2D5 - 0x2A4),
    /* 0x2A4 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 31, 0, 58, 0x2CF - 0x2AA),
    /* 0x2AA */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 58, 1, 2, 0x2C9 - 0x2B0),
    /* 0x2B0 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(1, 2, 1, 29, 0x2C3 - 0x2B6),
    /* 0x2B6 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(1, 29, 1, 56, 0x2BD - 0x2BC),
    /* 0x2BC */ SCHEDULE_CMD_RET_NONE(),
    /* 0x2BD */ SCHEDULE_CMD_RET_TIME(1, 29, 1, 56, 78),
    /* 0x2C3 */ SCHEDULE_CMD_RET_TIME(1, 2, 1, 29, 77),
    /* 0x2C9 */ SCHEDULE_CMD_RET_TIME(0, 58, 1, 2, 18),
    /* 0x2CF */ SCHEDULE_CMD_RET_TIME(0, 31, 0, 58, 76),
    /* 0x2D5 */ SCHEDULE_CMD_RET_TIME(0, 4, 0, 31, 75),
    /* 0x2DB */ SCHEDULE_CMD_RET_NONE(),
    /* 0x2DC */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_POSTHOUSE, 0x2E3 - 0x2E0),
    /* 0x2E0 */ SCHEDULE_CMD_RET_VAL_L(14),
    /* 0x2E3 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x2E4 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_POSTHOUSE, 0x2EE - 0x2E8),
    /* 0x2E8 */ SCHEDULE_CMD_RET_TIME(12, 0, 13, 1, 20),
    /* 0x2EE */ SCHEDULE_CMD_RET_NONE(),
    /* 0x2EF */ SCHEDULE_CMD_CHECK_TIME_RANGE_L(9, 0, 12, 0, 0x117 - 0x2F6),
    /* 0x2F6 */ SCHEDULE_CMD_CHECK_TIME_RANGE_L(12, 0, 13, 1, 0x409 - 0x2FD),
    /* 0x2FD */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 1, 17, 0, 0x315 - 0x303),
    /* 0x303 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(17, 0, 0, 5, 0x30A - 0x309),
    /* 0x309 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x30A */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_POSTHOUSE, 0x314 - 0x30E),
    /* 0x30E */ SCHEDULE_CMD_RET_TIME(17, 0, 0, 5, 8),
    /* 0x314 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x315 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_ICHIBA, 0x34A - 0x319),
    /* 0x319 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 1, 13, 4, 0x344 - 0x31F),
    /* 0x31F */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 4, 13, 15, 0x33E - 0x325),
    /* 0x325 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(16, 35, 16, 57, 0x338 - 0x32B),
    /* 0x32B */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(16, 57, 17, 0, 0x332 - 0x331),
    /* 0x331 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x332 */ SCHEDULE_CMD_RET_TIME(16, 57, 17, 0, 2),
    /* 0x338 */ SCHEDULE_CMD_RET_TIME(16, 35, 16, 57, 73),
    /* 0x33E */ SCHEDULE_CMD_RET_TIME(13, 4, 13, 15, 63),
    /* 0x344 */ SCHEDULE_CMD_RET_TIME(13, 1, 13, 4, 1),
    /* 0x34A */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_CLOCKTOWER, 0x37F - 0x34E),
    /* 0x34E */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 4, 13, 15, 0x379 - 0x354),
    /* 0x354 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 15, 13, 30, 0x373 - 0x35A),
    /* 0x35A */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 52, 14, 45, 0x36D - 0x360),
    /* 0x360 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(16, 15, 16, 35, 0x367 - 0x366),
    /* 0x366 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x367 */ SCHEDULE_CMD_RET_TIME(16, 15, 16, 35, 72),
    /* 0x36D */ SCHEDULE_CMD_RET_TIME(13, 52, 14, 45, 67),
    /* 0x373 */ SCHEDULE_CMD_RET_TIME(13, 15, 13, 30, 65),
    /* 0x379 */ SCHEDULE_CMD_RET_TIME(13, 4, 13, 15, 64),
    /* 0x37F */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_TOWN, 0x39C - 0x383),
    /* 0x383 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 30, 13, 52, 0x396 - 0x389),
    /* 0x389 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 52, 14, 50, 0x390 - 0x38F),
    /* 0x38F */ SCHEDULE_CMD_RET_NONE(),
    /* 0x390 */ SCHEDULE_CMD_RET_TIME(13, 52, 14, 50, 68),
    /* 0x396 */ SCHEDULE_CMD_RET_TIME(13, 30, 13, 52, 66),
    /* 0x39C */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_ALLEY, 0x408 - 0x3A0),
    /* 0x3A0 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 45, 15, 5, 0x402 - 0x3A6),
    /* 0x3A6 */ SCHEDULE_CMD_CHECK_NOT_IN_DAY_S(2, 0x3DB - 0x3AA),
    /* 0x3AA */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(15, 5, 15, 20, 0x3D5 - 0x3B0),
    /* 0x3B0 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(15, 20, 15, 25, 0x3CF - 0x3B6),
    /* 0x3B6 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(15, 25, 15, 40, 0x3C9 - 0x3BC),
    /* 0x3BC */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(15, 40, 16, 15, 0x3C3 - 0x3C2),
    /* 0x3C2 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x3C3 */ SCHEDULE_CMD_RET_TIME(15, 40, 16, 15, 81),
    /* 0x3C9 */ SCHEDULE_CMD_RET_TIME(15, 25, 15, 40, 17),
    /* 0x3CF */ SCHEDULE_CMD_RET_TIME(15, 20, 15, 25, 80),
    /* 0x3D5 */ SCHEDULE_CMD_RET_TIME(15, 5, 15, 20, 23),
    /* 0x3DB */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(15, 5, 15, 20, 0x3FC - 0x3E1),
    /* 0x3E1 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(15, 20, 15, 50, 0x3F6 - 0x3E7),
    /* 0x3E7 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(15, 50, 16, 15, 0x3F0 - 0x3ED),
    /* 0x3ED */ SCHEDULE_CMD_RET_VAL_L(0),
    /* 0x3F0 */ SCHEDULE_CMD_RET_TIME(15, 50, 16, 15, 71),
    /* 0x3F6 */ SCHEDULE_CMD_RET_TIME(15, 20, 15, 50, 25),
    /* 0x3FC */ SCHEDULE_CMD_RET_TIME(15, 5, 15, 20, 23),
    /* 0x402 */ SCHEDULE_CMD_RET_TIME(14, 45, 15, 5, 70),
    /* 0x408 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x409 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_POSTHOUSE, 0x413 - 0x40D),
    /* 0x40D */ SCHEDULE_CMD_RET_TIME(12, 0, 13, 1, 21),
    /* 0x413 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x414 */ SCHEDULE_CMD_CHECK_NOT_IN_DAY_L(3, 0x588 - 0x419),
    /* 0x419 */ SCHEDULE_CMD_CHECK_FLAG_L(WEEKEVENTREG_28_08, 0x55D - 0x41E),
    /* 0x41E */ SCHEDULE_CMD_CHECK_FLAG_L(WEEKEVENTREG_28_10, 0x52F - 0x423),
    /* 0x423 */ SCHEDULE_CMD_CHECK_TIME_RANGE_L(9, 0, 12, 0, 0x117 - 0x42A),
    /* 0x42A */ SCHEDULE_CMD_CHECK_TIME_RANGE_L(12, 0, 13, 1, 0x527 - 0x431),
    /* 0x431 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 1, 15, 0, 0x446 - 0x437),
    /* 0x437 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(18, 0, 6, 0, 0x43E - 0x43D),
    /* 0x43D */ SCHEDULE_CMD_RET_NONE(),
    /* 0x43E */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_POSTHOUSE, 0x445 - 0x442),
    /* 0x442 */ SCHEDULE_CMD_RET_VAL_L(24),
    /* 0x445 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x446 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_ICHIBA, 0x47B - 0x44A),
    /* 0x44A */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 0, 13, 1, 0x475 - 0x450),
    /* 0x450 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 5, 13, 15, 0x46F - 0x456),
    /* 0x456 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 45, 14, 55, 0x469 - 0x45C),
    /* 0x45C */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 55, 15, 0, 0x463 - 0x462),
    /* 0x462 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x463 */ SCHEDULE_CMD_RET_TIME(14, 55, 15, 0, 2),
    /* 0x469 */ SCHEDULE_CMD_RET_TIME(14, 45, 14, 55, 52),
    /* 0x46F */ SCHEDULE_CMD_RET_TIME(13, 5, 13, 15, 38),
    /* 0x475 */ SCHEDULE_CMD_RET_TIME(13, 0, 13, 1, 1),
    /* 0x47B */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_CLOCKTOWER, 0x4C8 - 0x47F),
    /* 0x47F */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 5, 13, 15, 0x4C2 - 0x485),
    /* 0x485 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 15, 13, 25, 0x4BC - 0x48B),
    /* 0x48B */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 25, 13, 35, 0x4B6 - 0x491),
    /* 0x491 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 15, 14, 25, 0x4B0 - 0x497),
    /* 0x497 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 25, 14, 35, 0x4AA - 0x49D),
    /* 0x49D */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 35, 14, 45, 0x4A4 - 0x4A3),
    /* 0x4A3 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x4A4 */ SCHEDULE_CMD_RET_TIME(14, 35, 14, 45, 50),
    /* 0x4AA */ SCHEDULE_CMD_RET_TIME(14, 25, 14, 35, 48),
    /* 0x4B0 */ SCHEDULE_CMD_RET_TIME(14, 15, 14, 25, 47),
    /* 0x4B6 */ SCHEDULE_CMD_RET_TIME(13, 25, 13, 35, 41),
    /* 0x4BC */ SCHEDULE_CMD_RET_TIME(13, 15, 13, 25, 40),
    /* 0x4C2 */ SCHEDULE_CMD_RET_TIME(13, 5, 13, 15, 39),
    /* 0x4C8 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_BACKTOWN, 0x4E5 - 0x4CC),
    /* 0x4CC */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 35, 13, 45, 0x4DF - 0x4D2),
    /* 0x4D2 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 45, 13, 55, 0x4D9 - 0x4D8),
    /* 0x4D8 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x4D9 */ SCHEDULE_CMD_RET_TIME(13, 45, 13, 55, 43),
    /* 0x4DF */ SCHEDULE_CMD_RET_TIME(13, 35, 13, 45, 42),
    /* 0x4E5 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_TOWN, 0x526 - 0x4E9),
    /* 0x4E9 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(13, 55, 14, 5, 0x520 - 0x4EF),
    /* 0x4EF */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 5, 14, 15, 0x51A - 0x4F5),
    /* 0x4F5 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 15, 14, 25, 0x514 - 0x4FB),
    /* 0x4FB */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 25, 14, 35, 0x50E - 0x501),
    /* 0x501 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(14, 35, 14, 45, 0x508 - 0x507),
    /* 0x507 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x508 */ SCHEDULE_CMD_RET_TIME(14, 35, 14, 45, 51),
    /* 0x50E */ SCHEDULE_CMD_RET_TIME(14, 25, 14, 35, 49),
    /* 0x514 */ SCHEDULE_CMD_RET_TIME(14, 15, 14, 25, 46),
    /* 0x51A */ SCHEDULE_CMD_RET_TIME(14, 5, 14, 15, 45),
    /* 0x520 */ SCHEDULE_CMD_RET_TIME(13, 55, 14, 5, 44),
    /* 0x526 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x527 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_POSTHOUSE, 0x52E - 0x52B),
    /* 0x52B */ SCHEDULE_CMD_RET_VAL_L(22),
    /* 0x52E */ SCHEDULE_CMD_RET_NONE(),
    /* 0x52F */ SCHEDULE_CMD_CHECK_TIME_RANGE_L(9, 0, 12, 0, 0x117 - 0x536),
    /* 0x536 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(12, 0, 13, 1, 0x552 - 0x53C),
    /* 0x53C */ SCHEDULE_CMD_CHECK_TIME_RANGE_L(13, 1, 17, 0, 0x315 - 0x543),
    /* 0x543 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(18, 0, 6, 0, 0x54A - 0x549),
    /* 0x549 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x54A */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_POSTHOUSE, 0x551 - 0x54E),
    /* 0x54E */ SCHEDULE_CMD_RET_VAL_L(24),
    /* 0x551 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x552 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_POSTHOUSE, 0x55C - 0x556),
    /* 0x556 */ SCHEDULE_CMD_RET_TIME(12, 0, 13, 1, 21),
    /* 0x55C */ SCHEDULE_CMD_RET_NONE(),
    /* 0x55D */ SCHEDULE_CMD_CHECK_TIME_RANGE_L(9, 0, 12, 0, 0x117 - 0x564),
    /* 0x564 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(12, 0, 13, 1, 0x580 - 0x56A),
    /* 0x56A */ SCHEDULE_CMD_CHECK_TIME_RANGE_L(13, 1, 15, 0, 0x446 - 0x571),
    /* 0x571 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(18, 0, 6, 0, 0x578 - 0x577),
    /* 0x577 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x578 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_POSTHOUSE, 0x57F - 0x57C),
    /* 0x57C */ SCHEDULE_CMD_RET_VAL_L(24),
    /* 0x57F */ SCHEDULE_CMD_RET_NONE(),
    /* 0x580 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_POSTHOUSE, 0x587 - 0x584),
    /* 0x584 */ SCHEDULE_CMD_RET_VAL_L(20),
    /* 0x587 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x588 */ SCHEDULE_CMD_RET_NONE(),
};

static u8 D_80AFB30C[] = {
    /* 0x00 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_POSTHOUSE, 0x32 - 0x04),
    /* 0x04 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 0, 0, 10, 0x2C - 0x0A),
    /* 0x0A */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 10, 0, 13, 0x29 - 0x10),
    /* 0x10 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 13, 0, 23, 0x23 - 0x16),
    /* 0x16 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 23, 0, 26, 0x1D - 0x1C),
    /* 0x1C */ SCHEDULE_CMD_RET_NONE(),
    /* 0x1D */ SCHEDULE_CMD_RET_TIME(0, 23, 0, 26, 32),
    /* 0x23 */ SCHEDULE_CMD_RET_TIME(0, 13, 0, 23, 83),
    /* 0x29 */ SCHEDULE_CMD_RET_VAL_L(27),
    /* 0x2C */ SCHEDULE_CMD_RET_TIME(0, 0, 0, 10, 82),
    /* 0x32 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_ICHIBA, 0x4F - 0x36),
    /* 0x36 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 23, 0, 26, 0x49 - 0x3C),
    /* 0x3C */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 26, 0, 36, 0x43 - 0x42),
    /* 0x42 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x43 */ SCHEDULE_CMD_RET_TIME(0, 26, 0, 36, 84),
    /* 0x49 */ SCHEDULE_CMD_RET_TIME(0, 23, 0, 26, 33),
    /* 0x4F */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_CLOCKTOWER, 0x6C - 0x53),
    /* 0x53 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 26, 0, 36, 0x66 - 0x59),
    /* 0x59 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 36, 0, 46, 0x60 - 0x5F),
    /* 0x5F */ SCHEDULE_CMD_RET_NONE(),
    /* 0x60 */ SCHEDULE_CMD_RET_TIME(0, 36, 0, 46, 86),
    /* 0x66 */ SCHEDULE_CMD_RET_TIME(0, 26, 0, 36, 85),
    /* 0x6C */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_TOWN, 0x89 - 0x70),
    /* 0x70 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 46, 0, 56, 0x83 - 0x76),
    /* 0x76 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 56, 0, 59, 0x7D - 0x7C),
    /* 0x7C */ SCHEDULE_CMD_RET_NONE(),
    /* 0x7D */ SCHEDULE_CMD_RET_TIME(0, 56, 0, 59, 34),
    /* 0x83 */ SCHEDULE_CMD_RET_TIME(0, 46, 0, 56, 87),
    /* 0x89 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_MILK_BAR, 0xB2 - 0x8D),
    /* 0x8D */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 56, 0, 59, 0xAC - 0x93),
    /* 0x93 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 59, 1, 9, 0xA6 - 0x99),
    /* 0x99 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(1, 9, 1, 40, 0xA0 - 0x9F),
    /* 0x9F */ SCHEDULE_CMD_RET_NONE(),
    /* 0xA0 */ SCHEDULE_CMD_RET_TIME(1, 9, 1, 40, 28),
    /* 0xA6 */ SCHEDULE_CMD_RET_TIME(0, 59, 1, 9, 88),
    /* 0xAC */ SCHEDULE_CMD_RET_TIME(0, 56, 0, 59, 36),
    /* 0xB2 */ SCHEDULE_CMD_RET_NONE(),
};

static u8 D_80AFB3C0[] = {
    /* 0x00 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_TOWN, 0x1D - 0x04),
    /* 0x04 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 10, 0, 13, 0x17 - 0x0A),
    /* 0x0A */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 13, 0, 23, 0x11 - 0x10),
    /* 0x10 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x11 */ SCHEDULE_CMD_RET_TIME(0, 13, 0, 23, 90),
    /* 0x17 */ SCHEDULE_CMD_RET_TIME(0, 10, 0, 13, 35),
    /* 0x1D */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_MILK_BAR, 0x3A - 0x21),
    /* 0x21 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 0, 0, 10, 0x34 - 0x27),
    /* 0x27 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 10, 0, 13, 0x2E - 0x2D),
    /* 0x2D */ SCHEDULE_CMD_RET_NONE(),
    /* 0x2E */ SCHEDULE_CMD_RET_TIME(0, 10, 0, 13, 37),
    /* 0x34 */ SCHEDULE_CMD_RET_TIME(0, 0, 0, 10, 89),
    /* 0x3A */ SCHEDULE_CMD_RET_NONE(),
};

static u8 D_80AFB3FC[] = {
    /* 0x00 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_TOWN, 0x1D - 0x04),
    /* 0x04 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(18, 0, 6, 0, 0x17 - 0x0A),
    /* 0x0A */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(5, 0, 5, 10, 0x11 - 0x10),
    /* 0x10 */ SCHEDULE_CMD_RET_NONE(),
    /* 0x11 */ SCHEDULE_CMD_RET_TIME(5, 0, 5, 10, 91),
    /* 0x17 */ SCHEDULE_CMD_RET_TIME(18, 0, 6, 0, 29),
    /* 0x1D */ SCHEDULE_CMD_RET_NONE(),
};

static u8 D_80AFB41C[] = {
    /* 0x00 */ SCHEDULE_CMD_CHECK_NOT_IN_SCENE_S(SCENE_TOWN, 0x11 - 0x04),
    /* 0x04 */ SCHEDULE_CMD_CHECK_TIME_RANGE_S(0, 0, 0, 10, 0x0B - 0x0A),
    /* 0x0A */ SCHEDULE_CMD_RET_NONE(),
    /* 0x0B */ SCHEDULE_CMD_RET_TIME(0, 0, 0, 10, 91),
    /* 0x11 */ SCHEDULE_CMD_RET_NONE(),
};

static s32 D_80AFB430[] = {
    -1, 0, 4, 1,  0,  0, 1, 4,  -1, -1, 15, 5, 0, 3, -1, -1, 1, 2, 11, 3, -1, -1, -1, 0, 0, 0, 0, 0, 1,  12, -1,
    -1, 2, 0, 10, 11, 0, 3, 1,  0,  1,  2,  0, 1, 0, 1,  2,  3, 4, 3,  5, 14, 3,  1,  0, 6, 4, 1, 2, 6,  7,  8,
    2,  1, 0, 6,  7,  9, 8, -1, 0,  1,  10, 3, 1, 0, 11, 12, 8, 2, 2,  3, 0,  1,  1,  0, 6, 9, 1, 2, 12, 13,
};

// Game scripts
static UNK_TYPE D_80AFB5A0[] = {
    0x00560800, 0x45090000, 0x170E28BA, 0x0C090000, 0x180E28BB, 0x0C090000, 0x170E28BC, 0x0C090000, 0x180E28BD,
    0x0C090000, 0x170E28BE, 0x0C090000, 0x180E28BF, 0x0C090000, 0x170E28C0, 0x2D00012D, 0x000B0C09, 0x00001156,
    0x08100900, 0x00170E29, 0x5C0C0900, 0x00180E29, 0x5D0C0900, 0x00170E29, 0x5E2D0001, 0x2D000B0C, 0x09000010,
};

static UNK_TYPE D_80AFB60C[] = {
    0x0E27742D,
    0x000B0C10,
};

static UNK_TYPE D_80AFB614[] = {
    0x0E27752D,
    0x000B0C10,
};

static UNK_TYPE D_80AFB61C[] = {
    0x0E27772D,
    0x000B0C10,
};

static UNK_TYPE D_80AFB624[] = {
    0x0E27782D,
    0x000B0C10,
};

static UNK_TYPE D_80AFB62C[] = {
    0x0E27792D,
    0x000B0C10,
};

static UNK_TYPE D_80AFB634[] = {
    0x0E27812D,
    0x000B0C10,
};

static UNK_TYPE D_80AFB63C[] = {
    0x0E277A2D,
    0x000B0C10,
};

static UNK_TYPE D_80AFB644[] = {
    0x0E27822D,
    0x000B0C11,
    0x58021000,
};

static UNK_TYPE D_80AFB650[] = {
    0x0E27832D,
    0x000B0C10,
};

static UNK_TYPE D_80AFB658[] = {
    0x25002E00, 0x1A005280, 0x000C0E27, 0x7C2D000B, 0x0C115280, 0x12100E27, 0x7D2D000B, 0x0C12100E, 0x277C0C0E,
    0x00FF1E00, 0x33000000, 0x1800282C, 0xFFFF0900, 0x000E277E, 0x2D000B0C, 0x11528011, 0x56012A00, 0x2E12102C,
    0x277D0C2F, 0x00002D00, 0x0B0C1152, 0x8012100E, 0x277D2D00, 0x0B0C1152, 0x80121000,
};

static UNK_TYPE D_80AFB6BC[] = {
    0x0900000E, 0x27A50C09, 0x0000170E, 0x27A60C09, 0x0000180E, 0x27A70C09, 0x0000170E,
    0x27A80C09, 0x0000180E, 0x27A90C09, 0x0000170E, 0x27AA0C09, 0x0000180E, 0x27AB0C09,
    0x0000170E, 0x27AC0C09, 0x0000180E, 0x27AD2D00, 0x0B2D0008, 0x0C115908, 0x09000010,
};

static UNK_TYPE D_80AFB710[] = {
    0x25003E00, 0x230E2780, 0x0C0F27AE, 0x0C120600, 0x84000013, 0x0084115A, 0x042F0000,
    0x2E2D000B, 0x2D00310C, 0x115A0110, 0x0E27802D, 0x000B0C11, 0x5A011210,
};

static UNK_TYPE D_80AFB744[] = {
    0x0E23692D,
    0x000B0C10,
};

static UNK_TYPE D_80AFB74C[] = {
    0x2CFFFF09, 0x00000E27, 0x7E2D000B, 0x0C115280, 0x1156012A, 0x002E1210,
};

static UNK_TYPE D_80AFB764[] = {
    0x2C277D2D,
    0x000B0C11,
    0x52801210,
};

ActorInit En_Pm_InitVars = {
    ACTOR_EN_PM,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MM,
    sizeof(EnPm),
    (ActorFunc)EnPm_Init,
    (ActorFunc)EnPm_Destroy,
    (ActorFunc)EnPm_Update,
    (ActorFunc)EnPm_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT1,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 14, 62, 0, { 0, 0, 0 } },
};

static ColliderSphereInit sSphereInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_SPHERE,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 0, { { 0, 0, 0 }, 26 }, 100 },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

static AnimationInfoS sAnimationInfo[] = {
    { &object_mm_Anim_002238, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_mm_Anim_002238, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_mm_Anim_00A4E0, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &object_mm_Anim_00B09C, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_mm_Anim_00B09C, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_mm_Anim_00BA78, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &object_mm_Anim_00C32C, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_mm_Anim_0099B4, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_mm_Anim_000FC4, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_mm_Anim_00A8D8, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_mm_Anim_00099C, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_mm_Anim_001F84, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &object_mm_Anim_000468, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_mm_Anim_00C640, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
};

s32 func_80AF7B40(void) {
    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_90_01)) {
        return 4;
    }

    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_89_40)) {
        return 3;
    }

    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_89_08)) {
        return 2;
    }

    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_86_01)) {
        return 1;
    }

    return 0;
}

s32 func_80AF7BAC(EnPm* this) {
    switch (this->unk_38C) {
        case 0:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_86_01)) {
                D_801F4E78 = gSaveContext.save.time;
                this->unk_38C++;
            }
            break;

        case 1:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_89_08)) {
                D_801F4E78 = gSaveContext.save.time;
                this->unk_38C++;
            }
            break;

        case 2:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_89_40)) {
                D_801F4E78 = 0;
                this->unk_38C++;
            }
            break;

        case 3:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_90_01)) {
                D_801F4E78 = gSaveContext.save.time;
                this->unk_38C++;
            }
            break;
    }

    return true;
}

Actor* func_80AF7CB0(EnPm* this, PlayState* play, u8 actorCat, s16 actorId) {
    Actor* phi_s0 = NULL;
    Actor* actor;

    while (true) {
        actor = SubS_FindActor(play, phi_s0, actorCat, actorId);
        phi_s0 = actor;

        if (actor == NULL) {
            break;
        }

        if (((EnPm*)phi_s0 != this) && (actor->update != NULL)) {
            break;
        }

        actor = actor->next;
        if (actor == NULL) {
            phi_s0 = NULL;
            break;
        }

        phi_s0 = actor;
    }

    return phi_s0;
}

EnDoor* func_80AF7D60(PlayState* play, s32 arg1) {
    s32 phi_a1;

    switch (arg1) {
        case 1:
        case 2:
        case 32:
        case 33:
            phi_a1 = 1;
            break;

        case 34:
        case 35:
        case 36:
        case 37:
            phi_a1 = 10;
            break;

        case 10:
        case 11:
        case 12:
        case 13:
            phi_a1 = 11;
            break;

        default:
            return NULL;
    }

    return SubS_FindDoor(play, phi_a1);
}

Actor* func_80AF7DC4(EnPm* this, PlayState* play, s32 arg2) {
    Actor* phi_s0 = NULL;
    Actor* actor;

    while (true) {
        actor = SubS_FindActor(play, phi_s0, ACTORCAT_PROP, ACTOR_EN_PST);
        phi_s0 = actor;

        if (actor == NULL) {
            break;
        }

        if (((EnPm*)actor != this) && (actor->update != NULL) && (actor->params == (s16)arg2)) {
            break;
        }

        actor = actor->next;
        if (actor == NULL) {
            phi_s0 = NULL;
            break;
        }

        phi_s0 = actor;
    }

    return phi_s0;
}

void func_80AF7E6C(EnPm* this) {
    this->skelAnime.playSpeed = this->unk_35C;
    SkelAnime_Update(&this->skelAnime);
}

s32 func_80AF7E98(EnPm* this, s32 arg1) {
    s32 phi_v1 = false;
    s32 ret = false;

    switch (arg1) {
        case 0:
        case 1:
            if ((this->unk_384 != 0) && (this->unk_384 != 1)) {
                phi_v1 = true;
            }
            break;

        case 3:
        case 4:
            if ((this->unk_384 != 3) && (this->unk_384 != 4)) {
                phi_v1 = true;
            }
            break;

        default:
            if (arg1 != this->unk_384) {
                phi_v1 = true;
            }
            break;
    }

    if (phi_v1) {
        this->unk_384 = arg1;
        ret = SubS_ChangeAnimationByInfoS(&this->skelAnime, sAnimationInfo, arg1);
        this->unk_35C = this->skelAnime.playSpeed;
    }

    return ret;
}

void func_80AF7F68(EnPm* this, PlayState* play) {
    f32 temp;
    s32 pad;

    switch (this->unk_258) {
        case 9:
        case 20:
        case 21:
        case 22:
        case 24:
            temp = this->colliderSphere.dim.modelSphere.radius * this->colliderSphere.dim.scale;
            this->colliderSphere.dim.worldSphere.radius = temp;
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderSphere.base);
            break;

        default:
            Collider_UpdateCylinder(&this->actor, &this->colliderCylinder);
            temp = this->actor.focus.pos.y - this->actor.world.pos.y;
            this->colliderCylinder.dim.height = temp;
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderCylinder.base);
            break;
    }
}

Actor* func_80AF8040(EnPm* this, PlayState* play) {
    Actor* actor;

    switch (this->unk_258) {
        case 16:
            actor = func_80AF7CB0(this, play, ACTORCAT_NPC, ACTOR_EN_AN);
            break;

        case 17:
            actor = func_80AF7CB0(this, play, ACTORCAT_NPC, ACTOR_EN_TEST3);
            break;

        case 28:
            actor = func_80AF7CB0(this, play, ACTORCAT_NPC, ACTOR_EN_AL);
            break;

        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            actor = func_80AF7DC4(this, play, this->unk_258 - 3);
            break;

        case 19:
            actor = func_80AF7DC4(this, play, 4);
            break;

        case 18:
            actor = func_80AF7DC4(this, play, 4);
            break;

        default:
            actor = &GET_PLAYER(play)->actor;
            break;
    }

    return actor;
}

s32 func_80AF80F4(EnPm* this, s16 arg1) {
    s32 ret = false;

    if (ActorCutscene_GetCurrentIndex() == 0x7C) {
        ActorCutscene_Stop(0x7C);
        ActorCutscene_SetIntentToPlay(arg1);
    } else if (ActorCutscene_GetCanPlayNext(arg1)) {
        ActorCutscene_StartAndSetUnkLinkFields(arg1, &this->actor);
        ret = true;
    } else {
        ActorCutscene_SetIntentToPlay(arg1);
    }
    return ret;
}

s16 func_80AF8170(EnPm* this, s32 arg1) {
    s32 i;
    s16 cs = -1;

    if ((this->actor.child != NULL) && (this->actor.child->update != NULL)) {
        cs = this->actor.child->cutscene;

        for (i = 0; i < arg1; i++) {
            cs = ActorCutscene_GetAdditionalCutscene(cs);
        }
    }
    return cs;
}

s32 func_80AF81E8(EnPm* this, PlayState* play) {
    s32 pad;
    s16 sp2A = func_80AF8170(this, 0);
    s32 ret = false;

    switch (this->unk_378) {
        case 0:
            if (!func_80AF80F4(this, sp2A)) {
                break;
            }

        case 2:
        case 4:
        case 6:
            if ((this->actor.child != NULL) && (this->actor.child->update != NULL)) {
                Camera_SetTargetActor(Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(sp2A)), this->actor.child);
            }
            this->unk_378++;
            ret = true;
            break;

        case 1:
        case 3:
        case 5:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_86_08) && (this->unk_378 == 3)) {
                ActorCutscene_Stop(sp2A);
            } else {
                Camera_SetTargetActor(Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(sp2A)), &this->actor);
            }
            this->unk_378++;
            ret = true;
            break;

        case 7:
            ActorCutscene_Stop(sp2A);
            this->unk_378++;
            ret = true;
            break;
    }
    return ret;
}

s32 func_80AF8348(EnPm* this, PlayState* play) {
    s32 pad;
    s16 sp2A = func_80AF8170(this, 0);
    s32 ret = false;

    switch (this->unk_378) {
        case 0:
            if (!func_80AF80F4(this, sp2A)) {
                break;
            }

        case 2:
        case 4:
        case 6:
        case 8:
            Camera_SetTargetActor(Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(sp2A)), &this->actor);
            this->unk_378++;
            ret = true;
            break;

        case 1:
        case 3:
        case 5:
        case 7:
            if ((this->actor.child != NULL) && (this->actor.child->update != NULL)) {
                Camera_SetTargetActor(Play_GetCamera(play, ActorCutscene_GetCurrentSubCamId(sp2A)), this->actor.child);
            }
            this->unk_378++;
            ret = true;
            break;

        case 9:
            ActorCutscene_Stop(sp2A);
            this->unk_378++;
            ret = true;
            break;
    }

    return ret;
}

s32 func_80AF8478(EnPm* this, PlayState* play) {
    s32 pad;
    s32 ret = false;

    switch (this->unk_378) {
        case 0:
            func_80AF7E98(this, 2);
            this->unk_356 &= ~0x20;
            this->unk_356 |= 0x200;
            this->unk_378++;
            break;

        case 1:
            if (Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
                func_80AF7E98(this, 4);
                this->unk_356 &= ~0x200;
                this->unk_356 |= 0x20;
                this->unk_378++;
                ret = true;
            }
            break;
    }
    return ret;
}

UNK_TYPE* func_80AF8540(EnPm* this, PlayState* play) {
    switch (this->unk_258) {
        case 28:
            this->unk_37C = func_80AF8348;
            return D_80AFB6BC;

        case 29:
            return D_80AFB710;

        case 16:
            this->unk_37C = func_80AF81E8;
            return D_80AFB5A0;

        case 17:
            return D_80AFB644;

        case 24:
            if (this->unk_356 & 0x2000) {
                this->unk_37C = func_80AF8478;
                return D_80AFB74C;
            } else if (this->unk_356 & 0x4000) {
                return D_80AFB764;
            } else {
                this->unk_37C = func_80AF8478;
                return D_80AFB658;
            }
            break;

        case 25:
            return D_80AFB650;

        default:
            if (Player_GetMask(play) == PLAYER_MASK_KAFEIS_MASK) {
                return D_80AFB744;
            }

            switch (this->unk_258) {
                case 9:
                    return D_80AFB614;

                case 14:
                    return D_80AFB624;

                case 20:
                    return D_80AFB61C;

                case 21:
                    return D_80AFB634;

                case 22:
                    return D_80AFB63C;

                case 18:
                case 74:
                case 75:
                case 76:
                case 77:
                case 78:
                case 79:
                    return D_80AFB62C;

                default:
                    return D_80AFB60C;
            }
            break;
    }
}

s32 func_80AF86F0(EnPm* this, PlayState* play) {
    s32 ret = false;

    if ((this->unk_356 & 7) && Actor_ProcessTalkRequest(&this->actor, &play->state)) {
        SubS_UpdateFlags(&this->unk_356, 0, 7);
        this->unk_398 = 0;
        this->unk_378 = 0;
        this->unk_37C = NULL;
        this->actor.child = this->unk_268;
        this->unk_25C = func_80AF8540(this, play);
        if ((this->unk_258 != 24) && (this->unk_258 != 9) && (this->unk_258 != 20) && (this->unk_258 != 21) &&
            (this->unk_258 != 22)) {
            this->unk_356 |= 0x20;
        }
        this->actionFunc = func_80AFA5FC;
        ret = true;
    }
    return ret;
}

s32 func_80AF87C4(EnPm* this, PlayState* play) {
    s32 ret = false;

    if ((play->csCtx.state != 0) && (play->sceneId == SCENE_00KEIKOKU) && (gSaveContext.sceneLayer == 9) &&
        (play->curSpawn == 1)) {
        if (!this->unk_380) {
            func_80AF7E98(this, 0);
            this->unk_258 = 255;
            this->unk_380 = true;
            this->actor.speed = 4.0f;
            this->actor.gravity = -1.0f;
        }
        ret = true;
    } else if (this->unk_380) {
        this->unk_258 = 0;
        this->unk_380 = false;
        this->actor.speed = 0.0f;
    }
    return ret;
}

void func_80AF8890(EnPm* this, Gfx** gfx, s32 arg2) {
    Matrix_Push();

    switch (arg2) {
        case 0:
            if (this->unk_356 & 0x800) {
                gSPDisplayList((*gfx)++, object_mm_DL_008348);
            }
            break;

        case 1:
            if (this->unk_356 & 0x1000) {
                gSPDisplayList((*gfx)++, object_mm_DL_0085C8);
            }
            break;

        case 2:
            if (this->unk_356 & 0x1000) {
                gSPDisplayList((*gfx)++, object_mm_DL_0083E0);
            }
            break;
    }

    Matrix_Pop();
}

void func_80AF898C(EnPm* this) {
    s32 pad;
    Vec3f sp40;
    Vec3f sp34;
    s32 pad2;

    Math_Vec3f_Copy(&sp40, &this->unk_268->world.pos);
    Math_Vec3f_Copy(&sp34, &this->actor.world.pos);
    Math_ApproachS(&this->unk_372, Math_Vec3f_Yaw(&sp34, &sp40) - this->actor.shape.rot.y, 4, 0x2AA8);
    this->unk_372 = CLAMP(this->unk_372, -0x1FFE, 0x1FFE);
    Math_Vec3f_Copy(&sp34, &this->actor.focus.pos);
    if (this->unk_268->id == ACTOR_PLAYER) {
        sp40.y = ((Player*)this->unk_268)->bodyPartsPos[7].y + 3.0f;
    } else {
        Math_Vec3f_Copy(&sp40, &this->unk_268->focus.pos);
    }
    Math_ApproachS(&this->unk_370, Math_Vec3f_Pitch(&sp34, &sp40), 4, 0x2AA8);
    this->unk_370 = CLAMP(this->unk_370, -0x1554, 0x1554);
}

void func_80AF8AC8(EnPm* this) {
    if ((this->unk_356 & 0x20) && (this->unk_268 != NULL) && (this->unk_268->update != NULL)) {
        if (DECR(this->unk_376) == 0) {
            func_80AF898C(this);
            this->unk_356 &= ~0x200;
            this->unk_356 |= 0x80;
            return;
        }
    }

    if (this->unk_356 & 0x80) {
        this->unk_356 &= ~0x80;
        this->unk_370 = 0;
        this->unk_372 = 0;
        this->unk_376 = 20;
    } else if (DECR(this->unk_376) == 0) {
        this->unk_356 |= 0x200;
    }
}

void func_80AF8BA8(s32 arg0) {
    static u16 D_80AFB8D4[] = {
        WEEKEVENTREG_27_02, WEEKEVENTREG_27_04, WEEKEVENTREG_27_08, WEEKEVENTREG_27_10, WEEKEVENTREG_27_20,
    };
    static u16 D_80AFB8E0[] = {
        WEEKEVENTREG_27_40, WEEKEVENTREG_27_80, WEEKEVENTREG_28_01, WEEKEVENTREG_28_02, WEEKEVENTREG_28_04,
    };

    if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_88_02)) {
        if (CHECK_WEEKEVENTREG(D_80AFB8D4[arg0])) {
            switch (gSaveContext.save.day) {
                case 2:
                    SET_WEEKEVENTREG(WEEKEVENTREG_28_08);
                    break;

                case 3:
                    SET_WEEKEVENTREG(WEEKEVENTREG_28_10);
                    break;
            }
            SET_WEEKEVENTREG(WEEKEVENTREG_51_02);
            SET_WEEKEVENTREG(WEEKEVENTREG_90_08);
        }
    }

    SET_WEEKEVENTREG(D_80AFB8E0[arg0]);
}

void func_80AF8C68(EnPm* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 talkState = Message_GetState(&play->msgCtx);
    u16 textId = play->msgCtx.currentTextId;

    if ((player->talkActor == &this->actor) && ((textId < 0xFF) || (textId > 0x200)) && (talkState == TEXT_STATE_3) &&
        (this->prevTalkState == TEXT_STATE_3)) {
        if ((play->state.frames % 3) == 0) {
            if (this->unk_360 == 120.0f) {
                this->unk_360 = 0.0f;
            } else {
                this->unk_360 = 120.0f;
            }
        }
    } else {
        this->unk_360 = 0.0f;
    }
    Math_SmoothStepToF(&this->unk_364, this->unk_360, 0.8f, 40.0f, 10.0f);
    Matrix_Translate(this->unk_364, 0.0f, 0.0f, MTXMODE_APPLY);
    this->prevTalkState = talkState;
}

s32 func_80AF8D84(EnPm* this, PlayState* play) {
    switch (this->unk_384) {
        case 10:
            func_80AF7E98(this, 9);
            break;

        case 7:
            func_80AF7E98(this, 0);
            break;
    }
    return true;
}

s32 func_80AF8DD4(EnPm* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    u16 textId = play->msgCtx.currentTextId;
    s32 pad;

    if (player->stateFlags1 & (PLAYER_STATE1_40 | PLAYER_STATE1_400)) {
        this->unk_356 |= 0x400;
        if (this->unk_358 != textId) {
            if ((this->unk_384 == 0) || (this->unk_384 == 1)) {
                func_80AF7E98(this, 7);
            }
            if ((textId == 0x277C) || (textId == 0x277D)) {
                func_80AF7E98(this, 10);
            }
        }
        this->unk_358 = textId;
    } else {
        if (this->unk_356 & 0x400) {
            this->unk_358 = 0;
            this->unk_356 &= ~0x400;
            func_80AF8D84(this, play);
        }
    }

    if (this->unk_18C != NULL) {
        this->unk_18C(this, play);
    }

    return 0;
}

s32 func_80AF8ED4(EnPm* this, PlayState* play, ScheduleOutput* scheduleOutput, u8 actorCat, s16 actorId) {
    u8 sp4F = this->actor.params & 0xFF;
    Vec3s* sp48;
    Vec3f sp3C;
    Vec3f sp30;
    Actor* sp2C;
    s32 pad;
    s32 ret = false;

    this->timePath = NULL;
    sp2C = func_80AF7CB0(this, play, actorCat, actorId);
    if (D_80AFB430[scheduleOutput->result] >= 0) {
        this->timePath = SubS_GetAdditionalPath(play, sp4F, D_80AFB430[scheduleOutput->result]);
    }

    if ((sp2C != NULL) && (sp2C->update != NULL)) {
        if (this->timePath != NULL) {
            sp48 = Lib_SegmentedToVirtual(this->timePath->points);
            Math_Vec3s_ToVec3f(&sp3C, &sp48[this->timePath->count - 2]);
            Math_Vec3s_ToVec3f(&sp30, &sp48[this->timePath->count - 1]);
            this->actor.shape.shadowDraw = NULL;
            this->actor.world.rot.y = Math_Vec3f_Yaw(&sp3C, &sp30);
            Math_Vec3f_Copy(&this->actor.world.pos, &sp30);
            ret = true;
        }
    }

    return ret;
}

s32 func_80AF9008(EnPm* this, PlayState* play, ScheduleOutput* scheduleOutput) {
    u16 sp56 = SCHEDULE_TIME_NOW;
    u8 sp55 = this->actor.params & 0xFF;
    EnDoor* door;
    Vec3s* sp4C;
    Vec3f sp40;
    Vec3f sp34;
    s16 temp;
    s32 ret = false;

    this->timePath = NULL;
    door = func_80AF7D60(play, scheduleOutput->result);
    if (D_80AFB430[scheduleOutput->result] >= 0) {
        this->timePath = SubS_GetAdditionalPath(play, sp55, D_80AFB430[scheduleOutput->result]);
    }

    if ((door != NULL) && (door->dyna.actor.update != NULL)) {
        if (this->timePath != 0) {
            sp4C = Lib_SegmentedToVirtual(this->timePath->points);
            Math_Vec3s_ToVec3f(&sp40, &sp4C[0]);
            Math_Vec3s_ToVec3f(&sp34, &sp4C[1]);
            Math_Vec3f_Copy(&this->unk_26C, &sp40);
            Math_Vec3f_Copy(&this->unk_278, &sp34);
            this->actor.world.rot.y = Math_Vec3f_Yaw(&sp40, &sp34);
            Math_Vec3f_Copy(&this->actor.world.pos, &sp40);
            temp = this->actor.world.rot.y - door->dyna.actor.shape.rot.y;
            if (ABS_ALT(temp) <= 0x4000) {
                this->unk_260 = -0x4B;
            } else {
                this->unk_260 = 0x4B;
            }

            this->unk_36C = scheduleOutput->time1 - scheduleOutput->time0;
            this->unk_36E = sp56 - scheduleOutput->time0;
            this->actor.flags &= ~ACTOR_FLAG_1;
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_90_08)) {
                this->unk_356 |= 0x800;
            }
            this->unk_356 |= 0x9000;
            this->unk_356 |= 0x200;
            func_80AF7E98(this, 0);
            this->actor.gravity = 0.0f;
            ret = true;
        }
    }
    return ret;
}

s32 func_80AF91E8(EnPm* this, PlayState* play, ScheduleOutput* scheduleOutput) {
    u16 sp2E = SCHEDULE_TIME_NOW;
    u16 phi_v1;
    u8 sp2B = this->actor.params & 0xFF;
    u16 tmp;
    s16 pad;
    s32 ret = false;

    this->timePath = NULL;

    if (D_80AFB430[scheduleOutput->result] >= 0) {
        this->timePath = SubS_GetAdditionalPath(play, sp2B, D_80AFB430[scheduleOutput->result]);
    }

    if ((this->timePath != NULL) && (this->timePath->count < 3)) {
        this->timePath = NULL;
    }

    if (this->timePath != 0) {
        if ((this->unk_258 < 38) && (this->unk_258 != 0) && (this->timePathTimeSpeed >= 0)) {
            phi_v1 = sp2E;
        } else {
            phi_v1 = scheduleOutput->time0;
        }

        if (scheduleOutput->time1 < phi_v1) {
            this->timePathTotalTime = (phi_v1 - scheduleOutput->time1) + 0xFFFF;
        } else {
            this->timePathTotalTime = scheduleOutput->time1 - phi_v1;
        }

        this->timePathElapsedTime = sp2E - phi_v1;
        tmp = phi_v1 = this->timePath->count - (SUBS_TIME_PATHING_ORDER - 1);
        this->timePathWaypointTime = this->timePathTotalTime / tmp;
        this->timePathWaypoint =
            (this->timePathElapsedTime / this->timePathWaypointTime) + (SUBS_TIME_PATHING_ORDER - 1);
        this->unk_356 &= ~8;
        this->unk_356 &= ~0x10;
        if (this->unk_258 == 27) {
            Actor_PlaySfx(&this->actor, NA_SE_EV_ROOM_CARTAIN);
            Flags_UnsetSwitch(play, 0);
        }

        switch (scheduleOutput->result) {
            case 83:
            case 84:
            case 85:
            case 86:
            case 87:
            case 88:
            case 89:
            case 90:
                this->unk_356 |= 0x9000;
                this->unk_356 |= 0x200;

            case 82:
                func_80AF7E98(this, 0);
                break;

            case 91:
                this->unk_356 |= 0x9000;
                func_80AF7E98(this, 12);
                break;

            default:
                SubS_UpdateFlags(&this->unk_356, 3, 7);
                func_80AF7E98(this, 0);
                if (CHECK_WEEKEVENTREG(WEEKEVENTREG_90_08)) {
                    this->unk_356 |= 0x800;
                }
                this->unk_356 |= 0x9000;
                this->unk_356 |= 0x200;
                break;
        }

        this->actor.gravity = -1.0f;
        ret = true;
    }

    return ret;
}

s32 func_80AF94AC(EnPm* this, PlayState* play, ScheduleOutput* scheduleOutput) {
    u8 sp4F = this->actor.params & 0xFF;
    Vec3f sp40;
    Vec3f sp34;
    Vec3s* sp30;
    s32 pad;
    s32 ret = false;

    this->timePath = NULL;
    if (D_80AFB430[scheduleOutput->result] >= 0) {
        this->timePath = SubS_GetAdditionalPath(play, sp4F, D_80AFB430[scheduleOutput->result]);
    }

    if ((this->timePath != 0) && (this->timePath->count >= 2)) {
        sp30 = Lib_SegmentedToVirtual(this->timePath->points);
        Math_Vec3s_ToVec3f(&sp40, &sp30[0]);
        Math_Vec3s_ToVec3f(&sp34, &sp30[1]);
        this->actor.world.rot.y = Math_Vec3f_Yaw(&sp40, &sp34);
        Math_Vec3s_Copy(&this->actor.shape.rot, &this->actor.world.rot);
        Math_Vec3f_Copy(&this->actor.world.pos, &sp40);
        Math_Vec3f_Copy(&this->actor.prevPos, &sp40);
        if (scheduleOutput->result == 24) {
            Flags_UnsetSwitch(play, 0);
            Flags_UnsetSwitch(play, 1);
            this->unk_394 = PLAYER_IA_NONE;
            this->unk_368 = 60.0f;
            func_80AF7E98(this, 9);
        }
        ret = true;
    }
    return ret;
}

s32 func_80AF95E8(EnPm* this, PlayState* play, ScheduleOutput* scheduleOutput) {
    u8 sp4F = this->actor.params & 0xFF;
    Vec3f sp40;
    Vec3f sp34;
    Vec3s* sp30;
    s32 pad;
    s32 ret = false;
    s32 phi_a3 = -1;

    switch (scheduleOutput->result) {
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            phi_a3 = scheduleOutput->result - 3;
            break;

        case 19:
            phi_a3 = 4;
            break;
    }

    if ((phi_a3 >= 0) && !func_80AF7DC4(this, play, phi_a3)) {
        return ret;
    }

    this->timePath = NULL;
    phi_a3 = D_80AFB430[scheduleOutput->result];
    if (phi_a3 >= 0) {
        this->timePath = SubS_GetAdditionalPath(play, sp4F, phi_a3);
    }

    if ((this->timePath != 0) && (this->timePath->count >= 2)) {
        sp30 = Lib_SegmentedToVirtual(this->timePath->points);
        Math_Vec3s_ToVec3f(&sp40, &sp30[this->timePath->count - 1]);
        Math_Vec3s_ToVec3f(&sp34, &sp30[this->timePath->count - 2]);
        this->actor.world.rot.y = Math_Vec3f_Yaw(&sp34, &sp40);
        Math_Vec3s_Copy(&this->actor.shape.rot, &this->actor.world.rot);
        Math_Vec3f_Copy(&this->actor.world.pos, &sp40);
        Math_Vec3f_Copy(&this->actor.prevPos, &sp40);

        switch (scheduleOutput->result) {
            case 27:
                Actor_PlaySfx(&this->actor, NA_SE_EV_ROOM_CARTAIN);
                Flags_SetSwitch(play, 0);
                this->unk_36C = 20;
                SubS_UpdateFlags(&this->unk_356, 3, 7);
                func_80AF7E98(this, 3);
                break;

            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 19:
                this->unk_356 |= 0x9000;
                func_80AF7E98(this, 11);
                break;

            case 18:
                this->unk_356 |= 0x9000;
                this->unk_356 |= 0x800;
                func_80AF7E98(this, 5);
                break;

            case 23:
                if (CHECK_WEEKEVENTREG(WEEKEVENTREG_90_08)) {
                    this->unk_356 |= 0x800;
                }
                SET_WEEKEVENTREG(WEEKEVENTREG_60_04);

            default:
                if (scheduleOutput->result == 29) {
                    this->actor.world.rot.y = BINANG_ROT180(this->actor.world.rot.y);
                }
                SubS_UpdateFlags(&this->unk_356, 3, 7);
                this->unk_356 |= 0x9000;
                func_80AF7E98(this, 3);
                break;
        }
        ret = true;
    }
    return ret;
}

s32 func_80AF98A0(EnPm* this, PlayState* play, ScheduleOutput* scheduleOutput) {
    s32 ret = false;

    if (Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_MM3, 116.0f, 26.0f, -219.0f, 0, -0x3F46, 0,
                           0) != NULL) {
        Actor_Kill(&this->actor);
        ret = true;
    }
    return ret;
}

s32 func_80AF992C(EnPm* this, PlayState* play, ScheduleOutput* scheduleOutput) {
    static Vec3f D_80AFB8EC = { 116.0f, 26.0f, -219.0f };
    static Vec3s D_80AFB8F8 = { 0x0000, 0xC0BA, 0x0000 };
    s32 pad;

    Math_Vec3f_Copy(&this->actor.world.pos, &D_80AFB8EC);
    Math_Vec3s_Copy(&this->actor.world.rot, &D_80AFB8F8);
    Math_Vec3s_Copy(&this->actor.shape.rot, &this->actor.world.rot);
    SubS_UpdateFlags(&this->unk_356, 3, 7);
    this->actor.targetMode = 6;
    this->actor.gravity = -1.0f;
    this->unk_368 = 80.0f;
    if (scheduleOutput->result == 14) {
        this->unk_356 &= ~0x200;
        func_80AF7E98(this, 13);
    } else {
        this->unk_356 &= ~0x200;
        func_80AF7E98(this, 8);
    }
    return true;
}

s32 func_80AF9A0C(EnPm* this, PlayState* play, ScheduleOutput* scheduleOutput) {
    s32 ret = false;

    if (func_80AF8ED4(this, play, scheduleOutput, ACTORCAT_NPC, ACTOR_EN_AN)) {
        SubS_UpdateFlags(&this->unk_356, 3, 7);
        this->unk_356 |= 0x20;
        this->unk_356 |= 0x9000;
        if (this->unk_258 != 0) {
            this->unk_356 |= 0x800;
            func_80AF7E98(this, 5);
        } else {
            func_80AF7E98(this, 3);
        }
        ret = true;
    }
    return ret;
}

s32 func_80AF9AB0(EnPm* this, PlayState* play, ScheduleOutput* scheduleOutput) {
    s32 ret = false;

    if (func_80AF8ED4(this, play, scheduleOutput, ACTORCAT_NPC, ACTOR_EN_TEST3)) {
        SubS_UpdateFlags(&this->unk_356, 3, 7);
        this->unk_356 |= 0x20;
        this->unk_356 |= 0x9000;
        if (this->unk_258 != 0) {
            this->unk_356 |= 0x800;
            func_80AF7E98(this, 5);
        } else {
            func_80AF7E98(this, 3);
        }
        ret = true;
    }
    return ret;
}

s32 func_80AF9B54(EnPm* this, PlayState* play, ScheduleOutput* scheduleOutput) {
    s32 ret = false;

    if (func_80AF8ED4(this, play, scheduleOutput, ACTORCAT_NPC, ACTOR_EN_AL)) {
        SubS_UpdateFlags(&this->unk_356, 3, 7);
        this->unk_356 |= 0x9000;
        this->unk_356 |= 0x20;
        if (this->unk_258 != 0) {
            this->unk_356 |= 0x800;
            func_80AF7E98(this, 5);
        } else {
            func_80AF7E98(this, 3);
        }
        ret = true;
    }
    return ret;
}

s32 func_80AF9BF8(EnPm* this, PlayState* play, ScheduleOutput* scheduleOutput) {
    s32 ret;

    this->actor.flags |= ACTOR_FLAG_1;
    this->actor.targetMode = 0;
    this->unk_394 = PLAYER_IA_NONE;
    this->unk_356 = 0;
    this->unk_368 = 40.0f;

    switch (scheduleOutput->result) {
        case 16:
            ret = func_80AF9A0C(this, play, scheduleOutput);
            break;

        case 17:
            ret = func_80AF9AB0(this, play, scheduleOutput);
            break;

        case 28:
            ret = func_80AF9B54(this, play, scheduleOutput);
            break;

        case 24:
            ret = func_80AF94AC(this, play, scheduleOutput);
            break;

        case 29:
            ret = func_80AF95E8(this, play, scheduleOutput);
            break;

        case 1:
        case 2:
        case 10:
        case 11:
        case 12:
        case 13:
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
            ret = func_80AF9008(this, play, scheduleOutput);
            break;

        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 18:
        case 19:
        case 23:
        case 25:
        case 27:
            ret = func_80AF95E8(this, play, scheduleOutput);
            break;

        case 8:
            ret = func_80AF98A0(this, play, scheduleOutput);
            break;

        case 9:
        case 14:
        case 20:
        case 21:
        case 22:
            ret = func_80AF992C(this, play, scheduleOutput);
            break;

        case 38:
        case 39:
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56:
        case 57:
        case 58:
        case 59:
        case 60:
        case 61:
        case 62:
        case 63:
        case 64:
        case 65:
        case 66:
        case 67:
        case 68:
        case 69:
        case 70:
        case 71:
        case 72:
        case 73:
        case 74:
        case 75:
        case 76:
        case 77:
        case 78:
        case 79:
        case 80:
        case 81:
        case 82:
        case 83:
        case 84:
        case 85:
        case 86:
        case 87:
        case 88:
        case 89:
        case 90:
        case 91:
            ret = func_80AF91E8(this, play, scheduleOutput);
            break;

        default:
            ret = false;
            break;
    }
    return ret;
}

s32 func_80AF9D04(EnPm* this, PlayState* play) {
    EnDoor* door = (EnDoor*)func_80AF7D60(play, this->unk_258);
    Vec3f sp38;
    s32 pad;
    f32 temp;

    if (!SubS_InCsMode(play) && (this->timePathTimeSpeed != 0)) {
        if ((door != NULL) && (door->dyna.actor.update != NULL)) {
            if (((f32)this->unk_36E / this->unk_36C) <= 0.9f) {
                door->unk_1A7 = this->unk_260;
            } else {
                door->unk_1A7 = 0;
            }
        }
        this->unk_36E = CLAMP(this->unk_36E, 0, this->unk_36C);
        temp = Math_Vec3f_DistXZ(&this->unk_26C, &this->unk_278) / this->unk_36C;
        sp38.x = 0.0f;
        sp38.y = 0.0f;
        sp38.z = this->unk_36E * temp;
        Lib_Vec3f_TranslateAndRotateY(&this->unk_26C, this->actor.world.rot.y, &sp38, &this->actor.world.pos);
        this->unk_36E += this->timePathTimeSpeed;
        if (Animation_OnFrame(&this->skelAnime, 3.0f) || Animation_OnFrame(&this->skelAnime, 8.0f)) {
            Actor_PlaySfx(&this->actor, NA_SE_EV_POSTMAN_WALK);
        }
    }
    return false;
}

s32 func_80AF9E7C(EnPm* this, PlayState* play) {
    f32 knots[265];
    Vec3f sp70;
    Vec3f sp64;
    Vec3f timePathTargetPos;
    s32 sp54;
    s32 sp50;

    sp50 = 0;
    sp54 = 0;

    SubS_TimePathing_FillKnots(knots, SUBS_TIME_PATHING_ORDER, this->timePath->count + SUBS_TIME_PATHING_ORDER);
    if (!(this->unk_356 & 8)) {
        timePathTargetPos = gZeroVec3f;
        SubS_TimePathing_Update(this->timePath, &this->timePathProgress, &this->timePathElapsedTime,
                                this->timePathWaypointTime, this->timePathTotalTime, &this->timePathWaypoint, knots,
                                &timePathTargetPos, this->timePathTimeSpeed);
        SubS_TimePathing_ComputeInitialY(play, this->timePath, this->timePathWaypoint, &timePathTargetPos);
        this->actor.world.pos.y = timePathTargetPos.y;
        this->unk_356 |= 8;
    } else {
        timePathTargetPos = this->timePathTargetPos;
    }

    this->actor.world.pos.x = timePathTargetPos.x;
    this->actor.world.pos.z = timePathTargetPos.z;

    if (SubS_InCsMode(play)) {
        sp54 = this->timePathElapsedTime;
        sp50 = this->timePathWaypoint;
        timePathTargetPos = this->actor.world.pos;
    }

    this->timePathTargetPos = gZeroVec3f;

    if (SubS_TimePathing_Update(this->timePath, &this->timePathProgress, &this->timePathElapsedTime,
                                this->timePathWaypointTime, this->timePathTotalTime, &this->timePathWaypoint, knots,
                                &this->timePathTargetPos, this->timePathTimeSpeed)) {
        this->unk_356 |= 0x10;
    } else {
        sp70 = this->actor.world.pos;
        sp64 = this->timePathTargetPos;
        this->actor.world.rot.y = Math_Vec3f_Yaw(&sp70, &sp64);
    }

    if (SubS_InCsMode(play)) {
        this->timePathElapsedTime = sp54;
        this->timePathWaypoint = sp50;
        this->timePathTargetPos = timePathTargetPos;
    } else if ((this->unk_258 != 91) &&
               (Animation_OnFrame(&this->skelAnime, 3.0f) || Animation_OnFrame(&this->skelAnime, 8.0f))) {
        Actor_PlaySfx(&this->actor, NA_SE_EV_POSTMAN_WALK);
    }

    if ((this->unk_356 & 0x10) && (this->unk_258 == 90)) {
        //! @bug Uses SET_WEEKEVENTREG instead of CHECK_WEEKEVENTREG
        if (!SET_WEEKEVENTREG(WEEKEVENTREG_89_40)) {
            SET_WEEKEVENTREG(WEEKEVENTREG_89_40);
        }
    }

    return false;
}

s32 func_80AFA170(EnPm* this, PlayState* play) {
    Vec3f sp34;
    Vec3f sp28;

    switch (this->unk_258) {
        case 28:
            if (gSaveContext.save.time >= CLOCK_TIME(1, 39)) {
                SET_WEEKEVENTREG(WEEKEVENTREG_89_08);
            }

        case 16:
        case 17:
        case 18:
            if ((this->unk_384 == 5) && Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
                func_80AF7E98(this, 6);
                this->unk_36C = 40;
            } else if ((this->unk_384 == 6) && (DECR(this->unk_36C) == 0)) {
                this->unk_356 &= ~0x800;
                func_80AF7E98(this, 4);
            }
            break;

        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 19:
            if ((this->unk_384 == 11) && Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
                func_80AF7E98(this, 4);
            }

            if ((this->unk_384 == 11) && Animation_OnFrame(&this->skelAnime, 8.0f)) {
                Actor_PlaySfx(&this->actor, NA_SE_EV_POSTMACHINE_HIT_OPEN);
            }

            if (this->unk_258 == 19) {
                func_80AF8BA8(4);
            } else {
                func_80AF8BA8(this->unk_258 - 3);
            }
            break;
    }

    if ((this->unk_268 != NULL) && (this->unk_268->update != NULL)) {
        Math_Vec3f_Copy(&sp34, &this->unk_268->world.pos);
        Math_Vec3f_Copy(&sp28, &this->actor.world.pos);
        this->actor.world.rot.y = Math_Vec3f_Yaw(&sp28, &sp34);
    }

    return true;
}

s32 func_80AFA334(EnPm* this, PlayState* play) {
    s16 temp_v0;

    switch (this->unk_258) {
        case 14:
        case 24:
            temp_v0 = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
            if (ABS_ALT(temp_v0) < 0x4000) {
                SubS_UpdateFlags(&this->unk_356, 3, 7);
            } else {
                SubS_UpdateFlags(&this->unk_356, 0, 7);
            }
            break;

        case 27:
            if (DECR(this->unk_36C) == 0) {
                Flags_SetSwitch(play, 1);
            }
            break;

        case 9:
        case 20:
        case 21:
        case 22:
            if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
                Actor_PlaySfx(&this->actor, NA_SE_VO_NP_SLEEP_OUT);
            }
            break;

        case 29:
            break;
    }

    return false;
}

void func_80AFA438(EnPm* this, PlayState* play) {
    switch (this->unk_258) {
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 16:
        case 17:
        case 18:
        case 28:
            func_80AFA170(this, play);
            break;

        case 1:
        case 2:
        case 10:
        case 11:
        case 12:
        case 13:
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
            func_80AF9D04(this, play);
            break;

        case 9:
        case 14:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 27:
        case 29:
            func_80AFA334(this, play);
            break;

        case 38:
        case 39:
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56:
        case 57:
        case 58:
        case 59:
        case 60:
        case 61:
        case 62:
        case 63:
        case 64:
        case 65:
        case 66:
        case 67:
        case 68:
        case 69:
        case 70:
        case 71:
        case 72:
        case 73:
        case 74:
        case 75:
        case 76:
        case 77:
        case 78:
        case 79:
        case 80:
        case 81:
        case 82:
        case 83:
        case 84:
        case 85:
        case 86:
        case 87:
        case 88:
        case 89:
        case 90:
        case 91:
            func_80AF9E7C(this, play);
            break;
    }

    Math_ApproachS(&this->actor.shape.rot.y, this->actor.world.rot.y, 3, 0x2AA8);
}

void func_80AFA4D0(EnPm* this, PlayState* play) {
    static u8* D_80AFB900[] = {
        D_80AFAD80, D_80AFB30C, D_80AFB3C0, D_80AFB3FC, D_80AFB41C,
    };
    u16 time = gSaveContext.save.time;
    u16 sp3C = 0;
    ScheduleOutput sp2C;

    this->timePathTimeSpeed = R_TIME_SPEED + ((void)0, gSaveContext.save.timeSpeedOffset);
    if (this->unk_38C != 0) {
        time = gSaveContext.save.time - D_801F4E78;
        sp3C = gSaveContext.save.time;
        gSaveContext.save.time = time;
    }

    if (!Schedule_RunScript(play, D_80AFB900[this->unk_38C], &sp2C) ||
        ((this->unk_258 != sp2C.result) && !func_80AF9BF8(this, play, &sp2C))) {
        this->actor.shape.shadowDraw = NULL;
        this->actor.flags &= ~ACTOR_FLAG_1;
        sp2C.result = 0;
    } else {
        this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        this->actor.flags |= ACTOR_FLAG_1;
    }

    this->unk_258 = sp2C.result;
    this->unk_268 = func_80AF8040(this, play);
    func_80AFA438(this, play);
    if (this->unk_38C != 0) {
        gSaveContext.save.time = sp3C;
    }
}

void func_80AFA5FC(EnPm* this, PlayState* play) {
    s16 yaw;
    Vec3f sp38;
    Vec3f sp2C;

    if (func_8010BF58(&this->actor, play, this->unk_25C, this->unk_37C, &this->unk_264)) {
        SubS_UpdateFlags(&this->unk_356, 3, 7);
        this->unk_356 &= ~0x20;
        this->unk_356 |= 0x200;
        this->actor.child = NULL;
        this->unk_376 = 20;
        this->unk_264 = 0;
        this->actionFunc = func_80AFA4D0;
        return;
    }

    if ((this->unk_258 != 9) && (this->unk_258 != 14) && (this->unk_258 != 16) && (this->unk_258 != 17) &&
        (this->unk_258 != 20) && (this->unk_258 != 21) && (this->unk_258 != 22) && (this->unk_258 != 24) &&
        (this->unk_258 != 25)) {
        if ((this->unk_268 != NULL) && (this->unk_268->update != NULL)) {
            Math_Vec3f_Copy(&sp38, &this->unk_268->world.pos);
            Math_Vec3f_Copy(&sp2C, &this->actor.world.pos);
            yaw = Math_Vec3f_Yaw(&sp2C, &sp38);
            Math_ApproachS(&this->actor.shape.rot.y, yaw, 4, 0x2AA8);
        }
    }
}

void func_80AFA724(EnPm* this, PlayState* play) {
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 30.0f, 12.0f, 0.0f, 4);
    if (Animation_OnFrame(&this->skelAnime, 3.0f) || Animation_OnFrame(&this->skelAnime, 8.0f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EV_POSTMAN_WALK);
    }
}

void EnPm_Init(Actor* thisx, PlayState* play) {
    EnPm* this = THIS;

    ActorShape_Init(&this->actor.shape, 0.0f, NULL, 14.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &object_mm_Skel_0096E8, NULL, this->jointTable, this->morphTable, 16);
    this->unk_384 = -1;
    func_80AF7E98(this, 0);
    Collider_InitAndSetCylinder(play, &this->colliderCylinder, &this->actor, &sCylinderInit);
    Collider_InitAndSetSphere(play, &this->colliderSphere, &this->actor, &sSphereInit);
    CollisionCheck_SetInfo2(&this->actor.colChkInfo, DamageTable_Get(0x16), &sColChkInfoInit);
    Actor_SetScale(&this->actor, 0.01f);
    this->unk_38C = func_80AF7B40();
    this->unk_258 = 0;
    this->unk_356 = 0;
    this->actionFunc = func_80AFA4D0;
    this->actionFunc(this, play);
}

void EnPm_Destroy(Actor* thisx, PlayState* play) {
    EnPm* this = THIS;

    Collider_DestroyCylinder(play, &this->colliderCylinder);
    Collider_DestroySphere(play, &this->colliderSphere);
}

void EnPm_Update(Actor* thisx, PlayState* play) {
    EnPm* this = THIS;

    if (!func_80AF86F0(this, play) && func_80AF87C4(this, play)) {
        func_80AFA724(this, play);
        func_80AF7E6C(this);
        func_80AF8AC8(this);
    } else {
        this->actionFunc(this, play);
        func_80AF7BAC(this);
        if (this->unk_258 != 0) {
            func_80AF8DD4(this, play);
            func_80AF7E6C(this);
            func_80AF8AC8(this);
            func_8013C964(&this->actor, play, this->unk_368, 30.0f, this->unk_394, this->unk_356 & 7);
            Actor_MoveWithGravity(&this->actor);
            Actor_UpdateBgCheckInfo(play, &this->actor, 30.0f, 12.0f, 0.0f, 4);
            func_80AF7F68(this, play);
        }
    }
}

s32 EnPm_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx,
                          Gfx** gfx) {
    EnPm* this = THIS;

    if (limbIndex == 15) {
        func_80AF8C68(this, play);
    }
    return false;
}

void EnPm_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx, Gfx** gfx) {
    EnPm* this = THIS;
    s32 pad;
    Vec3f sp2C;

    switch (limbIndex) {
        case 15:
            if (ActorCutscene_GetCurrentIndex() == -1) {
                Matrix_MultVec3f(&gZeroVec3f, &this->actor.focus.pos);
                Math_Vec3s_Copy(&this->actor.focus.rot, &this->actor.world.rot);
            }
            if ((this->unk_356 & 0x8000) && !CHECK_WEEKEVENTREG(WEEKEVENTREG_90_04)) {
                func_80AF8890(this, gfx, 1);
            }
            break;

        case 11:
            if (this->unk_356 & 0x800) {
                func_80AF8890(this, gfx, 0);
            }
            break;

        case 8:
            if ((this->unk_258 == 9) || (this->unk_258 == 20) || (this->unk_258 == 21) || (this->unk_258 == 22)) {
                Matrix_MultVec3f(&gZeroVec3f, &sp2C);
                Math_Vec3f_ToVec3s(&this->colliderSphere.dim.worldSphere.center, &sp2C);
            } else if (this->unk_258 == 24) {
                Matrix_MultVec3f(&gZeroVec3f, &sp2C);
                Math_Vec3f_ToVec3s(&this->colliderSphere.dim.worldSphere.center, &sp2C);
            }
            func_80AF8890(this, gfx, 2);
            break;
    }
}

void EnPm_TransformLimbDraw(PlayState* play, s32 limbIndex, Actor* thisx, Gfx** gfx) {
    EnPm* this = THIS;
    s32 stepRot;
    s32 overrideRot;

    if (!(this->unk_356 & 0x200)) {
        if (this->unk_356 & 0x80) {
            overrideRot = true;
        } else {
            overrideRot = false;
        }
        stepRot = true;
    } else {
        overrideRot = false;
        stepRot = false;
    }

    if (limbIndex == 15) {
        SubS_UpdateLimb(this->unk_370 + 0x4000, this->unk_372 + this->actor.shape.rot.y + 0x4000, &this->unk_284,
                        &this->unk_290, stepRot, overrideRot);
        Matrix_Pop();
        Matrix_Translate(this->unk_284.x, this->unk_284.y, this->unk_284.z, MTXMODE_NEW);
        Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
        Matrix_RotateYS(this->unk_290.y, MTXMODE_APPLY);
        Matrix_RotateXS(this->unk_290.x, MTXMODE_APPLY);
        Matrix_RotateZS(this->unk_290.z, MTXMODE_APPLY);
        Matrix_Push();
    }
}

void EnPm_Draw(Actor* thisx, PlayState* play) {
    static TexturePtr D_80AFB914[] = {
        object_mm_Tex_002950,
        object_mm_Tex_002750,
    };
    EnPm* this = THIS;
    s32 pad;

    if (this->unk_258 != 0) {
        func_8012C28C(play->state.gfxCtx);

        OPEN_DISPS(play->state.gfxCtx);

        Scene_SetRenderModeXlu(play, 0, 1);

        gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(D_80AFB914[0]));

        POLY_OPA_DISP = SubS_DrawTransformFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                               this->skelAnime.dListCount, EnPm_OverrideLimbDraw, EnPm_PostLimbDraw,
                                               EnPm_TransformLimbDraw, &this->actor, POLY_OPA_DISP);

        CLOSE_DISPS(play->state.gfxCtx);
    }
}
