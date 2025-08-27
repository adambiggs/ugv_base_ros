# UGV Base ROS - Build Instructions

This document provides step-by-step instructions for building the Waveshare UGV Base ROS project on Linux.

## Prerequisites

- Ubuntu 22.04 or similar Linux distribution
- Internet connection for downloading dependencies
- ESP32 development board

## Installation Steps

### 1. Install Arduino IDE and Dependencies

```bash
# Update package list
sudo apt update

# Install Arduino IDE and required packages
sudo apt install arduino libstdc++6 -y
```

### 2. Install Arduino CLI

```bash
# Download Arduino CLI
wget https://downloads.arduino.cc/arduino-cli/arduino-cli_1.3.0_Linux_64bit.tar.gz

# Extract to /usr/local/bin
sudo tar -xzf arduino-cli_1.3.0_Linux_64bit.tar.gz -C /usr/local/bin/

# Add to PATH (add this to your ~/.bashrc for permanent access)
export PATH="/usr/local/bin:$PATH"
```

### 3. Install ESP32 Board Support

```bash
# Add ESP32 board manager URL
arduino-cli config init
arduino-cli core update-index --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

# Install ESP32 core
arduino-cli core install esp32:esp32 --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

### 4. Install Required Libraries

```bash
# Install all required Arduino libraries
arduino-cli lib install "ArduinoJson" "Adafruit SSD1306" "INA219_WE" "ESP32Encoder" "PID_v2" "SimpleKalmanFilter" "Adafruit ICM20X" "ICM20948_WE" "SparkFun 9DoF IMU Breakout - ICM 20948 - Arduino Library"
```

### 5. Install Custom SCServo Library

```bash
# Create libraries directory if it doesn't exist
mkdir -p ~/Arduino/libraries

# Copy the custom SCServo library
cp -r SCServo ~/Arduino/libraries/
```

### 6. Fix Library Compatibility Issues

The project requires some compatibility fixes for newer ESP32 core versions:

#### Fix ESP-NOW Callback Function

Edit `ROS_Driver/esp_now_ctrl.h` and update the `OnDataSent` function signature:

```cpp
// Change from:
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)

// To:
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status)
```

And update the MAC address access:

```cpp
// Change from:
snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
         mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

// To:
snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
         info->des_addr[0], info->des_addr[1], info->des_addr[2], info->des_addr[3], info->des_addr[4], info->des_addr[5]);
```

## Building the Project

### Compile the Project

```bash
# Make sure you're in the project directory
cd /path/to/ugv_base_ros

# Compile the project
arduino-cli compile --fqbn esp32:esp32:esp32 ROS_Driver
```

### Upload to ESP32 (Optional)

```bash
# Upload to ESP32 (replace /dev/ttyUSB0 with your actual port)
arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyUSB0 ROS_Driver
```

## Project Structure

```
ugv_base_ros/
├── ROS_Driver/           # Main Arduino sketch
│   ├── ROS_Driver.ino   # Main sketch file
│   ├── *.h              # Header files for various modules
│   └── data/            # Configuration files
├── SCServo/             # Custom servo library
└── README.md            # Project documentation
```

## Key Features

- **Closed-loop Speed Control** with PID
- **Web App** based on ESP32
- **IMU** support (ICM20948)
- **OLED Screen** control
- **LED Lights** control (12V switches)
- **JSON Commands** for control
- **Camera PT** support
- **RoArm-M2** support
- **ESP-NOW** communication

## Configuration

To configure the robot type, send this JSON command via UART/USB@115200:

```json
{"T":900,"main":2,"module":2}
```

Where:
- `main`: 1=RaspRover, 2=UGV Rover, 3=UGV Beast
- `module`: 0=Nothing, 1=RoArm-M2, 2=Camera PT

## Troubleshooting

### Common Issues

1. **Library not found errors**: Make sure all libraries are installed in the correct location (`~/Arduino/libraries/`)

2. **ESP-NOW compilation errors**: Ensure the callback function signatures match the ESP32 core version

3. **Memory issues**: The project uses 92% of program storage, which is normal for this complex project

### Memory Usage

- **Program Storage**: 1,214,759 bytes (92% of 1,310,720 bytes)
- **Dynamic Memory**: 49,940 bytes (15% of 327,680 bytes)

## License

This project is licensed under the GNU General Public License v3.0. See the LICENSE file for details.

## Support

For issues and questions, refer to the original project repository and documentation.
