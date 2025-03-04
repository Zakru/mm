/*
 * File: z_en_zo.c
 * Overlay: ovl_En_Zo
 * Description: Zoras (unused)
 */

#include "z_en_zo.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8 | ACTOR_FLAG_10)

#define THIS ((EnZo*)thisx)

void EnZo_Init(Actor* thisx, PlayState* play);
void EnZo_Destroy(Actor* thisx, PlayState* play);
void EnZo_Update(Actor* thisx, PlayState* play);
void EnZo_Draw(Actor* thisx, PlayState* play);

void EnZo_FollowPath(EnZo* this, PlayState* play);
void EnZo_TreadWater(EnZo* this, PlayState* play);
void EnZo_DoNothing(EnZo* this, PlayState* play);

ActorInit En_Zo_InitVars = {
    ACTOR_EN_ZO,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ZO,
    sizeof(EnZo),
    (ActorFunc)EnZo_Init,
    (ActorFunc)EnZo_Destroy,
    (ActorFunc)EnZo_Update,
    (ActorFunc)EnZo_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 18, 64, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(0, 0),
    /* Deku Stick     */ DMG_ENTRY(0, 0),
    /* Horse trample  */ DMG_ENTRY(0, 0),
    /* Explosives     */ DMG_ENTRY(0, 0),
    /* Zora boomerang */ DMG_ENTRY(0, 0),
    /* Normal arrow   */ DMG_ENTRY(0, 0),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, 0),
    /* Hookshot       */ DMG_ENTRY(0, 0),
    /* Goron punch    */ DMG_ENTRY(0, 0),
    /* Sword          */ DMG_ENTRY(0, 0),
    /* Goron pound    */ DMG_ENTRY(0, 0),
    /* Fire arrow     */ DMG_ENTRY(0, 0),
    /* Ice arrow      */ DMG_ENTRY(0, 0),
    /* Light arrow    */ DMG_ENTRY(0, 0),
    /* Goron spikes   */ DMG_ENTRY(0, 0),
    /* Deku spin      */ DMG_ENTRY(0, 0),
    /* Deku bubble    */ DMG_ENTRY(0, 0),
    /* Deku launch    */ DMG_ENTRY(0, 0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(0, 0),
    /* Zora barrier   */ DMG_ENTRY(0, 0),
    /* Normal shield  */ DMG_ENTRY(0, 0),
    /* Light ray      */ DMG_ENTRY(0, 0),
    /* Thrown object  */ DMG_ENTRY(0, 0),
    /* Zora punch     */ DMG_ENTRY(0, 0),
    /* Spin attack    */ DMG_ENTRY(0, 0),
    /* Sword beam     */ DMG_ENTRY(0, 0),
    /* Normal Roll    */ DMG_ENTRY(0, 0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0),
    /* Unblockable    */ DMG_ENTRY(0, 0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0),
    /* Powder Keg     */ DMG_ENTRY(0, 0),
};

