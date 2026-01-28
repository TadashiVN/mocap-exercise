from flask import Flask, render_template, jsonify
import threading
import realtime_predict as rp

app = Flask(__name__)
realtime_thread = None

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/start")
def start():
    global realtime_thread
    if realtime_thread is None:
        realtime_thread = threading.Thread(target=rp.start_realtime, daemon=True)
        realtime_thread.start()
    return jsonify({"status": "started"})

@app.route("/result")
def result():
    action, conf = rp.get_latest_result()
    return jsonify({
        "action": action,
        "confidence": round(conf, 2)
    })

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
