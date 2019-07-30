# flask+gunicorn+nginx部署网站

## 配置NGINX

nginx配置：

```
server {
    charset utf-8;
    listen 80;
    server_name www.leosirius.com;

    location /static {
        alias /root/mysite/blog_code/flaskblog/static;
    }

    location / {
        proxy_set_header Host $host;
        proxy_pass http://unix:/root/mysite/blog_code/flaskblog.sock;
    }
}
```

使用ssl的nginx配置，注意一定要把阿里云安全组的相关端口打开

```

server {
    charset utf-8;
    listen 443;
    server_name www.leosirius.com;
    ssl on;
    ssl_certificate /etc/nginx/cert/2472165_www.leosirius.com.pem;
    ssl_certificate_key /etc/nginx/cert/2472165_www.leosirius.com.key;

    location /static {
        alias /root/mysite/blog_code/flaskblog/static;
    }

    location / {
        proxy_set_header Host $host;
        proxy_pass http://unix:/root/mysite/blog_code/flaskblog.sock;
    }
}

server{
    listen 80;
    server_name www.leosirius.com;
    rewrite ^(.*)$ https://$host$1 permanent;
}
```



- `location /static` 就是项目静态文件存放的地方，注意不是`templates`文件，是包含css、js和图片的那个`static`目录。
- `location proxy_pass` 后面的那个sock文件是`gunicorn`启动生成的。

还有比较坑的一点，如果`/var/log/nginx/error.log`在转发代理的时候报了permission denied的错误，看看是不是`/etc/nginx/nginx.conf`中的user是不是你启动gunicorn的user

## 配置gunicorn

gunicorn配置：

```
[Unit]
Description=gunicorn daemon
After=network.target

[Service]
User=root
Group=root
WorkingDirectory=/root/mysite/blog_code
ExecStart=/root/mysite/venv/bin/gunicorn --access-logfile - --workers 2 --bind unix:/root/mysite/blog_code/flaskblog.sock run:app

[Install]
WantedBy=multi-user.target
```

`ExecStart=/root/mysite/venv/bin/gunicorn --access-logfile - --workers 2 --bind unix:/root/mysite/blog_code/flaskblog.sock run:app`这条命令就是生成`flaskblog.sock`的命令。后面的run就是项目中创建`app.run()`调用的地方，注意在生产环境下的参数。在django中是一个叫`wsgi.py`的文件。冒号后面是app的名字，我们app的名字就叫app。

启动gunicorn服务

```
systemctl start gunicorn_flaskblog
systemctl enable gunicorn_flaskblog
```

查看服务状态

```
systemctl status gunicorn_flaskblog
```

## 后续维护

在后续的维护中，如果改了gunicorn配置文件，需要两条命令重启。如果只是改了flask项目的内容，只用执行下面的第二条命令即可。

```
systemctl daemon-reload
systemctl restart gunicorn_flaskblog
```

如果改了nginx配置，需要按照下面的方法重启

```
nginx -t
systemctl restart nginx
```

## mysql字符集

mysql如果字符集是utf8，要改成utf8mb4，支持4个字节的字符。

`mysql> ALTER TABLE table_name CONVERT TO CHARACTER SET utf8mb4;`