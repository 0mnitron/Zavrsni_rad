from flask import Flask, request
from tinydb import TinyDB, Query
import json

app = Flask(__name__)

#lista = []
# TinyDB database that stores its data in db.json
db = TinyDB('db.json')

@app.route('/', methods=['GET', 'POST'])
def home():
    data = request.data
    json_data = json.loads(data)
    #lista.append(json_data)
    db.insert(json_data)
    return "OK",200

@app.route('/data', methods=['GET', 'POST'])
def data():
    data = Query()
    #db.search(data.version == '1.0') or db.all()
    return str(lista)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
