from flask import Flask, request
import json
app = Flask(__name__)

lista = []

@app.route('/', methods=['GET', 'POST'])
def home():
    data = request.data
    idk = json.loads(data)
    lista.append(idk)
    return "OK",200

@app.route('/data', methods=['GET', 'POST'])
def data():
    return str(lista)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
