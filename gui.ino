import tkinter as tk
import paho.mqtt.client as mqtt

mqtt_server = "your_mqtt_server"  # Update with your MQTT broker address
mqtt_topic = "Data Receiver"

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT broker")
        client.subscribe(mqtt_topic)
    else:
        print("Connection to MQTT broker failed")

def on_message(client, userdata, msg):
    message = msg.payload.decode("utf-8")
    voltage, current = parse_message(message)
    update_gui(voltage, current)

def parse_message(message):
    parts = message.split(',')
    if len(parts) == 2:
        voltage_str, current_str = parts
        try:
            voltage = float(voltage_str)
            current = float(current_str)
            return voltage, current
        except ValueError:
            pass
    return None, None

def update_gui(voltage, current):
    if voltage is not None and current is not None:
        voltage_label.config(text=f"Voltage: {voltage:.2f} V")
        current_label.config(text=f"Current: {current:.2f} mA")

root = tk.Tk()
root.title("Arduino Data Receiver")

voltage_label = tk.Label(root, text="Voltage: -.- V")
voltage_label.pack()

current_label = tk.Label(root, text="Current: -.- mA")
current_label.pack()

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(mqtt_server, 1883, 60)

client.loop_start()

root.mainloop()
