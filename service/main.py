from sheet import Sheet
from random import randint
from mqtt.main import *
from datetime import datetime
from threading import Event

def decode(data):
    return json.loads(data.payload.decode().replace("'", '"'))

class MONITOR:
    def __init__(self, id, host='broker.mqtt.cool', port=1883, asn=True) -> None:
        self._id = id
        self.data_identifier = None     
        self.client = connect_mqtt(client_id=str(randint(100, 300)), broker=host, port=port)
        subscribe(self.client, f"data/BALANCA/{self._id}", self.process)

        subscribe(self.client, f"app/START/{self._id}", self.start)
        subscribe(self.client, f"app/STOP/{self._id}", self.stop)

        self.sheet = Sheet(["Data", "B1", "B2", "B3", "B4"])
        self.running = Event()

        if not asn:
            self.client.loop_forever() # sync
        else:
            self.client.loop_start() # async
        

    def start(self, client, userdata, data ):
        data = decode(data)
        self.data_identifier = data["_id"]
        self.running.set()
    
    def stop(self, client, userdata, data ):
        self.running.clear()
        self.write_on_disk()

    def process(self, client, userdata, data ):
        if self.running.is_set():
            data = decode(data)
            data['Data'] = datetime.now()
            self.sheet.add_data(data)
            return
    
    def write_on_disk(self):
        if not self.running.is_set() and self.data_identifier is not None:
            log_sheet = self.sheet.update_df()
            if log_sheet.size > 0:
                filename = f"IJ{self.data_identifier}_{self.sheet.date}.csv"
                log_sheet.to_csv(f"./log/log_{filename}")
            else:
                print("Nenhum dado para ser salvo")


if __name__ == "__main__":

    from os import mkdir
    from os.path import exists

    for folder in ['log']:
        if not exists(f'./{folder}'):
            mkdir(f'./{folder}')

    mn = MONITOR(1, asn=False)