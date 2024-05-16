from flask import Flask, render_template, request, send_file
from flask_cors import CORS
import base64
import os
from io import BytesIO
import find_rom
root = os.path.abspath('.')
app = Flask(__name__, template_folder=root, static_folder = root+"/")
CORS(app)

@app.after_request
def set_headers(response):
    response.headers['Cross-Origin-Opener-Policy'] = 'same-origin'
    response.headers['Cross-Origin-Embedder-Policy'] = 'require-corp'
    return response

@app.route('/',methods=["GET","POST"])
def index():
    return render_template('index.html')

@app.route('/process_name',methods=["GET","POST"])
def process_name():
    if request.method == "POST":
        game_name = request.get_json()["text"]
        print(game_name)
        closest_rom = find_rom.proc(game_name)
        with open("../res/roms/"+closest_rom,"rb") as rom:
            file_send = BytesIO(closest_rom.encode('utf-8')+b'\n'+rom.read())
            return send_file(file_send, mimetype='application/octet-stream')
    return ""

if __name__ == "__main__":
    print(root)
    app.run(host='192.168.1.191',ssl_context=('cert.pem','key.pem'),debug=True)