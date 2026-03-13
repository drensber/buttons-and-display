# Button controlled display

This ia a fairly simple example of a program that will change the state of a time display depending on buttons that are pressed and held or released.

## Building the code

### For ESP32 running FreeRTOS
```
cd ports/esp-idf
idf.py build
```

### For ESP32 running Zephyr
```
west build -p always -b esp32_devkitc_wroom/esp32/procpu ports/zephyr
```

### For desktop Linux unit-test environment
```
sh build_unit_test.sh
```
