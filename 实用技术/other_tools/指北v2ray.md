## 安装v2ray

安装方法有很多，这里就直接用官方提供的脚本：

```
bash <(curl -L -s https://install.direct/go.sh)
```

脚本会自动安装这些东西：

/usr/bin/v2ray/v2ray V2Ray 程序
/usr/bin/v2ray/v2ctl V2Ray 工具
/etc/v2ray/config.json 配置文件
/usr/bin/v2ray/geoip.dat IP 数据文件
/usr/bin/v2ray/geosite.dat 域名数据文件
/etc/systemd/system/v2ray.service Systemd Service
/etc/init.d/v2ray SysV 启动脚本

## 配置 V2Ray 服务端

虽然严格来说 V2Ray 并不分客户端和服务端……反正就是那个意思啦。

编辑配置文件 /etc/v2ray/config.json：

```
{
  "inbounds": [{
    "port": 10086,
    // 因为还要用 Nginx 反代，这里直接监听本地就行
    "listen": "127.0.0.1",
    "protocol": "vmess",
    "settings": {
      "clients": [
        {
          // 用户 UUID，自己随机弄一个
          "id": "23ad6b10-8d1a-40f7-8ad0-e3e35cd38297",
          "level": 1,
          "alterId": 64
        }
      ]
    },
    "streamSettings": {
      // 指定底层传输方式为 WebSocket
      "network": "ws",
      "wsSettings": {
        // 在哪个路径上提供 WS 服务，可自定义
        "path": "/whatever"
      }
    }
  }],
  "outbounds": [{
    "protocol": "freedom",
    "settings": {}
  },{
    "protocol": "blackhole",
    "settings": {},
    "tag": "blocked"
  }],
  "routing": {
    "rules": [
      {
        // 默认规则，禁止访问服务器内网
        "type": "field",
        "ip": ["geoip:private"],
        "outboundTag": "blocked"
      }
    ]
  }
}
```

## 运行v2ray

配置完了，运行一下：

```
systemctl start v2ray
```

如果你的服务器不用 Systemd：

```
service v2ray start
# 要么
/etc/init.d/v2ray start
# 或者手动运行
/usr/bin/v2ray/v2ray -config /etc/v2ray/config.json
```

测试一下有没有跑起来：(出现 400 Bad Request 就对了。)

```
curl -i http://127.0.0.1:10086/whatever
HTTP/1.1 400 Bad Request
Content-Type: text/plain; charset=utf-8
Sec-Websocket-Version: 13
X-Content-Type-Options: nosniff
Date: Sun, 12 Jan 2020 11:45:14 GMT
Content-Length: 12

Bad Request
```

## 配置nginx

```
server {
    listen 443 ssl http2;
    server_name example.com;
    index index.html index.htm index.php;
    root /data/wwwroot/example;

    ssl_certificate /etc/letsencrypt/live/example.com/fullchain.pem;
    ssl_certificate_key /etc/letsencrypt/live/example.com/privkey.pem;
    ssl_session_cache shared:SSL:10m;
    ssl_session_timeout 10m;
    ssl_protocols TLSv1.2 TLSv1.3;
    ssl_prefer_server_ciphers on;
    ssl_ciphers EECDH+AESGCM:EDH+AESGCM;

    location ~ [^/]\.php(/|$) {
        fastcgi_pass unix:/dev/shm/php-cgi.sock;
        fastcgi_index index.php;
        include fastcgi.conf;
    }

    location /whatever {
        proxy_redirect off;
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "upgrade";
        proxy_set_header Host $http_host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;

        set $is_v2ray 0;
        if ($http_upgrade = "websocket") {
            set $is_v2ray 1;
        }

        if ($is_v2ray = 1) {
            # 仅当请求为 WebSocket 时才反代到 V2Ray
            proxy_pass http://127.0.0.1:10086;
        }

        if ($is_v2ray = 0) {
            # 否则显示正常网页
            rewrite ^/(.*)$ /mask-page last;
        }
    }
}