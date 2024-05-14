from flask import Flask, render_template
from flask_cors import CORS
import os
root = os.path.abspath('.')
app = Flask(__name__, template_folder=root, static_folder = root+"/")
CORS(app)

@app.after_request
def set_headers(response):
    response.headers['Cross-Origin-Opener-Policy'] = 'same-origin'
    response.headers['Cross-Origin-Embedder-Policy'] = 'require-corp'
    return response

@app.route('/')
def index():
    return render_template('home.html')

if __name__ == "__main__":
    print(root)
    app.run(host='192.168.1.191',ssl_context=('cert.pem','key.pem'),debug=True)