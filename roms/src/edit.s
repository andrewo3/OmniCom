; To change if you edit the number of metatiles:
;   LevelEditTileTable, MapEditTypesRow

EditorOffX = 8
EditorOffY = 6

ReadJoyCombine:
  jsr ReadJoy
.proc KeyCombine
  lda keydown+1
  ora keydown
  sta keydown
  lda keylast+1
  ora keylast
  sta keylast
  rts
.endproc

.if 0
.proc ClearSRAM
  lda #$00
  sta 0
  lda #$60
  sta 1
  tay
Loop:
  sta (0),y
  iny
  bne :-
  inc 1
  ldx 1
  cpx #$7f
  beq Exit
  bne Loop
Exit:
  rts
.endproc
.endif

.proc StartEditorFromMenu
  ldx #0
  txa
: ;jsr rand_8_safe
  ;and #7
  sta LevelBuf,x
  inx
  bne :-
  lda #0
  sta EditorCurX
  sta EditorCurY
  sta EditorCurT
  lda #1
  sta LevelEditMode
.endproc
.proc StartEditorNormal
  jsr ClearOAM
  jsr wait_vblank
  jsr init_sound
  lda #0
  sta PPUMASK
  sta PPUSCROLL
  sta PPUSCROLL
  jsr ClearName

; to fix a minor bug with springs:
  ldx #0
: ldy LevelBuf,x
  lda MetatileBecomes,y
  sta LevelBuf,x
  inx
  bne :-

  PositionXY 0,  7,  5
  lda #16
  jsr PutCheckeredBar

  PositionXY 0,  7,  21
  lda #16|128
  jsr PutCheckeredBar

  PositionXY 0,  7,  23
; draw list of available tiles
  ldx #0
DrawTileSelect: 
  ldy MapEditTypesRow,x
  lda LevelEditTileTable,y
  sta PPUDATA
  lda #' '
  sta PPUDATA
  inx
  cpx #MapEditTypesRowLen
  bne DrawTileSelect

  PositionPrintXY 0, 10,  4, "Level Studio"

  PositionPrintXY 0,  1, 12, "-MAIN-"
  PositionPrintXY 0,  2, 14, "Play"
  PositionPrintXY 0,  1, 16, "Spawn"
  PositionPrintXY 0,  1, 18, "Setup"
  PositionPrintXY 0,  2, 20, "Quit"

;  lda HasExtraRAM
;  jeq DontDrawSRAM
;    PositionPrintXY 0,  32-7, 12, "-SRAM-"
;    PositionPrintXY 0,  32-6, 14, "Save"
;    PositionPrintXY 0,  32-6, 16, "Load"
;    PositionPrintXY 0,  32-6, 18, "View"
;  DontDrawSRAM:

; render the view
  ldx #0
  stx 0
RenderView:
  ldy 0
  lda EditViewRowAddr+0,y
  sta PPUADDR
  lda EditViewRowAddr+1,y
  sta PPUADDR
  iny
  iny
  cpy #32
  beq ExitRender
  sty 0
  lda #$BD
  sta PPUDATA
  pha
  .dj_loop #16, 1
    ldy LevelBuf,x
    lda LevelEditTileTable,y
    sta PPUDATA
    inx
  .end_djl
  pla
  sta PPUDATA

  jmp RenderView
ExitRender:
; clear attributes
  lda #$23
  sta PPUADDR
  lda #$c0
  sta PPUADDR
  ldx #64
  lda #0
: sta PPUDATA
  dex
  bne :-

  lda #0
  sta PPUSCROLL
  sta PPUSCROLL
  jsr wait_vblank
  lda #0
  sta PPUSCROLL
  sta PPUSCROLL
  lda #BG_ON | OBJ_ON
  sta PPUMASK
  jsr wait_vblank
  jmp EditorLoop
.endproc

MapEditTypesRowLen = 7
.proc MapEditTypesRow
  .byt METATILE_PLATFM, METATILE_SOLID, METATILE_SPIKES, METATILE_SPRING, METATILE_LAUNCH, METATILE_MIRROR, METATILE_MONEY
