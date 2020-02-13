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

download_one就是第一个版本中的一个for loop。把一个for loop写成一个函数，再并发调用这个函数。这是实现并发时重构代码的技巧。

### Where Are the Futures?

标准库中有两个Future类：

- concurrent.futures.Future
- asyncio.Future

它们都会推迟计算，类似js中的Promise。但注意，我们一般不直接操作者两个类，而是使用concurrent和asyncio两个库

下面的例子中，我们把前面用map实现的异步操作用future类来实现

```python
def download_many(cc_list):
    cc_list = cc_list[:5]
    with futures.ThreadPoolExecutor(max_workers=3) as executor:
        to_do = []
        # 第一个for循环，先把所有要执行的download_one方法submit。submit返回future，存在to_do中
        for cc in sorted(cc_list):
            future = executor.submit(download_one, cc)
            to_do.append(future)
            msg = 'Scheduled for {}: {}'
            print(msg.format(cc, future))

        results = []
        for future in futures.as_completed(to_do):  # as_completed yields futures as their are completed
            res = future.result()    # 如果future没有complete，concurrent中的result会阻塞
            msg = '{} result: {!r}'  # !r调用对象的repr
            print(msg.format(future, res))
            results.append(res)

    return len(results)
```

我们开了3个workers，可以看到先下载了3个，然后有下载好了2个。提交和执行完顺序是不同的。但是可以注意到，最先提交的三个仍然是最先完成的3个，这是符合逻辑的。

```bash
Scheduled for BR: <Future at 0x1051fe9d0 state=running>
Scheduled for CN: <Future at 0x105206890 state=running>
Scheduled for ID: <Future at 0x105213050 state=running>
Scheduled for IN: <Future at 0x105213810 state=pending>
Scheduled for US: <Future at 0x1052138d0 state=pending>
CN ID BR <Future at 0x105206890 state=finished returned str> result: 'CN'
<Future at 0x105213050 state=finished returned str> result: 'ID'
<Future at 0x1051fe9d0 state=finished returned str> result: 'BR'
IN <Future at 0x105213810 state=finished returned str> result: 'IN'
US <Future at 0x1052138d0 state=finished returned str> result: 'US'

5 flags downloaded in 1.74s
```

## 17.2 Blocking I/O and the GIL

注意由于GIL的存在，python中concurrent和asyncio都不能并行。多线程的意义就在于：如果遇到了IO阻塞，则切换到其他线程。


