import json
import os

from flask import Flask, request

app = Flask(__name__)


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
            cmd = f'echo "{metric} {value} -1" | nc -q0 65.21.1.49 2003'
            os.system(cmd)


@app.route('/', methods=['GET', 'POST'])
def home():
    data = request.data
    json_data = json.loads(data)
    send_dict_to_graphite(json_data)
    return "OK", 200


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