.endproc

.proc EditorPosToIndex
  lda EditorCurY
  .repeat 4
    asl
  .endrep
  ora EditorCurX
  tax
  rts
.endproc

.proc EditorLoop
  jsr ReadJoyCombine
  lda keydown
  and #KEY_LEFT|KEY_DOWN|KEY_UP|KEY_RIGHT
  sta 0
  lda keylast
  and #KEY_LEFT|KEY_DOWN|KEY_UP|KEY_RIGHT
  cmp 0
  bne :+
    lda LevelEditKeyRepeat
    cmp #16
    bcc NoRepeat
    lda retraces
    and #3
    bne NoRepeat
    lda keylast
    and #~(KEY_LEFT|KEY_DOWN|KEY_UP|KEY_RIGHT)
    sta keylast
  NoRepeat:

    lda LevelEditKeyRepeat
    bmi DidRepeat
    inc LevelEditKeyRepeat
    jmp DidRepeat
: lda #0
  sta LevelEditKeyRepeat
DidRepeat:

  lda keydown
  and #KEY_LEFT
  beq :+
    lda keylast
    and #KEY_LEFT
    bne :+
      dec EditorCurX
: lda keydown
  and #KEY_RIGHT
  beq :+
    lda keylast
    and #KEY_RIGHT
    bne :+
      inc EditorCurX
: lda keydown
  and #KEY_UP
  beq :+
    lda keylast
    and #KEY_UP
    bne :+
      dec EditorCurY
      bpl :+
        lda #14
        sta EditorCurY
: lda keydown
  and #KEY_DOWN
  beq :+
    lda keylast
    and #KEY_DOWN
    bne :+
      inc EditorCurY
      lda EditorCurY
      cmp #15
      bne :+
        lda #0
        sta EditorCurY
  :

  lda keydown ; get blocks
  and #KEY_B
  beq ExitGetBlock
    lda keylast
    and #KEY_B
    bne ExitGetBlock
      jsr EditorPosToIndex
      lda LevelBuf,x
      jeq EditSelectBlockType
        ldy #0
GetBlk: cmp MapEditTypesRow,y
        beq GotBlock
        iny
        cpy #MapEditTypesRowLen
        bne GetBlk
        beq ExitGetBlock
    GotBlock:
      sty EditorCurT
      jmp EditSelectBlockType
ExitGetBlock:

  lda keydown ; do side menu
  and #KEY_START
  beq :+
    lda keylast
    and #KEY_START
    jeq EditSelectActionType
:


  lda keylast
  eor #255
  and keydown
  and #KEY_UP|KEY_LEFT|KEY_DOWN|KEY_RIGHT
  sta 0

  lda EditorCurX
  and #15
  sta EditorCurX
  lda EditorCurY
  and #15
  sta EditorCurY

  lda keydown ; put blocks
  and #KEY_A
  beq ExitPutBlock
    lda keylast
    and #KEY_A
    eor #KEY_A
    ora 0
    beq ExitPutBlock
      jsr EditorPosToIndex
      lda LevelBuf,x
      bne :+
        ldy EditorCurT
        lda MapEditTypesRow,y
        sta LevelBuf,x
        jmp UpdateBlockPPU
      :
      lda #0
      sta LevelBuf,x
    UpdateBlockPPU:
      tax
      lda EditorCurY
      asl
      tay
      lda EditViewRowAddr+0,y
      sta PPUADDR
      lda EditViewRowAddr+1,y
      sec
      adc EditorCurX
      sta PPUADDR
      lda LevelEditTileTable,x
      sta PPUDATA
      lda #0
      sta PPUSCROLL
      sta PPUSCROLL
ExitPutBlock:

  lda EditorCurY
  asl
  asl
  asl
  add #EditorOffY*8-1
  sta OAM_YPOS+(4*0)
  lda EditorCurX
  asl
  asl
  asl
  add #EditorOffX*8
  sta OAM_XPOS+(4*0)
  lda retraces
  and #1
  ora #$4c
  sta OAM_TILE+(4*0)
  lda #0
  sta OAM_ATTR+(4*0)

  sta OAM_TILE+(4*2)

  clc
  jsr DispTilePickerArrow

  jsr wait_vblank
  jmp EditorLoop
