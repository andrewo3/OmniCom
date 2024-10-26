from flask import Flask, render_template, request, redirect, send_file
from flask_cors import CORS
import os
from io import BytesIO
import find_rom
root = os.path.abspath('.')
application = Flask(__name__)
CORS(application)

@application.after_request
def set_headers(response):
    response.headers['Cross-Origin-Opener-Policy'] = 'same-origin'
    response.headers['Cross-Origin-Embedder-Policy'] = 'require-corp'
    return response

@application.route('/',methods=["GET","POST"])
def index():
    return render_template('index.html')

@application.route('/process_name',methods=["POST"])
def process_name():
    if request.method == "POST":
        game_name = request.get_json()["text"]
        print(game_name)
        closest_rom = find_rom.proc(game_name)[-1]
        with open("roms/"+closest_rom,"rb") as rom:
            file_send = BytesIO(closest_rom.encode('utf-8')+b'\n'+rom.read())
            return send_file(file_send, mimetype='application/octet-stream')
    return ""

@application.route("/get_rom",methods=["POST"])
def get_rom():
    #same as process_name but it skips the guessing part
    if request.method == "POST":
        game_name = request.get_json()["text"]
        with open("roms/"+game_name,"rb") as rom:
            file_send = BytesIO(game_name.encode('utf-8')+b'\n'+rom.read())
            return send_file(file_send, mimetype='application/octet-stream')

@application.route('/matches',methods=["POST"])
def matches():
    name = request.get_json()["text"]
    print(name)
    sorted_list = find_rom.proc(name)[::-1]
    return sorted_list

if __name__ == "__main__":
    print(root)
    application.run(host='0.0.0.0',ssl_context=('cert/cert.pem','cert/key.pem'))
    #application.run()