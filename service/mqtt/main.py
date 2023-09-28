import argparse
from paho.mqtt import client as mqtt_client
import time
import json

def connect_mqtt(client_id, broker, port, callback_c=False, callback_dc=False) -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print(f"{client_id} Connected to MQTT Broker!")
        else:
            print(f"Failed to connect, return code {rc}")
    def on_disconnect(client, userdata, rc):
        print(
            f"{client_id} Disconnected to MQTT Broker!"
        )

    client = mqtt_client.Client(client_id)
    client.on_connect = callback_c or on_connect
    client.on_disconnect = callback_dc or on_disconnect
    client.connect(broker, port)
    return client

def subscribe(client: mqtt_client, topic, callback=False):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")

    client.subscribe(topic)
    client.message_callback_add(topic, callback)

def unsubscribe(client: mqtt_client, topic):
    client.unsubscribe(topic)

def publish(client, topic, payload):
    status, _ = client.publish(topic, payload)
    if status == mqtt_client.MQTT_ERR_SUCCESS:
        print(f"Sent `{payload}` to topic `{topic}`")
    else:
        print(f"Failed to send message to topic {topic}")