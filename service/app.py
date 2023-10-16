import dash
from dash import dcc, html, Input, Output
import plotly.graph_objs as go
import random

# Inicializar a aplicação Dash
app = dash.Dash(__name__)

# Layout da aplicação
app.layout = html.Div([
    # Título "Dados"
    html.H1('Dados', style={'text-align': 'center', 'margin-top': '20px'}),
    # Botões Iniciar e Calibrar
    html.Div([
        html.Button('Iniciar', id='btn-iniciar', className='botao-verde'),
        html.Button('Calibrar', id='btn-calibrar', className='botao-amarelo')
    ], style={'display': 'flex', 'justify-content': 'center', 'margin-top': '20px'}),
    # Gráfico de Barras representando as balanças
    html.Div([
        dcc.Graph(
            id='grafico-barras',
            config={'displayModeBar': False},
            figure={
                'data': [
                    {'x': ['Balança 1', 'Balança 2', 'Balança 3', 'Balança 4'], 'y': [random.randint(1, 100) for _ in range(4)], 'type': 'bar', 'name': 'Balanças'}
                ],
                'layout': {
                    'title': 'Balanças',
                    'width': 500,
                    'height': 400
                }
            }
        )
    ], style={'display': 'flex', 'justify-content': 'center', 'margin-top': '20px'}),

    # Intervalo de atualização
    dcc.Interval(
        id='interval-component',
        interval=10*1000,  # Atualizar a cada 10 segundos (10*1000 milissegundos)
        n_intervals=0
    )
])

# Callbacks para atualização do gráfico de barras (simulação de dados aleatórios)
@app.callback(
    Output('grafico-barras', 'figure'),
    [Input('interval-component', 'n_intervals')]
)
def atualizar_grafico(n_intervals):
    # Atualize os dados do gráfico a cada intervalo
    dados_aleatorios = [random.randint(1, 100) for _ in range(4)]
    figura = go.Figure(data=[go.Bar(x=['Balança 1', 'Balança 2', 'Balança 3', 'Balança 4'], y=dados_aleatorios)])
    figura.update_layout(title='Balanças', width=1200, height=600)

    return figura

# Função para atualizar o gráfico com base nos dados recebidos
def atualizar_grafico_com_dados_recebidos(dados):
    figura = go.Figure(data=[go.Bar(x=['Balança 1', 'Balança 2', 'Balança 3', 'Balança 4'], y=dados)])
    figura.update_layout(title='Balanças', width=1200, height=600)

    return figura

if __name__ == '__main__':
    app.run_server(debug=True, host="0.0.0.0")
