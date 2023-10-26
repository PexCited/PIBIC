import pandas as pd
from datetime import datetime

class Sheet:
    def __init__(self, sensors):
        self.columns = sensors
        self.clear()
        
    def add_data(self, payload):
        self.memory.append(payload)

    def update_df(self):
        return pd.DataFrame([*self.memory], columns=self.columns)
    
    def clear(self):
        self.memory = []
        self.start_at = datetime.now().strftime('%d-%m-%Y_%H%M%S')

    @property
    def date(self):
        return datetime.now().strftime('%d-%m-%Y_%H%M%S')