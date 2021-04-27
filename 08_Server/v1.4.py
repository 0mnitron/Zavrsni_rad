import json
import os

from flask import Flask, request

app = Flask(__name__)

test_data = []

def rijeka():
    import requests
    api_url = "http://api.openweathermap.org/data/2.5/weather?q=Rijeka&appid=6f8f89bb25d8bac2f740ea5581f676c0&units=metric"
    r = requests.get(api_url)
    str_rijeka = {'time': '-1', 'wsid':'hr.51000.0', 'temp': r.json()['main']['temp'], 'pressure': r.json()['main']['pressure'], 'humidity': r.json()['main']['humidity'], 'wind':r.json()['wind']['speed'], 'visibility': r.json()['visibility']}
    print(str_rijeka)
    send_dict_to_graphite(str_rijeka)


def send_dict_to_graphite(in_dict):
    """
    Converts dictionary in_dict to list of graphite textfile commands,
    and sends it to graphite host using graphyte library.

        Parameters:
            in_dict (dict): A dictionary containing single stations data
    """
    for key,value in in_dict.items():
        if key != 'wsid' and key != 'time':
            metric = f"{in_dict['wsid']}.{key}"
            timestamp = int(in_dict['time'])
            print(metric, value, timestamp)
            cmd = f'echo "{metric} {value} -1" | nc  -q0 65.21.1.49 2003'
            print(cmd)
            os.system(cmd)


@app.route('/', methods=['GET', 'POST'])
def home():
    data = request.data
    json_data = json.loads(data)

    test_data.append(json_data)
    
    send_dict_to_graphite(json_data)
    rijeka()
    return "OK", 200

###
@app.route('/data', methods=['GET', 'POST'])
def data():
    return str(test_data)
###

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
