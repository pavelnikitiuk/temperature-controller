Import("env")
import serial
import time
import subprocess

def reset_and_monitor(*args, **kwargs):
    port = env.get("UPLOAD_PORT") or env.GetProjectOption("monitor_port") or "/dev/ttyUSB0"
    baudrate = int(env.GetProjectOption("monitor_speed") or 115200)

    print(f"Resetting device on port {port}...")

    try:
        ser = serial.Serial(port, baudrate, timeout=1)
        ser.setDTR(False)
        ser.setRTS(True)
        time.sleep(0.1)
        ser.setRTS(False)
        ser.setDTR(True)
        ser.close()
        print("Reset complete.")
    except Exception as e:
        print(f"Could not reset device: {e}")
        return

    time.sleep(1)

    print("Starting monitor...")
    subprocess.Popen(["platformio", "device", "monitor"])

# Добавляем пользовательскую цель
env.AddCustomTarget(
    name="reset_monitor",
    dependencies=None,
    actions=[reset_and_monitor],
    title="Reset and Monitor",
    description="Resets the ESP8266 and opens serial monitor"
)