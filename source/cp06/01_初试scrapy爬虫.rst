初试scrapy爬虫
==============

直接上手
--------

scrapy框架理论性的东西就不在这里记录了，网上一大堆。另外我使用virtualenv做虚拟环境。

目标：爬取豆瓣top250信息。

安装scrapy，创建项目\ ``douban``\ ，创建爬虫名为\ ``douban_spider``

::

   pip install scrapy -i https://pypi.douban.com/simple/
   scrapy startproject douban
   scrapy crawl douban_spider

自定义配置
----------

在配置中有几处改动

.. code:: python

   ROBOTSTXT_OBEY = False
   ...
   # 使用一个真实的user_agent，不然验证通不过。
   USER_AGENT = 'User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.142 Safari/537.36'
   ...
   # 每次请求休息0.5秒
   DOWNLOAD_DELAY = 0.5

数据模型
--------

在item.py里加上我们的数据信息

.. code:: python

   import scrapy


   class DoubanItem(scrapy.Item):
       serial_number = scrapy.Field()  # 序号
       movie_name = scrapy.Field()     # 名字
       introduce = scrapy.Field()      # 简介
       star = scrapy.Field()           # 星级
       evalute = scrapy.Field()        # 评论数
       describe = scrapy.Field()       # 描述

初试爬虫
--------

然后在\ ``douban_spider.py``\ 中写入的我们基本爬虫逻辑

.. code:: python

   # -*- coding: utf-8 -*-
   import scrapy


   class DoubanSpiderSpider(scrapy.Spider):
       name = 'douban_spider'                             # 爬虫名
       allowed_domains = ['movie.douban.com']             # 允许的url
       start_urls = ['https://movie.douban.com/top250']   # 入口url

       def parse(self, response):
           print(response.text)        # 打印数据

开始爬，就可以看到爬下来的信息被打印出来。

::

   scrapy crawl douban_spider

编写spider
----------

我们来正式编写spider的逻辑

.. code:: python

   # -*- coding: utf-8 -*-
   import scrapy
   from douban.items import DoubanItem

   class DoubanSpiderSpider(scrapy.Spider):
       name = 'douban_spider'                             # 爬虫名
       allowed_domains = ['movie.douban.com']             # 允许的url
       start_urls = ['https://movie.douban.com/top250']   # 入口url

       def parse(self, response):
           movie_list = response.xpath("//div[@class='article']//ol[@class='grid_view']/li")
           for item in movie_list:
               douban_item = DoubanItem()
               douban_item['serial_number'] = item.xpath(".//div[@class='item']//em/text()").extract_first()
               douban_item['movie_name'] = item.xpath(".//div[@class='info']/div[@class='hd']/a/span[1]/text()").extract_first()
               intro_content = item.xpath(".//div[@class='info']/div[@class='bd']/p[1]/text()").extract()
               # 因为intro有两行内容，这里处理一下
               intro = ''
               for line in intro_content:
                   line = ''.join(line.split()) # 处理空格
                   intro += ' ' + line
               douban_item['introduce'] = intro
               douban_item['star'] = item.xpath(".//span[@class='rating_num']/text()").extract_first()
               douban_item['evalute'] = item.xpath(".//div[@class='star']/span[4]//text()").extract_first()
               douban_item['describe'] = item.xpath(".//p/span/text()").extract_first()

               # 把一页的数据yield到Pipline的douban_item中
               yield douban_item

           # 如果有下一页，则加入到请求队列中
           next_url = response.xpath(".//span[@class='next']/link/@href").extract()
           if next_url:
               next_url = next_url[0]
               yield scrapy.Request("https://movie.douban.com/top250" + next_url, callback=self.parse)

保存数据
--------

我们可以以json或csv的格式输出数据

::

   scrapy crawl douban_spider -o output.json
   scrapy crawl douban_spider -o output.csv

我们再把数据保存在mongdoDB中，首先安装pymongo

::

   pip install pymongo

配置中开启piplines的配置，然后加入数据库的配置

.. code:: python

   ITEM_PIPELINES = {
      'douban.pipelines.DoubanPipeline': 300,
   }
   ...
   mongo_host = '127.0.0.1'
   mongo_port = 27017
   mongo_db_name = 'douban'                # 数据库名
   mongo_db_collection = 'douban_movie'    # 数据表名

编写\ ``piplines.py``

.. code:: python

   import pymongo
   from douban.settings import mongo_host, mongo_port, mongo_db_name, mongo_db_collection

   class DoubanPipeline(object):
       def __init__(self):
           host = mongo_host
           port = mongo_port
           dbname = mongo_db_name
           sheetname = mongo_db_collection

           client = pymongo.MongoClient(host=host, port=port)
           mydb = client[dbname]
           self.post = mydb[sheetname]

       def process_item(self, item, spider):
           data = dict(item)      # 这个item就是douban_spider中yield的douban_item
           self.post.insert(data)
           return item

爬虫伪装
--------

两种伪装方式：

-  代理ip
-  随机user-agent
