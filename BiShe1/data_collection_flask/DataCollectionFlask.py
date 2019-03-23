import os, uuid
from flask import Flask, request, render_template, session, redirect, url_for, flash
from flask_bootstrap import Bootstrap
from flask_moment import Moment
from datetime import datetime
from flask_wtf import FlaskForm
from flask_wtf.file import FileRequired
from wtforms import StringField, FileField, MultipleFileField, SubmitField
from wtforms.validators import DataRequired
from flask_uploads import UploadSet, IMAGES, configure_uploads

basedir = os.path.abspath(os.path.dirname(__file__))

def RandomName(filename):
    suffix = os.path.splitext(filename)[1]
    newname = uuid.uuid4().hex + suffix
    return newname

# 创建Flask应用实例 
app = Flask(__name__)
# 给Flask-WTF配置密钥，以保护用户会话
app.config['SECRET_KEY'] = 'Vanellope & Liberty'
# 配置上传文件保存目录
app.config['UPLOAD_PATH'] = os.path.join(basedir, 'uploads')

# 初始化各extension
bootstrap = Bootstrap(app)
moment = Moment(app)

# 定义表单类
class UploadForm(FlaskForm):
    #photos = MultipleFileField('Select uploaded files', validators=[FileRequired()])
    photo = FileField('Choose File', validators=[FileRequired()])
    submit = SubmitField('Submit')

@app.errorhandler(404)
def page_not_found(e):
    return render_template('404.html'), 404

@app.errorhandler(500)
def internal_server_error(e):
    return render_template('500.html'), 500

@app.route('/', methods=['GET', 'POST'])
def index():
    return render_template('index.html', current_time=datetime.utcnow())

@app.route('/upload', methods=['GET', 'POST'])
def upload():
    form = UploadForm()
    if request.method == 'POST':
        if 'photo' not in request.files:
            flash('No file part')
            return redirect(request.url)
        file = request.files['photo']
        if file.filename == '':
            flash('No selected file')
            return redirect(request.url)
        filename = RandomName(file.filename)
        file.save(os.path.join(app.config['UPLOAD_PATH'], filename))
        flash('Upload done !')
        return redirect(request.url)
    return render_template('upload.html', form=form)
