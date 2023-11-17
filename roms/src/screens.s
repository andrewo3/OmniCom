.proc StartMainMenu
  jsr init_sound
  lda #0
  sta EnableNMIDraw
  sta LevelEditMode
  lda #DIFFICULTY_STANDARD
  sta GameDifficulty

  lda #0
  sta PPUMASK
  sta PPUSCROLL
  sta PPUSCROLL
  jsr ClearOAM

  lda #<TitleName
  ldx #>TitleName
  jsr CopyFromCHR
  lda #0
  sta 0
  lda #5
  sta 1
  lda #$20
  sta PPUADDR
  lda #$00
  sta PPUADDR
  jsr PKB_unpackblk
  lda #0
  sta PPUSCROLL
  sta PPUSCROLL

  jsr wait_vblank

  lda #BG_ON | OBJ_ON
  sta PPUMASK
  jsr WaitForKey
  jsr EnableForPress
  jsr wait_vblank
.endproc
.proc StartMainMenu2
  ldy #MainMenuData-MenuData
  jsr RunChoiceMenu
  asl
  tax

  lda retraces
  beq :+
  ora #1
: sta r_seed

  lda MainMenuAddrs+1,x
  pha
  lda MainMenuAddrs+0,x
  pha
  rts
.endproc

.segment "CHR_DATA"
TitleName:
.addr EndTitle-TitleName
.incbin "title.pkb"
EndTitle:
.segment "CODE"

.proc MainMenuAddrs
  .raddr MenuNewGame
  .raddr MenuNewGameHard
  .raddr ShowDirections
  .raddr StartExtrasMenu
.endproc

.proc ExtrasMenuAddrs
  .raddr StartEditorFromMenu
  .raddr ShowCredits
  .raddr StartMainMenu2
  .raddr 0
.endproc

.proc LevelpackMenuAddrs
  .raddr NewGameStart1
  .raddr NewGameStart2
  .raddr NewGameStart3
  .raddr StartMainMenu2
.endproc

MenuNewGameHard:
  lda #DIFFICULTY_HARDER
  sta GameDifficulty
MenuNewGame:
  ldy #LevelPackMenuData-MenuData
  jsr RunChoiceMenu
  asl
  tax

  lda retraces
  beq :+
  ora #1
: sta r_seed

  lda LevelpackMenuAddrs+1,x
  pha
  lda LevelpackMenuAddrs+0,x
  pha
  rts

NewGameStart1:
  lda #0
  sta LevelNumber
  jmp NewGame
NewGameStart2:
  lda #12
  sta LevelNumber
  jmp NewGame
NewGameStart3:
  lda #24
  sta LevelNumber
  jmp NewGame

.proc StartExtrasMenu
  ldy #ExtrasMenuData-MenuData
  jsr RunChoiceMenu
  asl
  tax

  lda retraces
  beq :+
  ora #1
: sta r_seed

  lda ExtrasMenuAddrs+1,x
  pha
  lda ExtrasMenuAddrs+0,x
  pha
  rts  
.endproc

MenuData:
.proc MainMenuData
  .byt 4
  .ppuxy 0, 10, 20
  .byt "Normal Game",0
  .ppuxy 0, 10, 21
  .byt "Harder Game",0
  .ppuxy 0, 10, 22
  .byt "How To Play",0
  .ppuxy 0, 10, 23
  .byt "Extra Stuff",0
.endproc
.proc ExtrasMenuData
  .byt 3
  .ppuxy 0, 10, 20
  .byt "Level Edit",0
  .ppuxy 0, 10, 21
  .byt "Credits",0
  .ppuxy 0, 10, 22
  .byt "BACK",0
  .ppuxy 0, 10, 23
  .byt "       ",0
.endproc
.proc LevelPackMenuData
  .byt 4
  .ppuxy 0, 10, 20
  .byt "Level 1",0
  .ppuxy 0, 10, 21
  .byt "Level 13",0
  .ppuxy 0, 10, 22
  .byt "Level 25",0
  .ppuxy 0, 10, 23
  .byt "BACK",0
.endproc

.proc EnableForPress
  lda keydown+0
  beq :+
  lda #1
  sta PlayerEnabled+0
:
  lda keydown+1
  beq :+
  lda #1
  sta PlayerEnabled+1
: rts
.endproc
.proc RunChoiceMenu ; returns choice selected in A
  ldx MenuData,y
  stx 1             ; keep number of choices for later
  jsr ClearMenuChoices

