import json
from collections import deque
from random import randint
import dash
import plotly.graph_objs as go
from dash import dcc, html, Input, Output, State
from mqtt.main import *
dataB1 = deque(maxlen=3)

QTD_BALANCAS = 4

PAYLOAD_PER_SEC = 100
SECONDS2SHOW = 10
GRAF_LEN = PAYLOAD_PER_SEC * SECONDS2SHOW

DATAFrames = [deque(maxlen=GRAF_LEN) for _ in range(QTD_BALANCAS)]
# Configurações do MQTT
MQTT_BROKER = "test.mosquitto.org"  # Utilizando "test.mosquitto.org"
MQTT_PORT = 1883
ID = client_id=str(randint(100, 300))
client = connect_mqtt(client_id=ID, broker=MQTT_BROKER, port=1883)

# Inicializa a aplicação Dash
app = dash.Dash(__name__, external_stylesheets=['styles.css'])

# Layout da aplicação
app.layout = html.Div([
    # Título "Dados" com estilo de fonte
    html.H1('Dados das Balanças', style={'text-align': 'center', 'margin-top': '20px', 'font-family': 'Bebas Neue'}),

    # Container centralizado com largura máxima e alinhamento ao centro
    html.Div(className='center-container', children=[
        # Adicione uma div para os botões com uma classe CSS para estilização
        html.Div([
            # Botão para iniciar medição com estilo de botão
            html.Button('Iniciar Medição', id='btn-iniciar-medicao', n_clicks=0, style={'margin-bottom': '10px', 'background-color': 'darkgreen', 'color': 'white'}),
            # Botão para iniciar a calibração com estilo de botão
            html.Button('Calibrar', id='btn-iniciar-calibracao', n_clicks=0, style={'background-color': 'darkgreen', 'color': 'white'}),
        ], className='button-container'),
    ]),

    # Gráficos de Linhas representando as balanças
    html.Div([
        dcc.Graph(id=f'grafico-linhas-{i}', config={'displayModeBar': False})
        for i in range(QTD_BALANCAS)
    ], style={'display': 'flex', 'flex-wrap': 'wrap', 'justify-content': 'center', 'margin-top': '20px'}),

    # Popup para mostrar mensagens e campo de entrada para peso de referência
    html.Div(id='popup-container', style={'display': 'none'}, children=[
        html.Div(id='popup-content', style={'border': '1px solid black', 'padding': '10px'}),
        dcc.Input(id='input-peso-referencia', type='number', step=0.01, placeholder='Peso de Referência (g)'),
        html.Button('OK', id='btn-popup-ok')
    ]),

    # Adicione este componente para controlar a atualização dos gráficos
    dcc.Interval(id='interval-component', interval=PAYLOAD_PER_SEC, n_intervals=0)
])

# Função de callback para receber mensagens MQTT e atualizar os gráficos
def on_message(client, userdata, message):
    global dados_balancas

    # Decodifica a mensagem JSON
    payload = json.loads(message.payload.decode())

    for i in range(QTD_BALANCAS):
        DATAFrames[i].append(payload.get(f"B{i+1}", -1))

# Função para atualizar o gráfico de uma balança específica
@app.callback(
    *[Output(f'grafico-linhas-{i}', 'figure') for i in range(QTD_BALANCAS)],
    Input('interval-component', 'n_intervals'),
)
def atualizar_grafico(n):
    OUTPUT = [go.Figure() for _ in range(QTD_BALANCAS)]

    for index in range(QTD_BALANCAS):
        OUTPUT[index].add_trace(go.Scatter(x=list(range(len(DATAFrames[index]))), y=list(DATAFrames[index]), mode='lines+markers', name=f'Balança {index+1}'))
        OUTPUT[index].update_layout(title=f'Dados da Balança {index+1}', xaxis_title='Amostra', yaxis_title='Valor')

    return OUTPUT

# Função de callback para iniciar a calibração e abrir o popup
@app.callback(
    Output('popup-container', 'style'),
    Output('popup-content', 'children'),
    Input('btn-iniciar-calibracao', 'n_clicks')
)
def iniciar_calibracao(n_clicks):
    if n_clicks > 0:
        # Enviar comando para iniciar a calibração via MQTT
        client.publish("config/A", "iniciar")
        # Mensagens para exibir no popup
        mensagens = ["Retire o peso", "Coloque o peso"]
        popup_content = [html.P(mensagem) for mensagem in mensagens]
        # Limpar o contador de cliques para que a função possa ser chamada novamente
        return {'display': 'block'}, popup_content
    return {'display': 'none'}, []

# Função de callback para iniciar a medição
@app.callback(
    Output('btn-iniciar-medicao', 'n_clicks'),
    Input('btn-iniciar-medicao', 'n_clicks')
)
def iniciar_medicao(n_clicks):
    if n_clicks > 0:
        # Enviar comando para iniciar a medição via MQTT
        client.publish("medir/A", "iniciar")
        # Limpar o contador de cliques para que a função possa ser chamada novamente
    return 0



if __name__ == '__main__':
    subscribe(client, "data/BALANCA/1", on_message)
    client.loop_start()
    #client.loop_forever()
    app.run(debug=True)