.endproc

.proc EditSelectActionType
  lda #0
  sta 0
Loop:
  jsr ReadJoy
  lda #0
  sta PlayerEnabled
  sta PlayerEnabled+1
  jsr EnableForPress
  jsr KeyCombine

  lda keydown
  and #KEY_UP
  beq :+
    lda keylast
    and #KEY_UP
    bne :+
      dec 0
      bpl :+
        lda #3
        sta 0
  :

  lda keydown
  and #KEY_DOWN
  beq :+
    lda keylast
    and #KEY_DOWN
    bne :+
      inc 0
      lda 0
      cmp #4
      bne :+
        lda #0
        sta 0
  :

  lda keydown
  and #KEY_A
  beq :+
    lda keylast
    and #KEY_A
    bne :+
    lda 0
    asl
    tax
    lda ActionAddresses+1,x
    pha
    lda ActionAddresses+0,x
    pha
    rts
  :
  lda keydown
  and #KEY_B
  beq :+
    lda keylast
    and #KEY_B
    jeq EditorLoop    
  :

  lda #6*8
  sta OAM_XPOS+(4*2)
  lda #$4b
  sta OAM_TILE+(4*2)
  lda #OAM_XFLIP
  sta OAM_ATTR+(4*2)
  lda 0
  asl
  asl
  asl
  asl
  add #14*8
  sta OAM_YPOS+(4*2)
  jmp Loop
ActionAddresses:
  .raddr NewGame
  .raddr SetSpawn
  .raddr EditLevelGoal
  .raddr StartMainMenu
SetSpawn:
  lda EditorCurX
  sta LevelEditStartX
  lda EditorCurY
  sta LevelEditStartY
  jmp EditorLoop
.endproc

EditEnemyNames:
  .byt "None",0
EditEnemyNameWalker:
  .byt "Walker",0
EditEnemyNameJumpy:
  .byt "Jumpy",0
EditEnemyNameDiagShoot:
  .byt "DiagShoot",0
EditEnemyNameWaitShoot:
  .byt "WaitShoot",0
EditEnemyNameBomb:
  .byt "Bomb",0
EditEnemyNameFlyBomber:
  .byt "FlyBomber",0
EditEnemyNameFlyDiagonal:
  .byt "FlyDiag",0
EditEnemyNameFloatShootV:
  .byt "FloatShootV",0
EditEnemyNameGoomba:
  .byt "Goomba",0
EditEnemyNameSneaker:
  .byt "Sneaker",0
EditEnemyNameGeorge:
  .byt "Volcano",0
EditEnemyNameRetard:
  .byt "Giana Owl",0
EditEnemyNameToastbot:
  .byt "Toastbot",0
EditEnemyNameBurger1:
  .byt "Burger1",0
EditEnemyNameBurger2:
  .byt "Burger2",0
EditEnemyNameBurger3:
  .byt "Burger3",0
EditEnemyNameBottle:
  .byt "Water",0
EditEnemyNameBillNye:
  .byt "Bill Nye",0
EditEnemyNameTable:
  .byt 0
  .byt EditEnemyNameWalker-EditEnemyNames
  .byt EditEnemyNameJumpy-EditEnemyNames
  .byt EditEnemyNameDiagShoot-EditEnemyNames
  .byt EditEnemyNameWaitShoot-EditEnemyNames
  .byt EditEnemyNameBomb-EditEnemyNames
  .byt EditEnemyNameFlyBomber-EditEnemyNames
  .byt EditEnemyNameFlyDiagonal-EditEnemyNames
  .byt EditEnemyNameFloatShootV-EditEnemyNames
  .byt EditEnemyNameGoomba-EditEnemyNames
  .byt EditEnemyNameSneaker-EditEnemyNames
  .byt EditEnemyNameGeorge-EditEnemyNames
  .byt EditEnemyNameRetard-EditEnemyNames
  .byt EditEnemyNameToastbot-EditEnemyNames
  .byt EditEnemyNameBurger1-EditEnemyNames
  .byt EditEnemyNameBurger2-EditEnemyNames
  .byt EditEnemyNameBurger3-EditEnemyNames
  .byt EditEnemyNameBottle-EditEnemyNames
  .byt EditEnemyNameBillNye-EditEnemyNames