NewRow:
  lda #0
  sta PPUSCROLL
  sta PPUSCROLL
  jsr wait_vblank
  iny               ; also skips over zeroes when branched here
  dex
  bmi NoMoreRows
  lda MenuData,y
  sta PPUADDR
  iny
  lda MenuData,y
  sta PPUADDR
  iny
: lda MenuData,y
  beq NewRow
  sta PPUDATA
  iny
  jmp :-
NoMoreRows:
  jsr ClearOAM
  lda #0
  sta 0            ; current pos

ChoiceLoop:
  jsr ReadJoy
  jsr EnableForPress

  lda keydown
  ora keydown+1
  and #KEY_UP
  beq :+
    lda keylast
    ora keylast+1
    and #KEY_UP
    bne :+
      dec 0
      bpl :+
        lda 1
        sta 0
        dec 0
  :

  lda keydown
  ora keydown+1
  and #KEY_DOWN
  beq :+
    lda keylast
    ora keylast+1
    and #KEY_DOWN
    bne :+
      inc 0
      lda 0
      cmp 1
      bcc :+
        lda #0
        sta 0
  :

  lda #10*8-16
  sta OAM_XPOS+(4*0)
  lda 0
  asl
  asl
  asl
  add #20*8-1
  sta OAM_YPOS+(4*0)
  sta 2
  lda #$28
  sta OAM_TILE+(4*0)
  lda #0
  sta OAM_ATTR+(4*0)

  lda keydown
  ora keydown+1
  and #KEY_A|KEY_START
  beq :+
    lda keylast
    ora keylast+1
    and #KEY_A|KEY_START
    beq ChoiceSelected
  :
  jsr wait_vblank
  jmp ChoiceLoop

ChoiceSelected:
  jsr ClearOAM
  lda 0
  rts
.endproc

.proc ClearMenuChoices
  lda #0
  sta PPUSCROLL
  sta PPUSCROLL
  jsr wait_vblank
  PositionXY 0, 10, 20
  jsr PrintSpaces
  PositionXY 0, 10, 21
  jsr PrintSpaces
  PositionXY 0, 10, 22
  jsr PrintSpaces
  PositionXY 0, 10, 23
  jsr PrintSpaces
  rts
PrintSpaces:
  lda #' '
  .repeat 12
    sta PPUDATA
  .endrep
  rts
.endproc

.macro PositionPrintXY NT, XP, YP, String
  PositionXY NT, XP, YP
  jsr PutStringImmediate
  .byt String, 0
.endmacro

.proc WaitForKeyTimed
: jsr wait_vblank
  jsr ReadJoy
  dey
  beq :+
  lda keydown
  ora keydown+1
  beq :-
  lda keylast
  ora keylast+1
  bne :-
: rts

.endproc

.proc PreLevelScreen
  jsr wait_vblank
  lda #0
  sta PPUMASK
  jsr ClearName

  PositionPrintXY 0, 8,6,  "= NEXT LEVEL ="
  PositionPrintXY 0, 10, 9, "Level: "
  lda LevelNumber
  add #1
  jsr PutDecimal
  PositionPrintXY 0, 10,11, "Lives: "

  lda PlayerEnabled
  beq :+
    lda PlayerLives
    jsr PutDecimal
    lda #' '
    sta PPUDATA
  :
  lda PlayerEnabled+1
  beq :+
    lda PlayerLives+1
    jsr PutDecimal
  :

  PositionXY 0, 7,13
  jsr DispGoal

  lda #0
  sta PPUSCROLL
  sta PPUSCROLL
  jsr wait_vblank
  lda #BG_ON | OBJ_ON
  sta PPUMASK
  ldy #100
  jsr WaitForKeyTimed
  lda #0
  sta PPUMASK
  rts

DispGoal:
  lda LevelGoalType
  asl
  tax
  lda GoalAddrs+1,x
  pha
  lda GoalAddrs+0,x
  pha
  rts
GoalAddrs:
  .raddr Defeat
  .raddr Collect 
  .raddr Survive
  .raddr Step
Defeat:
  jsr PutStringImmediate
  .byt "Defeat ",0
  lda LevelGoalParam
  jsr PutDecimal 
  jsr PutStringImmediate
  .byt " enemies",0
  rts
