.segment "ZEROPAGE"

  MaxNumPlayers = 2

  retraces:	       .res 1
  keydown:	       .res MaxNumPlayers
  keylast: 	       .res MaxNumPlayers

  HasExtraRAM:     .res 1

  random1:         .res 2
  random2:         .res 2

  psg_sfx_state:    .res 32

  PlayerEnabled:    .res MaxNumPlayers
  PlayerPX:	        .res MaxNumPlayers
  PlayerPYH:        .res MaxNumPlayers
  PlayerPYL:        .res MaxNumPlayers
  PlayerVYH:        .res MaxNumPlayers
  PlayerVYL:        .res MaxNumPlayers
  PlayerInvincible: .res MaxNumPlayers
  PlayerDropLock:   .res MaxNumPlayers
  PlayerDead:       .res MaxNumPlayers
  PlayerXShifted:   .res 1

  PlayerDir:        .res MaxNumPlayers ; 0 or 1
  PlayerHead:       .res MaxNumPlayers ; tile number
  PlayerBody:       .res MaxNumPlayers ; not a tile number
  PlayerHeld:       .res MaxNumPlayers ; tile number?
  PlayerPowerType:  .res MaxNumPlayers ;
  PlayerPowerTime:  .res MaxNumPlayers ;
  PlayerJumpCancelLock: .res MaxNumPlayers

  PlayerLives:      .res MaxNumPlayers
  PlayerHealth:     .res MaxNumPlayers
  MaxHealthNormal    = 4
  MaxHealthHard      = 3
  MaxNumBlockUpdates = 3
  MaxNumTileUpdates  = 5
  BlockUpdateA1:   .res MaxNumBlockUpdates
  BlockUpdateA2:   .res MaxNumBlockUpdates
  BlockUpdateB1:   .res MaxNumBlockUpdates
  BlockUpdateB2:   .res MaxNumBlockUpdates
  BlockUpdateT1:   .res MaxNumBlockUpdates
  BlockUpdateT2:   .res MaxNumBlockUpdates
  BlockUpdateT3:   .res MaxNumBlockUpdates
  BlockUpdateT4:   .res MaxNumBlockUpdates

  TileUpdateA1:    .res MaxNumTileUpdates
  TileUpdateA2:    .res MaxNumTileUpdates
  TileUpdateT:     .res MaxNumTileUpdates

  StatusRow1:      .res 28
  StatusRow2:      .res 12

  EnableNMIDraw:   .res 1

  GameDifficulty:  .res 1
  DIFFICULTY_STANDARD = 0
  DIFFICULTY_HARDER   = 1
  DIFFICULTY_MOREHARD = 2

  LevelNumber:     .res 1

  OamPtr:          .res 1
  TempVal:         .res 4
  FlashColor:      .res 1

  TouchRight:      .res 1
  TouchBottom:     .res 1
  TouchTopA:       .res 1
  TouchTopB:       .res 1
  TouchLeftA:      .res 1
  TouchLeftB:      .res 1
  TouchWidthA:     .res 1
  TouchWidthB:     .res 1
  TouchHeightA:    .res 1
  TouchHeightB:    .res 1

  mul_factor_a:    .res 1
  mul_factor_x:    .res 1
  mul_product_lo:  .res 1
  mul_product_hi:  .res 1
 
  EditorCurX:      .res 1
  EditorCurY:      .res 1
  EditorCurT:      .res 1

  EnemyWidth:      .res 1

  NumScoreDigits = 5
  ScoreDigits:     .res NumScoreDigits*2
  .res 1

.segment "BSS"
  .res 14
  soundBSS:        .res 64
  NumRowsMade:     .res 1

  EditorBouncyFX:  .res 1 ; a timer

  IsWarmboot:    .res 1
  WarmbootData:  .res 4
  PlayerStartPX: .res MaxNumPlayers
  PlayerStartPY: .res MaxNumPlayers

  MaxDelayedMetaEdits = 10
  DelayedMetaEditIndx: .res MaxDelayedMetaEdits
  DelayedMetaEditTime: .res MaxDelayedMetaEdits
  DelayedMetaEditType: .res MaxDelayedMetaEdits

  MaxExplosions = 3
  ExplosionPosX: .res MaxExplosions
  ExplosionPosY: .res MaxExplosions
  ExplosionSize: .res MaxExplosions
  ExplosionTime: .res MaxExplosions

  PowerupLen  =  3
  PowerupPX:   .res PowerupLen
  PowerupPY:   .res PowerupLen
  PowerupVX:   .res PowerupLen
  PowerupVY:   .res PowerupLen
  PowerupF:    .res PowerupLen
  ; E----TTT
  ;  E - Enabled
  ;  T - Type of powerup

  NeedPowerupSound: .res 1
  BothDeadTimer: .res 1

  BulletLen   = 12
  ObjectLen   = 12
  BulletPX:    .res BulletLen
  BulletPY:    .res BulletLen
  BulletPYL:   .res BulletLen
  BulletVX:    .res BulletLen
  BulletVY:    .res BulletLen
  BulletVYL:   .res BulletLen
  BulletLife:  .res BulletLen
  BulletF:     .res BulletLen  ;flags
  ; EP--TTTT
  ;  E - enabled
  ;  P - player bullet, if 1 can damage enemies (as well as players!)
  ;  T - type

  ObjectPX:    .res ObjectLen
  ObjectPYH:   .res ObjectLen
  ObjectPYL:   .res ObjectLen
  ObjectVYH:   .res ObjectLen
  ObjectVYL:   .res ObjectLen

  ObjectF1:    .res ObjectLen ; HHTTTTTD, Health, Type, Direction
  ObjectF2:    .res ObjectLen ; ----SSSS, State
  ObjectF3:    .res ObjectLen ; PPP-----, Parameter, Weapon
  ObjectTimer: .res ObjectLen ; when timer reaches 0, reset state

; level config
  LevelGoalType:      .res 1
  LevelGoalParam:     .res 1
  LevelEnemyPool:     .res 16
  ScreenEnemiesCount: .res 1
  MaxScreenEnemies:   .res 1
  LevelWon:           .res 1
  Timer60:            .res 1
  LevelConfigBytes:   .res 8
  LevelMapPointer = LevelConfigBytes + 6
  LevelEditMode:      .res 1
  LevelEditStartX:    .res 1
  LevelEditStartY:    .res 1
  IsMappedLevel:      .res 1
  LevelEditKeyRepeat: .res 1

  LevelBuf = $700
  BulletMap = LevelBuf - 64
  AttribMap = BulletMap - 64
  CollectMap = AttribMap - 32
.code
