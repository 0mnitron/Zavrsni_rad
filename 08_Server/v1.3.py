import json

from flask import Flask, request
import graphyte

graphyte.init('127.0.0.1:2003')

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
            path = f"{in_dict['wsid']}.{key}"
            timestamp = in_dict['time']
            graphyte.send(path,value,timestamp)


@app.route('/', methods=['GET', 'POST'])
def home():
    data = request.data
    json_data = json.loads(data)
    send_dict_to_graphite(json_data)
    return "OK", 200


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
