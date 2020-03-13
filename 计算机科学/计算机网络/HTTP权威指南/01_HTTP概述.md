# 1 HTTP概述

- HTTP使用的是可靠的数据传输协议，可以使开发人员专注于应用程序的细节，而不用考虑因特网中存在的一些缺陷和问题。

## 1.3 资源

- Web服务器是Web资源的宿主。Web资源是Web内容的源头

### 媒体类型

MIME（Multipurpose Internet Mail Extension）类型，原本是为了解决电子邮件中的问题，HTTP也使用。用来描述媒体内容。MIME是文本标记，由主要对象类型和子类型组成，如

- text/html
- text/plain
- image/png

### URI

URI包括URL和URN。但是由于URN很少用，所以URI和URL基本上可以互换使用。

- URL：描述了特定服务器上某资源的特定位置
- URN：是资源的特定名称，与位置无关。如无论`RFC 2141`文档位于何处，都可以用`urn:ietf:urf:2141`来命名