.proc EditLevelGoal
  jsr wait_vblank
  jsr ClearOAM
  lda #0
  sta PPUMASK
  jsr ClearName
  PositionPrintXY 0,  10, 3, "Level Setup"
  PositionPrintXY 0,   9, 4, "-------------"

  PositionPrintXY 0, 13,  6,  "Goal"
  PositionPrintXY 0,  7,  8,    "Type:"
  PositionPrintXY 0,  5,  9,  "Amount:"

  PositionPrintXY 0, 12, 11,  "Enemies"
  PositionPrintXY 0,  5, 13,  "Enemy1:"
  PositionPrintXY 0,  5, 14,  "Enemy2:"
  PositionPrintXY 0,  5, 15,  "Enemy3:"
  PositionPrintXY 0,  5, 16,  "Enemy4:"
  PositionPrintXY 0,  5, 17,  "Distr.:"
  PositionPrintXY 0,  6, 18,   "Hard?:"
  PositionPrintXY 0,  8, 19,     "Max:"

  ldx #0
  stx 0 ; current option
: txa
  pha
  jsr EditUpdateGoalLine
  pla
  tax
  inx
  cpx #9
  bne :-

  jsr wait_vblank
  lda #0
  sta PPUSCROLL
  sta PPUSCROLL
  lda #BG_ON+OBJ_ON
  sta PPUMASK
.endproc
.proc EditConfigLoop
  jsr wait_vblank
  jsr ReadJoyCombine

  lda keydown ; Press the A button to copy current enemy to next slot
  and #KEY_A
  beq NotCopy
    lda keylast
    and #KEY_A
    bne NotCopy
      ldx 0
      cpx #2
      bcc NotCopy
      cpx #5
      bcs NotCopy
      lda LevelConfigBytes,x
      sta LevelConfigBytes+1,x
      lda 0
      add #1
      jsr EditUpdateGoalLine
      lda #0
      sta PPUSCROLL
      sta PPUSCROLL
  NotCopy:

  lda keydown
  and #KEY_LEFT
  beq NotLeft
    lda keylast
    and #KEY_LEFT
    bne NotLeft
      ldx 0
      jsr LaunchLoad
      sub #1
      bpl :+
        lda ConfigMax,x
        sub #1
      :
      ldx 0
      jsr LaunchSave
      txa
      jsr EditUpdateGoalLine
      lda #0
      sta PPUSCROLL
      sta PPUSCROLL
  NotLeft:

  lda keydown
  and #KEY_RIGHT
  beq NotRight
    lda keylast
    and #KEY_RIGHT
    bne NotRight
      ldx 0
      jsr LaunchLoad
      add #1
      cmp ConfigMax,x
      bcc :+
        lda #0
      :
      ldx 0
      jsr LaunchSave
      txa
      jsr EditUpdateGoalLine
      lda #0
      sta PPUSCROLL
      sta PPUSCROLL
  NotRight:

  lda keydown
  and #KEY_UP
  beq :+
    lda keylast
    and #KEY_UP
    bne :+
      dec 0
      bpl :+
        lda #8
        sta 0
  :

  lda keydown
  and #KEY_DOWN
  beq :+
    lda keylast
    and #KEY_DOWN
    bne :+
      inc 0
      lda 0
      cmp #9
      bne :+
        lda #0
        sta 0
  :

  lda keydown
  and #KEY_B
  beq :+
    lda keylast
    and #KEY_B
    jeq StartEditorNormal
  :
  lda #11*8
  sta OAM_XPOS+(4*0)
  lda retraces
  and #1
  ora #$4c
  sta OAM_TILE+(4*0)
  lda #OAM_XFLIP
  sta OAM_ATTR+(4*0)

  lda 0
  asl
  asl
  asl
  add #(8*8)-1
  sta OAM_YPOS+(4*0)
  ldy 0
  cpy #2
  bcc :+
    add #8*3
    sta OAM_YPOS+(4*0)
  :

  jmp EditConfigLoop

