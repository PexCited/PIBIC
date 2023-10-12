from sheet import Sheet
from random import randint
from mqtt.main import *
from datetime import datetime

def decode(data):
    return json.loads(data.payload.decode().replace("'", '"'))

class BL:
    def __init__(self, id, host='10.97.241.241', port=1883, asn=True) -> None:
        self._id = id
        self.client = connect_mqtt(client_id=str(randint(100, 300)), broker=host, port=port)
        subscribe(self.client, f"data/bl/{self._id}", self.process)
        subscribe(self.client, f"write/bl/{self._id}", self.write_on_disk)
        self.sheet = Sheet(["Data", "Evento"])

        if not asn:
            self.client.loop_forever() # sync
        else:
            self.client.loop_start() # async

    def process(self, client, userdata, data ):
        data = decode(data)
        data['Balanca'] = self._id
        data['Data'] = datetime.now()
        self.sheet.add_data(data)
        return
    
    def write_on_disk(self):
        log_sheet = self.sheet.update_df()
        if log_sheet.size > 0:
            filename = f"IJ{self._id}_{self.sheet.date}.xlsx"
            # planilha = create_data_table(log_sheet)
            # planilha.to_excel(f"./data/{filename}")
            log_sheet.to_excel(f"./log/log_{filename}")
        else:
            print("Nenhum dado para ser salvo")


bl_1 = BL(1, asn=True)
bl_2 = BL(2, asn=True)
bl_3 = BL(3, asn=True)
bl_4 = BL(4, asn=False)