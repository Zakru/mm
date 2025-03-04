/*
 * File: z_obj_hgdoor.c
 * Overlay: ovl_Obj_Hgdoor
 * Description: Music Box House - Closet Door
 */

#include "z_obj_hgdoor.h"
#include "objects/object_hgdoor/object_hgdoor.h"

#define FLAGS (ACTOR_FLAG_100000)

#define THIS ((ObjHgdoor*)thisx)

void ObjHgdoor_Init(Actor* thisx, PlayState* play);
void ObjHgdoor_Destroy(Actor* thisx, PlayState* play);
void ObjHgdoor_Update(Actor* thisx, PlayState* play);
void ObjHgdoor_Draw(Actor* thisx, PlayState* play);

void ObjHgdoor_SetupIdle(ObjHgdoor* this);
void ObjHgdoor_Idle(ObjHgdoor* this, PlayState* play);
void ObjHgdoor_SetupCutscene(ObjHgdoor* this);
void ObjHgdoor_PlayCutscene(ObjHgdoor* this, PlayState* play);
void ObjHgdoor_SetupCsAction(ObjHgdoor* this);
void ObjHgdoor_HandleCsAction(ObjHgdoor* this, PlayState* play);
void ObjHgdoor_SetupStopCs(ObjHgdoor* this);
void ObjHgdoor_StopCs(ObjHgdoor* this, PlayState* play);
s32 ObjHgdoor_Rotate(ObjHgdoor* this, PlayState* play);

ActorInit Obj_Hgdoor_InitVars = {
    ACTOR_OBJ_HGDOOR,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_HGDOOR,
    sizeof(ObjHgdoor),
    (ActorFunc)ObjHgdoor_Init,
    (ActorFunc)ObjHgdoor_Destroy,
    (ActorFunc)ObjHgdoor_Update,
    (ActorFunc)ObjHgdoor_Draw,
};

static s16 sOpenFlag = 0;

void ObjHgdoor_SetChild(ObjHgdoor* this, PlayState* play) {
    Actor* actorIterator = play->actorCtx.actorLists[ACTORCAT_PROP].first;

    while (actorIterator) {
        if ((actorIterator->id == ACTOR_OBJ_HGDOOR) && (&this->dyna.actor != actorIterator)) {
            this->dyna.actor.child = actorIterator;
            break;
        }
        actorIterator = actorIterator->next;
    }
}

void ObjHgdoor_SetParent(ObjHgdoor* this, PlayState* play) {
    Actor* actorIterator = play->actorCtx.actorLists[ACTORCAT_PROP].first;

    while (actorIterator) {
        if (actorIterator->id == ACTOR_EN_HG) {
            this->dyna.actor.parent = actorIterator;
            break;
        }
        actorIterator = actorIterator->next;
    }
}

void ObjHgdoor_Init(Actor* thisx, PlayState* play) {
    ObjHgdoor* this = THIS;
    s32 pad;
    CollisionHeader* header = NULL;

    Actor_SetScale(&this->dyna.actor, 0.1f);
    DynaPolyActor_Init(&this->dyna, 1);
    if (OBJHGDOOR_IS_RIGHT_DOOR(&this->dyna.actor)) {
        CollisionHeader_GetVirtual(&object_hgdoor_Colheader_001D10, &header);
    } else {
        CollisionHeader_GetVirtual(&object_hgdoor_Colheader_0018C0, &header);
    }
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, header);
    this->rotation = 0;
    this->timer = 0;
    this->cutscene = this->dyna.actor.cutscene;
    ObjHgdoor_SetupIdle(this);
}

void ObjHgdoor_Destroy(Actor* thisx, PlayState* play) {
    ObjHgdoor* this = THIS;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void ObjHgdoor_SetupIdle(ObjHgdoor* this) {
    this->actionFunc = ObjHgdoor_Idle;
}

void ObjHgdoor_Idle(ObjHgdoor* this, PlayState* play) {
    if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_75_20) && !CHECK_WEEKEVENTREG(WEEKEVENTREG_52_20) &&
        (this->dyna.actor.xzDistToPlayer < 100.0f) && (this->dyna.actor.playerHeightRel < 40.0f) &&
        OBJHGDOOR_IS_RIGHT_DOOR(&this->dyna.actor)) {
        ObjHgdoor_SetChild(this, play);
        ObjHgdoor_SetParent(this, play);
        ObjHgdoor_SetupCutscene(this);
    }
}