LaunchLoad:
  sta 1
  txa
  asl
  tay
  lda ConfigLoadAddrTable+1,y
  pha
  lda ConfigLoadAddrTable+0,y
  pha
  lda 1
  rts
ConfigLoadAddrTable:
  .raddr LoadGoalType
  .raddr LoadGoalAmount
  .raddr LoadEnemyType
  .raddr LoadEnemyType
  .raddr LoadEnemyType
  .raddr LoadEnemyType
  .raddr LoadEnemyDistr
  .raddr LoadEnemyHard
  .raddr LoadEnemyAmount
LaunchSave:
  sta 1
  txa
  asl
  tay
  lda ConfigSaveAddrTable+1,y
  pha
  lda ConfigSaveAddrTable+0,y
  pha
  lda 1
  rts
ConfigSaveAddrTable:
  .raddr SaveGoalType
  .raddr SaveGoalAmount
  .raddr SaveEnemyType
  .raddr SaveEnemyType
  .raddr SaveEnemyType
  .raddr SaveEnemyType
  .raddr SaveEnemyDistr
  .raddr SaveEnemyHard
  .raddr SaveEnemyAmount
LoadGoalType:
  lda LevelConfigBytes+0
  and #3
  rts
LoadGoalAmount:
  lda LevelConfigBytes+0
  lsr
  lsr
  rts
LoadEnemyType:
  lda LevelConfigBytes,x
  rts
LoadEnemyDistr:
  lda LevelConfigBytes+1
  lsr
  lsr
  lsr
  lsr
  rts
LoadEnemyHard:
  lda GameDifficulty
  rts
LoadEnemyAmount:
  lda LevelConfigBytes+1
  and #7
  rts
SaveGoalType:
  sta 2
  lda LevelConfigBytes+0
  and #%11111100
  ora 2
  sta LevelConfigBytes+0
  rts
SaveGoalAmount:
  asl
  asl
  sta 2
  lda LevelConfigBytes+0
  and #3
  ora 2
  sta LevelConfigBytes+0
  rts
SaveEnemyType:
  sta LevelConfigBytes,x
  rts
SaveEnemyDistr:
  asl
  asl
  asl
  asl
  sta 2
  lda LevelConfigBytes+1
  and #%11101111
  ora 2
  sta LevelConfigBytes+1
  rts
SaveEnemyHard:
  sta GameDifficulty
  rts
SaveEnemyAmount:
  sta 2
  lda LevelConfigBytes+1
  and #%11111000
  ora 2
  sta LevelConfigBytes+1
  rts
ConfigMax:
  .byt 4, 32, 19, 19, 19, 19, 2, 2, 8
.endproc

.proc EditUpdateGoalLine ; A = line to update
  cmp #8 ; enemy max
  bcc :+
  PositionXY 0,  12, 19
  lda LevelConfigBytes+1
  and #7
  add #1
  jmp PutHex

: cmp #7 ; difficulty
  bcc :+
  PositionXY 0,  12, 18
  lda GameDifficulty
  asl
  asl
  tax
  lda Difficulties+0,x
  sta PPUDATA
  lda Difficulties+1,x
  sta PPUDATA
  lda Difficulties+2,x
  sta PPUDATA
  lda Difficulties+3,x
  sta PPUDATA
  rts
  Difficulties: .byt "EasyHard"

: cmp #6 ; enemy distribution
  bcc :+
  PositionXY 0,  12, 17
  lda LevelConfigBytes+1
  and #%10000
  beq NotAltDist
  ldy #' '
  lda #'5'
  sta PPUDATA
  sty PPUDATA
  sta PPUDATA
  lda #'3'
  sty PPUDATA
  sta PPUDATA
  sty PPUDATA
  sta PPUDATA
  rts