Collect:
  jsr PutStringImmediate
  .byt "Collect ",0
  lda LevelGoalParam
  jsr PutDecimal
  jsr PutStringImmediate
  .byt " dollars",0
  rts
Survive:
  jsr PutStringImmediate
  .byt "Survive ",0
  lda LevelGoalParam
  jsr PutDecimal
  jsr PutStringImmediate
  .byt " seconds",0
  rts
Step:
  jsr PutStringImmediate
  .byt "Green to red",0
  rts
.endproc

.proc ShowDirections
  jsr wait_vblank
  lda #0
  sta PPUMASK
  jsr ClearName
  PositionPrintXY 0, 8,4,  "Double Action"
  PositionPrintXY 0, 9,5,  "Blaster Guys"

  PositionPrintXY 0, 2,8,  "Complete the objectives for"
  PositionPrintXY 0, 2,9,  "each level to move on to the"
  PositionPrintXY 0, 2,10, "next one, while avoiding any"
  PositionPrintXY 0, 2,11, "hazards like enemy bullets."
  PositionPrintXY 0, 2,12, "Players can join in at any"
  PositionPrintXY 0, 2,13, "time by pressing A and B."

  PositionPrintXY 0, 8,16, "A-Jump"
  PositionPrintXY 0, 8,17, "B-Shoot"
  PositionPrintXY 0, 8,18, "Start-Pause"

  PositionPrintXY 0, 2,21, "Press anything to continue"

  lda #0
  sta PPUSCROLL
  sta PPUSCROLL
  jsr wait_vblank
  lda #BG_ON | OBJ_ON
  sta PPUMASK
  jsr WaitForKey
  jmp StartMainMenu
.endproc

.proc ShowCredits
  jsr wait_vblank
  lda #0
  sta PPUMASK
  jsr ClearName
  PositionPrintXY 0, 8,4,  "Double Action"
  PositionPrintXY 0, 9,5,  "Blaster Guys"

  PositionPrintXY 0, 2,8,  "Nearly everything"
  PositionPrintXY 0, 3,9,  "by NovaSquirrel"

  PositionPrintXY 0, 2,11, "Sound engine, boom sound"
  PositionPrintXY 0, 3,12, "and drums by Tepples"

  PositionPrintXY 0, 2,14, "Music is a heavily edited"
  PositionPrintXY 0, 3,15, "version of Morning Mood"

  PositionPrintXY 0, 2,21, "Press anything to continue"

  lda #0
  sta PPUSCROLL
  sta PPUSCROLL
  jsr wait_vblank
  lda #BG_ON | OBJ_ON
  sta PPUMASK
  jsr WaitForKey
  jmp StartMainMenu
.endproc

.proc LevelWasWon
  lda #0
  sta EnableNMIDraw
;  jsr init_sound
  lda #0
  sta PlayerVYH
  sta PlayerVYH+1
  sta PlayerVYL
  sta PlayerVYL+1
SoarLoop:
  jsr wait_vblank
  jsr ClearOAM
  jsr SlowdownMusic
  jsr update_sound

  ldx #0
  stx 15
SoarAllPlayers:
  jsr PlayerSoar
  bcc :+
  inc 15
: inx
  cpx #MaxNumPlayers
  bne SoarAllPlayers

  lda 15
  cmp #2
  bne SoarLoop

  lda LevelEditMode
  jeq NewLevel
  jmp StartEditorNormal

PlayerSoarDone:
  sec
  rts
PlayerSoar:
  lda PlayerEnabled,x ; skip if not enabled
  beq PlayerSoarDone
  lda PlayerPYH,x     ; skip if they're already up
  cmp #8
  bcc PlayerSoarDone

  lda PlayerPYL,x
  sub PlayerVYL,x
  bcs :+
    inc PlayerPYH,x
: sta PlayerPYL,x
  lda PlayerVYL,x
  add #32
  bcc :+
    inc PlayerVYH,x
: sta PlayerVYL,x
  lda PlayerPYH,x
  sub PlayerVYH,x
  sta PlayerPYH,x
NoSoar:
  jsr DispPlayer
  clc
  rts
.endproc

.proc SlowdownMusic
  lda music_tempoLo
  sub #<5
  sta music_tempoLo
  lda music_tempoHi
  sbc #>5
  sta music_tempoHi
  bmi :+
  rts
: lda #0
  sta music_tempoHi
  sta music_tempoLo
  jsr stop_music
  rts
.endproc
