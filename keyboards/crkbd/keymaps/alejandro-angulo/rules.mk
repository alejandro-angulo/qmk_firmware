# Space optimizaiton, enables link time optimization
# See: https://github.com/qmk/qmk_firmware/issues/3224
EXTRAFLAGS += -flto

OLED_ENABLE  = yes
OLED_DRIVER = SSD1306
EXTRAKEY_ENABLE = yes
CONSOLE_ENABLE = yes
WPM_ENABLE = yes