NotAltDist:
  ldy #' '
  lda #'4'
  sta PPUDATA
  sty PPUDATA
  sta PPUDATA
  sty PPUDATA
  sta PPUDATA
  sty PPUDATA
  sta PPUDATA
  rts

: cmp #2 ; enemy types
  bcc NotEnemyTypes
  sub #2
  pha
  asl
  tax
  lda EnemyTypePPUAddr+0,x
  sta PPUADDR
  lda EnemyTypePPUAddr+1,x
  sta PPUADDR
  pla
  tax
  lda LevelConfigBytes+2,x
  tax
  ldy EditEnemyNameTable,x
: lda EditEnemyNames,y
  beq :+
  sta PPUDATA
  iny
  bne :-
: lda #' '
  sta PPUDATA
  sta PPUDATA
  sta PPUDATA
  sta PPUDATA
  sta PPUDATA
  sta PPUDATA
  rts
EnemyTypePPUAddr:
  .ppuxy 0, 12, 13
  .ppuxy 0, 12, 14
  .ppuxy 0, 12, 15
  .ppuxy 0, 12, 16

NotEnemyTypes:
  cmp #1 ; goal amount
  bcc :+
  PositionXY 0,  12,  9
  lda LevelConfigBytes+0
  lsr
  lsr
  add #1
  tay
  lda BCD64,y
  jsr PutHex
  rts

: PositionXY 0,  12,  8 ; goal type
  lda LevelConfigBytes+0
  and #3
  asl
  sta TempVal
  asl
  asl
  add TempVal
  tax
  ldy #10
: lda GoalTypeText,x
  sta PPUDATA
  inx
  dey
  bne :-
  rts
GoalTypeText: .byt "Mario Bros"
              .byt "Fast Eddie"
              .byt "Survive   "
              .byt "HotelMario"
.endproc

.proc EditSelectBlockType
  lda #0
  sta EditorBouncyFX
Loop:
  jsr ReadJoyCombine

  lda keydown
  and #KEY_LEFT
  beq :+
    lda keylast
    and #KEY_LEFT
    bne :+
      dec EditorCurT
      bpl :+
        lda #MapEditTypesRowLen-1
        sta EditorCurT
  :

  lda keydown
  and #KEY_RIGHT
  beq :+
    lda keylast
    and #KEY_RIGHT
    bne :+
      inc EditorCurT
      lda EditorCurT
      cmp #MapEditTypesRowLen
      bcc :+
        lda #0
        sta EditorCurT
  :

  lda keydown
  and #KEY_A
  beq :+
    lda keylast
    and #KEY_A
    bne :+
    jmp EditorLoop
  :

  lda keydown
  sec 
  jsr DispTilePickerArrow
  inc EditorBouncyFX

  jsr wait_vblank
  jmp Loop
.endproc

.proc DispTilePickerArrow
  php
  lda EditorCurT
  asl
  asl
  asl
  asl
  add #EditorOffX*8-8
  sta OAM_XPOS+(4*1)
  lda #$5b
  sta OAM_TILE+(4*1)
  lda #0
  sta OAM_ATTR+(4*1)

  lda EditorBouncyFX
  lsr
  lsr
  and #7
  tax
  lda #-4
  plp
  bcs :+
  lda #0
: add #EditorOffY*8+18*8
  sta OAM_YPOS+(4*1)
  rts
.endproc

.proc LevelEditTileTable
  .byt $20, $FF, $FD, $FE, $FC, $00, $00, $00, $00, '$', $80, $F0, $F0, $F0
.endproc

.proc EditViewRowAddr
  .repeat 16, I
    .ppuxy 0, 7, (I+6)
  .endrep
.endproc

.proc PutCheckeredBar
  sta 1
  and #31
  tax

  lda #$DE
  bit 1    ; choose top or bottom set of corners
  bpl :+
  lda #$EE
: pha
  sta PPUDATA

  lda #$BE
: sta PPUDATA
  dex
  bne :-

  pla
  ora #$F
  sta PPUDATA
  rts
.endproc
