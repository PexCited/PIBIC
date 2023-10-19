import dash
from dash import dcc, html, Input, Output, State
import plotly.graph_objs as go
import paho.mqtt.client as mqtt
import json

# Configurações do MQTT
MQTT_BROKER = "test.mosquitto.org"  # Utilizando "test.mosquitto.org"
MQTT_PORT = 1883

# Tópicos MQTT para as balanças
MQTT_TOPICS = ["data/BALANCA/1", "data/BALANCA/2", "data/BALANCA/3", "data/BALANCA/4"]


# Inicializa a aplicação Dash
app = dash.Dash(__name__,external_stylesheets=['styles.css'])

# Dados das balanças
dados_balancas = [[] for _ in range(len(MQTT_TOPICS))]

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
        for i in range(1, len(MQTT_TOPICS) + 1)
    ], style={'display': 'flex', 'flex-wrap': 'wrap', 'justify-content': 'center', 'margin-top': '20px'}),

    # Popup para mostrar mensagens e campo de entrada para peso de referência
    html.Div(id='popup-container', style={'display': 'none'}, children=[
        html.Div(id='popup-content', style={'border': '1px solid black', 'padding': '10px'}),
        dcc.Input(id='input-peso-referencia', type='number', step=0.01, placeholder='Peso de Referência (g)'),
        html.Button('OK', id='btn-popup-ok')
    ])
])

# Função de callback para receber mensagens MQTT e atualizar os gráficos
def on_message(client, userdata, message):
    global dados_balancas

    # Obtém o índice da balança com base no tópico
    balanca_index = MQTT_TOPICS.index(message.topic)

    # Decodifica a mensagem JSON
    payload = json.loads(message.payload.decode())
    dados_balancas[balanca_index].append(payload["valor"])

    # Atualiza o gráfico correspondente
    atualizar_grafico(balanca_index + 1)

# Função para atualizar o gráfico de uma balança específica
def atualizar_grafico(balanca_numero):
    fig = go.Figure()
    dados = dados_balancas[balanca_numero - 1]
    fig.add_trace(go.Scatter(x=list(range(len(dados))), y=dados, mode='lines+markers', name=f'Balança {balanca_numero}'))

    fig.update_layout(title=f'Dados da Balança {balanca_numero}', xaxis_title='Amostra', yaxis_title='Valor')

    # Atualiza o gráfico no layout da aplicação
    app.layout[f'grafico-linhas-{balanca_numero}'].figure = fig

# Função de callback para iniciar a calibração e abrir o popup
@app.callback(
    Output('popup-container', 'style'),
    Output('popup-content', 'children'),
    Input('btn-iniciar-calibracao', 'n_clicks')
)
def iniciar_calibracao(n_clicks):
    if n_clicks > 0:
        # Enviar comando para iniciar a calibração via MQTT
        client.publish("comando/calibrar", "iniciar")
        # Mensagens para exibir no popup
        mensagens = ["Retire o peso", "Coloque o peso"]
        popup_content = [html.P(mensagem) for mensagem in mensagens]
        # Limpar o contador de cliques para que a função possa ser chamada novamente
        return {'display': 'block'}, popup_content
    else:
        return {'display': 'none'}, []

# Função de callback para iniciar a medição
@app.callback(
    Output('btn-iniciar-medicao', 'n_clicks'),
    Input('btn-iniciar-medicao', 'n_clicks')
)
def iniciar_medicao(n_clicks):
    if n_clicks > 0:
        # Enviar comando para iniciar a medição via MQTT
        client.publish("comando/medir", "iniciar")
        # Limpar o contador de cliques para que a função possa ser chamada novamente
        return 0
    else:
        return 0

if __name__ == '__main__':
    app.run_server(debug=True)
