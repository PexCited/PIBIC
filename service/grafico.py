import serial
import sys
import threading

def read_serial():
    while not stop_event.is_set():
        try:
            linha = porta_serial.readline().decode('utf-8')
            print(linha, end='')
        except KeyboardInterrupt:
            break

# Configure a porta serial (substitua 'COMx' pelo nome da porta serial correta)
porta_serial = serial.Serial('COM4', 115200)

# Crie um evento para controlar a leitura
stop_event = threading.Event()

# Crie uma thread para ler os dados da porta serial
thread = threading.Thread(target=read_serial)
thread.start()

print("Pressione 'q' e Enter para parar a leitura")

# Aguarde o usuário pressionar 'q' para parar a leitura
try:
    while True:
        entrada = input()
        if entrada == 'q':
            stop_event.set()
            break
except KeyboardInterrupt:
    pass

# Espere a thread de leitura terminar e feche a porta serial
thread.join()
porta_serial.close()
