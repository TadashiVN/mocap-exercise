# Image Guide for README

Add the following images to make the GitHub repository more complete and easier to understand.

| File name | Required? | What to show | Suggested caption |
|---|---:|---|---|
| `system_architecture.svg` | Already added | System flow from MPU6050 to Flask result | System architecture of the IMU-based squat posture evaluation prototype. |
| `hardware_setup.jpg` | Yes | ESP32 boards, MPU6050 sensors, wiring, and PC/laptop connection | ESP32 and MPU6050 sensor setup used for collecting squat motion data. |
| `wearable_setup.jpg` | Optional | Sensors attached to body positions such as head, wrists, and ankles | Wearable sensor placement used during squat data collection. |
| `serial_monitor.png` | Yes | Arduino Serial Monitor showing CSV data lines | Serial Monitor output showing IMU data streamed from an ESP32 node. |
| `web_demo.png` | Yes | Flask page showing action/prediction and confidence | Flask web interface displaying the latest squat posture prediction and confidence score. |
| `data_plot.png` | Optional | Plot of accelerometer/gyroscope values over time | Example accelerometer and gyroscope signals collected during squat movement. |

Recommended before applying:

1. Add `hardware_setup.jpg`.
2. Add `serial_monitor.png`.
3. Add `web_demo.png`.
4. Add `wearable_setup.jpg` only if the photo looks clear and professional.
