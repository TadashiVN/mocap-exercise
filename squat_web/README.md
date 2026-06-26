# Wearable IMU-Based Squat Posture Evaluation

A university student prototype that uses ESP32 nodes and MPU6050 IMU sensors to collect body motion data and classify squat posture using a pre-trained TensorFlow/Keras model.

> **Project scope:** This is a student prototype for learning embedded sensing, Serial communication, and basic AI-based posture evaluation. It is not a production-ready fitness product.

## 1. Overview

The system collects accelerometer and gyroscope data from wearable MPU6050 sensors. Each ESP32 node streams sensor data to a PC through Serial Port. A Python application reads the serial data, builds a sliding time window, runs model inference, and displays the latest squat posture result on a simple Flask web interface.

## 2. System Architecture

![System Architecture](docs/images/system_architecture.svg)

**Image note:** This diagram is already included. If you have a better real diagram from your report, replace it with `docs/images/system_architecture.png` and update the image path above.

```mermaid
flowchart LR
    A[MPU6050 IMU Sensors] --> B[ESP32 Nodes]
    B --> C[Serial Port / USB]
    C --> D[Python Realtime Reader]
    D --> E[TensorFlow / Keras Model]
    E --> F[Flask Web Interface]
    F --> G[Squat Posture Result]
```

## 3. Hardware Setup

- 5 ESP32 development boards
- 5 MPU6050 IMU sensors
- USB cables or power source for each ESP32
- PC / laptop for receiving serial data and running the web app

**Image to add:** `docs/images/hardware_setup.jpg`  
**Suggested caption:** ESP32 and MPU6050 sensor setup used for collecting squat motion data.  
**What to capture:** Take a clear photo of the ESP32 boards, MPU6050 sensors, wires, and the PC/laptop connection. If the sensors are attached to the body, add one extra image named `docs/images/wearable_setup.jpg`.

## 4. Software & Tools

- Arduino IDE
- Python
- Flask
- TensorFlow / Keras
- PySerial

## 5. Repository Structure

```text
.
├── app.py                         # Flask web server
├── realtime_predict.py            # Realtime serial reading and model inference
├── requirements.txt               # Python dependencies
├── templates/
│   └── index.html                 # Simple web UI
├── firmware/
│   └── ESP32_node/
│       └── ESP32_node.ino         # ESP32 firmware for MPU6050 data streaming
├── models/
│   └── squat_model.h5             # Pre-trained squat classification model
└── docs/
    └── images/                    # Project diagrams, photos, and screenshots
```

## 6. Data Format

Each ESP32 node sends CSV data through Serial Port:

```text
NODE_ID,SEQ,LOCAL_TS,AX,AY,AZ,GX,GY,GZ
```

Where:

- `NODE_ID`: ESP32 sensor node ID
- `SEQ`: sample sequence number
- `LOCAL_TS`: local timestamp from ESP32
- `AX, AY, AZ`: accelerometer values
- `GX, GY, GZ`: gyroscope values

**Image to add:** `docs/images/serial_monitor.png`  
**Suggested caption:** Serial Monitor output showing IMU data streamed from an ESP32 node.  
**What to capture:** Open Arduino Serial Monitor and take a screenshot showing several lines of data in this format.

## 7. Squat Classes

The current model predicts four squat posture classes:

- `squat_correct`
- `squat_bent_back`
- `squat_shallow`
- `squat_heels_up`

**Image to add:** `docs/images/web_demo.png`  
**Suggested caption:** Flask web interface displaying the latest squat posture prediction and confidence score.  
**What to capture:** Run `python app.py`, open `http://localhost:5000`, click **START**, and take a screenshot when the page shows a prediction result.

**Optional image to add:** `docs/images/data_plot.png`  
**Suggested caption:** Example accelerometer and gyroscope signals collected from MPU6050 sensors during squat movement.  
**What to capture:** If you have a Python plot of IMU values over time, place it here. This is optional but useful for showing the sensor-data part.

## 8. How to Run

### 8.1 Install Python dependencies

```bash
pip install -r requirements.txt
```

### 8.2 Upload ESP32 firmware

Open `firmware/ESP32_node/ESP32_node.ino` in Arduino IDE.

Before uploading, update the `NODE_ID` value for each ESP32 board:

```cpp
#define NODE_ID 1
```

Suggested node IDs:

```text
1 = head
2 = left_wrist
3 = right_wrist
4 = left_ankle
5 = right_ankle
```

### 8.3 Configure COM ports

In `realtime_predict.py`, update `NODE_PORTS` to match your computer:

```python
NODE_PORTS = {
    1: "COM3",
    2: "COM16",
    3: "COM17",
    4: "COM18",
    5: "COM15"
}
```

### 8.4 Run the Flask web app

```bash
python app.py
```

Open the browser at:

```text
http://localhost:5000
```

Click **START** to begin realtime prediction.

## 9. My Contribution

- Programmed ESP32 nodes to read MPU6050 accelerometer and gyroscope data
- Streamed IMU data from ESP32 to PC through Serial Port
- Built a Python realtime prediction script for reading sensor data and running model inference
- Built a simple Flask web interface to display squat posture results

## 10. Project Status

This is a university student project / prototype. The main purpose is to practice sensor-based data collection, serial communication, and basic AI-based posture evaluation using embedded devices and Python.

## 11. Notes for Reviewers

- COM ports must be changed depending on the PC.
- The current model is included for demo purposes.
- Real hardware photos and screenshots should be added under `docs/images/` before publishing the repository.
- Local datasets, logs, IDE folders, and environment files should not be uploaded to GitHub.
