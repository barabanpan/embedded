# First time with ESP-IDF and interrupts

## Objective
 - Use ESP-IDF for the first time
 - Get acquainted with the concept of interrupts

Simulated in WOKWI: [Link.](https://wokwi.com/projects/468915969113694209)

## Components
 - ESP32 C3
 - 2x LEDs
 - 2x 220Ω resistor for the LEDs
 - 1x push button
 - 10kΩ resistor for button pull up

## Description
![simulation](images/simulation.gif)

Red LED switches state every second (timer interrupt).

Greeen LED switches state when button is pushed (GPIO interrupt).

## Install and use idf.py
[Installation instruction](https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32/get-started/linux-macos-setup.html)

Then
```commandline
get_idf
idf.py create-project esp_interrupts
cd esp_interrupts
idf.py set-target esp32c3
idf.py build
idf.py flash
idf.py monitor
```

![On actual board](images/actual_board.gif)

## Useful links
 - [GPIO interrupts tutorial](https://randomnerdtutorials.com/esp-idf-esp32-gpio-interrupts/)
 - [idf.py installation instruction](https://docs.espressif.com/projects/esp-idf/en/v5.0/esp32/get-started/linux-macos-setup.html)

