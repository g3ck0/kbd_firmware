FROM qmkfm/qmk_cli

ENV QMK_FIRMWARE_VERSION=0.31.8

VOLUME /qmk_firmware
WORKDIR /qmk_firmware
