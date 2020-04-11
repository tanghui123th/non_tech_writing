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