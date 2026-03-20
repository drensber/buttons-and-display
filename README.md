# Button controlled display

Change the state of a time display depending on the state of buttons that are pressed and held or released.

It's fairly simple and generic embedded software functionality. The more novel thing I've done here is make all of the application-level code use an abstraction layer that can be linked against either FreeRTOS, Zephyr, or a unit test environement.

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
