import dash
from dash import dcc, html, Input, Output, State, callback
import plotly.graph_objs as go

# Inicializar a aplicação Dash
app = dash.Dash(__name__, title='PIBIC', update_title='Carregando...', suppress_callback_exceptions=True,)


# Layout da aplicação
app.layout = html.Div([
    # Gráfico de barras
    dcc.Graph(id='grafico-barras',
              figure={
                  'data': [
                      {'x': ['Balança 1', 'Balança 2', 'Balança 3', 'Balança 4'], 'y': [10, 20, 15, 25], 'type': 'bar', 'name': 'Barras'},
                  ],
                  'layout': {
                      'title': 'Gráfico de Barras'
                  }
              }),

    # Botões Iniciar e Calibrar
    html.Div([
        html.Button('Iniciar', id='btn-iniciar', className='botao'),
        html.Button('Calibrar', id='btn-calibrar', className='botao')
    ])
])

if __name__ == '__main__':
    app.run_server(debug=True)
