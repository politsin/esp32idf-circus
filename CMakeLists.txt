# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(startup)
#list(APPEND EXTRA_COMPONENT_DIRS ds18x20)
#list(APPEND EXTRA_COMPONENT_DIRS onewire)

# list(APPEND EXTRA_COMPONENT_DIRS iot)
list(APPEND EXTRA_COMPONENT_DIRS i2cdev)
# list(APPEND EXTRA_COMPONENT_DIRS hx711)
# list(APPEND EXTRA_COMPONENT_DIRS DendoStepper)
list(APPEND EXTRA_COMPONENT_DIRS esp32-c3-rotary-encoder)

# Create a SPIFFS image from the contents of the 'font' directory
# that fits the partition named 'storage'. FLASH_IN_PROJECT indicates that
# the generated image should be flashed when the entire project is flashed to
# the target with 'idf.py -p PORT flash
# spiffs_create_partition_image(storage data FLASH_IN_PROJECT)

