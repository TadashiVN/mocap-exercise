<img width="923" height="498" alt="image" src="https://github.com/user-attachments/assets/ef04d520-aa39-41e3-ab8c-81c6ee5f40d1" /># Wearable IMU-Based Squat Posture Evaluation

A university student prototype that uses ESP32 nodes and MPU6050 IMU sensors to collect body motion data and classify squat posture using a pre-trained TensorFlow/Keras model.

> **Project scope:** This is a student prototype for learning embedded sensing, Serial communication, and basic AI-based posture evaluation. It is not a production-ready fitness product.

## 1. Overview

The system collects accelerometer and gyroscope data from wearable MPU6050 sensors. Each ESP32 node streams sensor data to a PC through Serial Port. A Python application reads the serial data, builds a sliding time window, runs model inference, and displays the latest squat posture result on a simple Flask web interface.
<img width="1067" height="606" alt="image" src="https://github.com/user-attachments/assets/e0b95772-9cba-4b1f-a118-2b49347cc3a9" />

## 2. System Architecture


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

<img width="939" height="430" alt="image" src="https://github.com/user-attachments/assets/6a939661-2cd8-4bdf-80bd-db3ed24721c8" />


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

<img width="474" height="409" alt="image" src="https://github.com/user-attachments/assets/9dc6911e-61d7-4de6-b8c1-b4ee58baeff1" />


## 7. Squat Classes

The current model predicts four squat posture classes:

- `squat_correct`
- `squat_bent_back`
- `squat_shallow`
- `squat_heels_up`

<img width="801" height="484" alt="image" src="https://github.com/user-attachments/assets/156327d2-c0b6-4c57-91c3-4ec6f745b621" />
<img width="991" height="487" alt="image" src="https://github.com/user-attachments/assets/0fe902e5-85c6-4cb2-99c6-8244148ff140" />
<img width="923" height="498" alt="image" src="https://github.com/user-attachments/assets/7a189832-43f6-4a72-bd59-ed8e0265fb09" />
<img width="792" height="494" alt="image" src="https://github.com/user-attachments/assets/401b3050-c154-4c4b-94ac-3fa6d2e5a105" />


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


## 9. Project Status

This is a university student project / prototype. The main purpose is to practice sensor-based data collection, serial communication, and basic AI-based posture evaluation using embedded devices and Python.

## 10. Notes for Reviewers

- COM ports must be changed depending on the PC.
- The current model is included for demo purposes.
- Real hardware photos and screenshots should be added under `docs/images/` before publishing the repository.
- Local datasets, logs, IDE folders, and environment files should not be uploaded to GitHub.