void ObjHgdoor_SetupCutscene(ObjHgdoor* this) {
    this->actionFunc = ObjHgdoor_PlayCutscene;
}

void ObjHgdoor_PlayCutscene(ObjHgdoor* this, PlayState* play) {
    if (ActorCutscene_GetCanPlayNext(this->cutscene)) {
        ActorCutscene_Start(this->cutscene, &this->dyna.actor);
        ObjHgdoor_SetupCsAction(this);
        ObjHgdoor_SetupCsAction((ObjHgdoor*)this->dyna.actor.child);
    } else {
        if (ActorCutscene_GetCurrentIndex() == 0x7C) {
            ActorCutscene_Stop(0x7C);
        }
        ActorCutscene_SetIntentToPlay(this->cutscene);
    }
}

void ObjHgdoor_SetupCsAction(ObjHgdoor* this) {
    this->csAction = 0x63;
    this->actionFunc = ObjHgdoor_HandleCsAction;
}

void ObjHgdoor_HandleCsAction(ObjHgdoor* this, PlayState* play) {
    s32 actionIndex;

    if (Cutscene_CheckActorAction(play, 483)) {
        actionIndex = Cutscene_GetActorActionIndex(play, 483);
        if (this->csAction != play->csCtx.actorActions[actionIndex]->action) {
            this->csAction = play->csCtx.actorActions[actionIndex]->action;
            switch (play->csCtx.actorActions[actionIndex]->action) {
                case 1:
                    Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_WOOD_DOOR_OPEN_SPEEDY);
                    if ((this->dyna.actor.parent != NULL) && (this->dyna.actor.parent->id == ACTOR_EN_HG)) {
                        this->dyna.actor.parent->colChkInfo.health = 1;
                    }
                    sOpenFlag = 1;
                    break;

                case 2:
                    break;
            }
        }
        if ((sOpenFlag == 1) && ObjHgdoor_Rotate(this, play)) {
            ObjHgdoor_SetupStopCs(this);
        }
    } else {
        this->csAction = 0x63;
    }
}

void ObjHgdoor_SetupStopCs(ObjHgdoor* this) {
    this->timer = 0;
    this->actionFunc = ObjHgdoor_StopCs;
}

void ObjHgdoor_StopCs(ObjHgdoor* this, PlayState* play) {
    if (this->timer++ > 80) {
        if (!ActorCutscene_GetCanPlayNext(this->cutscene)) {
            ActorCutscene_Stop(this->cutscene);
        }
    }
}

/**
 * @brief Function to increment the rotation angle of the door
 *
 * @return true when door is fully open, false otherwise
 */
s32 ObjHgdoor_Rotate(ObjHgdoor* this, PlayState* play) {
    if (this->rotation < 0x5555) {
        this->rotation += 0x1388;
    } else {
        return true;
    }

    return false;
}

void ObjHgdoor_Open(ObjHgdoor* this) {
    this->dyna.actor.shape.rot.y = this->dyna.actor.home.rot.y;
    if (OBJHGDOOR_IS_RIGHT_DOOR(&this->dyna.actor)) {
        this->dyna.actor.shape.rot.y += this->rotation;
    } else {
        this->dyna.actor.shape.rot.y -= this->rotation;
    }
}

void ObjHgdoor_Update(Actor* thisx, PlayState* play) {
    ObjHgdoor* this = THIS;

    this->actionFunc(this, play);
    ObjHgdoor_Open(this);
}

void ObjHgdoor_Draw(Actor* thisx, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);
    func_8012C28C(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    if (OBJHGDOOR_IS_RIGHT_DOOR(thisx)) {
        gSPDisplayList(POLY_OPA_DISP++, object_hgdoor_DL_001AB0);
        gSPDisplayList(POLY_OPA_DISP++, object_hgdoor_DL_001BA8);
    } else {
        gSPDisplayList(POLY_OPA_DISP++, object_hgdoor_DL_001670);
        gSPDisplayList(POLY_OPA_DISP++, object_hgdoor_DL_001768);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}
