VIA_ENABLE         = yes
VIAL_ENABLE        = yes
VIALRGB_ENABLE     = yes
ENCODER_MAP_ENABLE = yes
LAYER_LOCK_ENABLE = yes
#modules
LEADER_ENABLE = yes


#to improve shrink of the file
LTO_ENABLE = yes
#to enable rgb matrix to use them
RGBLIGHT_ENABLE = no
# RGB_MATRIX_ENABLE = yes
# COMBO_ENABLE = yes
# Source files compilation
SRC += xcase/xcase.c
SRC += feature/modcase.c
SRC += feature/ligth.c
SRC += feature/modprocessrecord.c
SRC += handsdown/moutis_semantickeys.c
