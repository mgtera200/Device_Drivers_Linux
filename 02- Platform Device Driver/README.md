# Platform Device Driver for Raspberry Pi LEDs

This folder contains the source code for a platform device driver developed for Raspberry Pi 4, enabling control over two platform devices: a Red LED and a Green LED. The driver interfaces with the GPIO pins on the Raspberry Pi to toggle the LEDs based on input signals.

## Features

- **Platform Device Management**: The driver manages two platform devices, providing control over a Red LED and a Green LED.
- **GPIO Interaction**: Utilizes the GPIO driver to interact with the hardware, allowing for efficient LED control.
- **Dynamic LED Control**: LEDs respond dynamically to input signals, providing real-time feedback.

## Getting Started

To use the platform device driver, follow these steps:

1. Ensure the necessary kernel headers are installed on your Raspberry Pi.
2. Compile the driver module using the provided Makefile.
3. Insert the compiled module into the kernel using `insmod`.
4. Access the device files under `/dev` to control the LEDs.

## Dependencies

- Linux Kernel Headers
- GPIO Driver

## Usage

Once the driver is inserted into the kernel, you can control the LEDs by writing to the device files under `/dev`. For example:

```bash
echo "1" > /dev/red_led    # Turn on the Red LED
echo "0" > /dev/green_led  # Turn off the Green LED