static AnimationInfoS sAnimationInfo[] = {
    { &gZoraIdleAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gZoraIdleAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gZoraSurfacingAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gZoraHandsOnHipsTappingFootAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gZoraArmsOpenAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gZoraThrowRupeeAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gZoraWalkAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
};

s8 sBodyParts[] = { -1, 1, 12, 13, 14, 9, 10, 11, 0, 6, 7, 8, 3, 4, 5, 2, -1, -1, -1, -1 };
s8 sParentBodyParts[] = { 0, 0, 0, 0, 3, 4, 0, 6, 7, 0, 9, 10, 0, 12, 13 };
u8 sShadowSizes[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

s32 EnZo_ChangeAnim(SkelAnime* skelAnime, s16 animIndex) {
    s16 frameCount;
    s32 didChange = false;

    if ((animIndex >= 0) && (animIndex < ARRAY_COUNT(sAnimationInfo))) {
        didChange = true;
        frameCount = sAnimationInfo[animIndex].frameCount;
        if (frameCount < 0) {
            frameCount = Animation_GetLastFrame(sAnimationInfo[animIndex].animation);
        }
        Animation_Change(skelAnime, sAnimationInfo[animIndex].animation, sAnimationInfo[animIndex].playSpeed,
                         sAnimationInfo[animIndex].startFrame, frameCount, sAnimationInfo[animIndex].mode,
                         sAnimationInfo[animIndex].morphFrames);
    }
    return didChange;
}

s32 EnZo_PlayWalkingSound(EnZo* this, PlayState* play) {
    u8 leftWasGrounded;
    u8 rightWasGrounded;
    s32 waterSfxId;
    u16 sfxId;
    u8 isFootGrounded;

    leftWasGrounded = this->isLeftFootGrounded;
    rightWasGrounded = this->isRightFootGrounded;

    if (this->actor.bgCheckFlags & BGCHECKFLAG_WATER) {
        if (this->actor.depthInWater < 20.0f) {
            waterSfxId = NA_SE_PL_WALK_WATER0 - SFX_FLAG;
        } else {
            waterSfxId = NA_SE_PL_WALK_WATER1 - SFX_FLAG;
        }
        sfxId = waterSfxId + SFX_FLAG;
    } else {
        sfxId = SurfaceType_GetSfx(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId) + SFX_FLAG;
    }

    this->isLeftFootGrounded = isFootGrounded = SubS_IsFloorAbove(play, &this->leftFootPos, -6.0f);
    if ((this->isLeftFootGrounded) && (!leftWasGrounded) && (isFootGrounded)) {
        Actor_PlaySfx(&this->actor, sfxId);
    }

    this->isRightFootGrounded = isFootGrounded = SubS_IsFloorAbove(play, &this->rightFootPos, -6.0f);
    if ((this->isRightFootGrounded) && (!rightWasGrounded) && (isFootGrounded)) {
        Actor_PlaySfx(&this->actor, sfxId);
    }

    return 0;
}

void EnZo_Blink(EnZo* this, s32 maxEyeIndex) {
    if (DECR(this->blinkTimer) == 0) {
        this->eyeIndex++;
        if (this->eyeIndex >= maxEyeIndex) {
            this->eyeIndex = 0;
            this->blinkTimer = Rand_S16Offset(30, 30);
        }
    }
}

void EnZo_UpdateCollider(EnZo* this, PlayState* play) {
    this->collider.dim.pos.x = this->actor.world.pos.x;
    this->collider.dim.pos.y = this->actor.world.pos.y;
    this->collider.dim.pos.z = this->actor.world.pos.z;

    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

void EnZo_LookAtPlayer(EnZo* this, PlayState* play) {
    static TrackOptionsSet sTrackOptions = {
        { 0xFA0, 4, 1, 3 },
        { 0x1770, 4, 1, 6 },
        { 0xFA0, 4, 1, 3 },
        { 0x1770, 4, 1, 6 },
    };
    Player* player = GET_PLAYER(play);
    Vec3f point;

    SkelAnime_Update(&this->skelAnime);
    if (SubS_AngleDiffLessEqual(this->actor.shape.rot.y, 0x2710, this->actor.yawTowardsPlayer)) {
        point.x = player->actor.world.pos.x;
        point.y = player->bodyPartsPos[PLAYER_BODYPART_HEAD].y + 3.0f;
        point.z = player->actor.world.pos.z;
        SubS_TrackPoint(&point, &this->actor.focus.pos, &this->actor.shape.rot, &this->trackTarget, &this->headRot,
                        &this->upperBodyRot, &sTrackOptions);
    } else {
        Math_SmoothStepToS(&this->trackTarget.x, 0, 4, 1000, 1);
        Math_SmoothStepToS(&this->trackTarget.y, 0, 4, 1000, 1);

        Math_SmoothStepToS(&this->headRot.x, 0, 4, 1000, 1);
        Math_SmoothStepToS(&this->headRot.y, 0, 4, 1000, 1);

        Math_SmoothStepToS(&this->upperBodyRot.x, 0, 4, 1000, 1);
        Math_SmoothStepToS(&this->upperBodyRot.y, 0, 4, 1000, 1);
    }

    EnZo_Blink(this, 3);
    SubS_FillLimbRotTables(play, this->limbRotY, this->limbRotZ, 20);
}

void EnZo_Walk(EnZo* this, PlayState* play) {
    if (ENZO_GET_PATH(&this->actor) != 0x3F) {
        EnZo_ChangeAnim(&this->skelAnime, 6);
    }

    if (ENZO_GET_PATH(&this->actor) != 0x3F) {
        this->actionFunc = EnZo_FollowPath;
    } else {
        this->actionFunc = EnZo_DoNothing;
    }
}

void EnZo_FollowPath(EnZo* this, PlayState* play) {
    s16 speed;
    Vec3f pos;

    Math_SmoothStepToF(&this->actor.speed, 1.0f, 0.4f, 1000.0f, 0.0f);
    speed = this->actor.speed * 400.0f;
    if (SubS_CopyPointFromPath(this->path, this->waypoint, &pos) && SubS_MoveActorToPoint(&this->actor, &pos, speed)) {
        this->waypoint++;
        if (this->waypoint >= this->path->count) {
            this->waypoint = 0;
        }
    }

    if (this->actor.depthInWater > 60.0f) {
        EnZo_ChangeAnim(&this->skelAnime, 1);
        this->actionFunc = EnZo_TreadWater;
        this->actor.gravity = 0.0f;
        this->actor.speed = 0.0f;
    }
}

void EnZo_TreadWater(EnZo* this, PlayState* play) {
    f32 targetYVel;

    if (this->actor.depthInWater < (sREG(0) + 50.0f)) {
        targetYVel = -1.0f;
    } else {
        targetYVel = 1.0f;
    }
    Math_ApproachF(&this->actor.velocity.y, targetYVel, (sREG(1) + 18.0f) * 0.01f, (sREG(2) + 12.0f) * 0.01f);
}

void EnZo_DoNothing(EnZo* this, PlayState* play) {
}

void EnZo_Init(Actor* thisx, PlayState* play) {
    EnZo* this = THIS;
    s32 pad;

    ActorShape_Init(&this->actor.shape, 0.0f, NULL, 0.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &gZoraSkel, NULL, this->jointTable, this->morphTable, ZORA_LIMB_MAX);
    EnZo_ChangeAnim(&this->skelAnime, 0);

    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);

    this->path = SubS_GetPathByIndex(play, ENZO_GET_PATH(&this->actor), 0x3F);
    Actor_SetScale(&this->actor, 0.01f);

    this->actionFunc = EnZo_Walk;
    this->actor.gravity = -4.0f;
}

void EnZo_Destroy(Actor* thisx, PlayState* play) {
    EnZo* this = THIS;

    Collider_DestroyCylinder(play, &this->collider);
}

void EnZo_Update(Actor* thisx, PlayState* play) {
    EnZo* this = THIS;

    this->actionFunc(this, play);
    Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 0.0f, 0.0f, 4);
    EnZo_LookAtPlayer(this, play);
    EnZo_PlayWalkingSound(this, play);
    EnZo_UpdateCollider(this, play);
}

s32 EnZo_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx,
                          Gfx** gfx) {
    EnZo* this = THIS;

    if (limbIndex == ZORA_LIMB_HEAD) {
        Matrix_Translate(1500.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        Matrix_RotateXS(this->headRot.y, MTXMODE_APPLY);
        Matrix_RotateZS(-this->headRot.x, MTXMODE_APPLY);
        Matrix_Translate(-1500.0f, 0.0f, 0.0f, MTXMODE_APPLY);
    }

    if (limbIndex == ZORA_LIMB_TORSO) {
        Matrix_RotateXS(-this->upperBodyRot.y, MTXMODE_APPLY);
        Matrix_RotateZS(-this->upperBodyRot.x, MTXMODE_APPLY);
    }

    if ((limbIndex == ZORA_LIMB_TORSO) || (limbIndex == ZORA_LIMB_LEFT_UPPER_ARM) ||
        (limbIndex == ZORA_LIMB_RIGHT_UPPER_ARM)) {
        rot->y += (s16)(Math_SinS(this->limbRotY[limbIndex]) * 200.0f);
        rot->z += (s16)(Math_CosS(this->limbRotZ[limbIndex]) * 200.0f);
    }
    return false;
}

void EnZo_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx, Gfx** gfx) {
    EnZo* this = THIS;
    Vec3f sp30 = { 400.0f, 0.0f, 0.0f };
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };

    if (sBodyParts[limbIndex] >= 0) {
        Matrix_MultVec3f(&zeroVec, &this->bodyPartsPos[sBodyParts[limbIndex]]);
    }
    if (limbIndex == ZORA_LIMB_HEAD) {
        Matrix_MultVec3f(&sp30, &this->actor.focus.pos);
    }
    if (limbIndex == ZORA_LIMB_LEFT_FOOT) {
        Matrix_MultVec3f(&zeroVec, &this->leftFootPos);
    }
    if (limbIndex == ZORA_LIMB_RIGHT_FOOT) {
        Matrix_MultVec3f(&zeroVec, &this->rightFootPos);
    }
}

