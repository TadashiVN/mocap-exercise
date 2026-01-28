import serial
import threading
import time
import numpy as np
from collections import deque
import tensorflow as tf

MODEL_PATH = "squat_model.h5"

NODE_PORTS = {
    1: "COM3",
    2: "COM16",
    3: "COM17",
    4: "COM18",
    5: "COM15"
}

BAUDRATE = 115200
WINDOW_SIZE = 120
FEATURE_DIM = 6
PREDICT_INTERVAL = 1.0

CLASSES = [
    "squat_correct",
    "squat_bent_back",
    "squat_shallow",
    "squat_heels_up"
]

# ===== GLOBAL STATE =====
latest_action = "waiting"
latest_confidence = 0.0
is_running = False

print(" Loading model...")
model = tf.keras.models.load_model(MODEL_PATH)
print(" Model loaded")

buffers = {nid: deque(maxlen=WINDOW_SIZE) for nid in NODE_PORTS}
serial_ports = {}

# ===== SERIAL READ =====
def read_serial(nid, ser):
    while is_running:
        try:
            line = ser.readline().decode(errors="ignore").strip()
            if not line:
                continue

            parts = line.split(",")
            if len(parts) != 9:
                continue

            ax, ay, az = map(float, parts[3:6])
            gx, gy, gz = map(float, parts[6:9])

            buffers[nid].append([ax, ay, az, gx, gy, gz])
        except:
            time.sleep(0.01)

# ===== START REALTIME =====
def start_realtime():
    global latest_action, latest_confidence, is_running
    is_running = True

    # connect serial
    for nid, port in NODE_PORTS.items():
        ser = serial.Serial(port, BAUDRATE, timeout=1)
        time.sleep(2)
        ser.reset_input_buffer()
        serial_ports[nid] = ser

        t = threading.Thread(target=read_serial, args=(nid, ser), daemon=True)
        t.start()

    last_predict = time.time()

    while is_running:
        time.sleep(0.05)

        if not all(len(buffers[n]) >= WINDOW_SIZE for n in buffers):
            continue

        if time.time() - last_predict < PREDICT_INTERVAL:
            continue

        last_predict = time.time()

        window = []
        for i in range(WINDOW_SIZE):
            frame = []
            for nid in sorted(buffers):
                frame.extend(buffers[nid][i])
            window.append(frame)

        X = np.array(window, dtype=np.float32).reshape(1, WINDOW_SIZE, -1)

        probs = model.predict(X, verbose=0)[0]
        idx = np.argmax(probs)

        latest_action = CLASSES[idx]
        latest_confidence = float(probs[idx])

# ===== GET RESULT =====
def get_latest_result():
    return latest_action, latest_confidence
