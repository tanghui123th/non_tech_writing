# 17 Concurrency with Futures

## 17.1 Example: Web Downloads in Three Styles

这章和下章中，分别用`flags.py``flags_threadpool.py``flags_asyncio.py`三种方式来实现下载。后两种时间其实差不多，但都比第一种少很多。

### A Sequential Download Script

```python
import os
import time
import sys

import requests

POP20_CC = ('CN IN US ID BR PK NG BG RU JP '
            'MX PH VN ET EG DE IR TR CD FR').split()

BASE_URL = 'http://flupy.org/data/flags'

DEST_DIR = 'downloads/'


def save_flag(img, filename):
    path = os.path.join(DEST_DIR, filename)
    with open(path, 'wb') as fp:
        fp.write(img)


def get_flag(cc):
    url = '{}/{cc}/{cc}.gif'.format(BASE_URL, cc=cc.lower())
    print('url = ' + url)
    resp = requests.get(url)
    return resp.content


def show(text):
    print(text, end=' ')
    sys.stdout.flush()


def download_many(cc_list):
    for cc in sorted(cc_list):
        image = get_flag(cc)
        show(cc)
        save_flag(image, cc.lower() + '.gif')

    return len(cc_list)


def main(download_many):
    t0 = time.time()
    count = download_many(POP20_CC)
    elapsed = time.time() - t0
    msg = '\n{} flags downloaded in {:.2f}s'
    print(msg.format(count, elapsed))


if __name__ == '__main__':
    main(download_many)
```

我本地测试的结果如下

```bash
BG BR CD CN DE EG ET FR ID IN IR JP MX NG PH PK RU TR US VN
20 flags downloaded in 15.81s
```

### Downloading with concurrent.futures

concurrent.futures提供以下两个类，实现了多线程和多进程的高级抽象。

- ThreadPoolExecutor
- ProcessPoolExecutor

executor.map方法类似内建的map，不过是并发执行的

```python
from concurrent import futures

from flags import save_flag, get_flag, show, main

MAX_WORKERS = 20


def download_one(cc):
    image = get_flag(cc)
    show(cc)
    save_flag(image, cc.lower() + '.gif')
    return cc


def download_many(cc_list):
    workers = min(MAX_WORKERS, len(cc_list))
    with futures.ThreadPoolExecutor(workers) as executor:
        res = executor.map(download_one, sorted(cc_list))

    return len(list(res))


if __name__ == '__main__':
    main(download_many)
```

我本地测试的结果如下

```bash
BR FR JP NG ID BG IN CN EG DE IR PH VN RU PK MX TR ET US CD
20 flags downloaded in 8.17s
```

download_one就是第一个版本中的一个for loop。把一个for loop写成一个函数，再并发调用这个函数。这是重构代码实现并发的技巧。