static Gfx sTransparencyDlist[] = {
    gsDPSetRenderMode(AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                          G_RM_FOG_SHADE_A,
                      AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                          GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)),
    gsDPSetAlphaCompare(G_AC_THRESHOLD),
    gsSPEndDisplayList(),
};

void EnZo_Draw(Actor* thisx, PlayState* play) {
    EnZo* this = THIS;
    s32 i;
    u8* shadowTex = GRAPH_ALLOC(play->state.gfxCtx, SUBS_SHADOW_TEX_SIZE);
    u8* shadowTexIter;
    TexturePtr eyeTextures[] = { gZoraEyeOpenTex, gZoraEyeHalfTex, gZoraEyeClosedTex };

    OPEN_DISPS(play->state.gfxCtx);
    func_8012C28C(play->state.gfxCtx);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gDPPipeSync(POLY_OPA_DISP++);
    gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(eyeTextures[this->eyeIndex]));
    gSPSegment(POLY_OPA_DISP++, 0x0C, &sTransparencyDlist[2]);

    POLY_OPA_DISP =
        SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                           EnZo_OverrideLimbDraw, EnZo_PostLimbDraw, &this->actor, POLY_OPA_DISP);
    Matrix_RotateXS(0, MTXMODE_NEW);

    for (i = 0, shadowTexIter = shadowTex; i < SUBS_SHADOW_TEX_SIZE; i++) {
        *shadowTexIter = 0;
        shadowTexIter++;
    }
    for (i = 0; i < 5; i++) {
        SubS_GenShadowTex(this->bodyPartsPos, &this->actor.world.pos, shadowTex, i / 5.0f,
                          ARRAY_COUNT(this->bodyPartsPos), sShadowSizes, sParentBodyParts);
    }

    SubS_DrawShadowTex(&this->actor, &play->state, shadowTex);
    CLOSE_DISPS(play->state.gfxCtx);
}
