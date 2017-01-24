$(call inherit-product-if-exists, target/allwinner/astar-common/astar-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := astar_parrot
PRODUCT_DEVICE := astar-parrot
PRODUCT_MODEL := Atar R16 Parrot
