idf_component_register(SRCS "esp32_ttgo.c"
                    INCLUDE_DIRS "include"
                    PRIV_INCLUDE_DIRS "priv_include"
                    REQUIRES driver
                    PRIV_REQUIRES esp_lcd)

