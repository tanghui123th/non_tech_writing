# 18 Concurrency with asyncio

asyncio: a package that implements concurrency with coroutines driven by an event loop.

## 18.1 Thread Versus Coroutine: A Comparison

### thread version

```python
import threading
import itertools
import time
import sys


class Signal:  # 使用go来从外部给线程内发送消息
    go = True


def spin(msg, signal):
    write, flush = sys.stdout.write, sys.stdout.flush
    for char in itertools.cycle('|/-\\'):
        status = char + ' ' + msg
        write(status)
        flush()
        write('\x08' * len(status))
        time.sleep(1)
        if not signal.go:
            break

    write(' ' * len(status) + '\x08' * len(status))


def slow_function():
    # pretend waiting a long time for IO
    time.sleep(3)
    return 42


def supervisor():
    signal = Signal()

    # spin函数用另一个线程来跑
    spinner = threading.Thread(target=spin, args=('thinking!', signal))
    print('spinner object:', spinner)  # spinner是一个Thread对象

    spinner.start()
    result = slow_function()
    signal.go = False

    spinner.join()  # 主进程在这里等待spinner进程结束。

    return result


def main():
    result = supervisor()
    print('Answer:', result)


if __name__ == '__main__':
    main()
```

输出结果是`| thinking!`，前面是一个spinner，3秒后被清掉。注意sleep会block主进程，但是spinner进程不会被block，照样执行，所以我们能看到旋转的spinner。

```bash
leo@localhost py_test $ python3 spinner_thread.py 
spinner object: <Thread(Thread-1, initial)>
Answer: 42
```

注意python没有关闭线程的api，所以这里用了一个signal.go来让线程自己结束。

### asyncio version

注意这里的语法和书上的有些出入，主要是`@asyncio.coroutine`换成了`async`，`yield from`换成了`await`。`create_task()`会创建一个Task并run

```python
import asyncio
import itertools
import sys


async def spin(msg):
    write, flush = sys.stdout.write, sys.stdout.flush
    for char in itertools.cycle('|/-\\'):
        status = char + ' ' + msg
        write(status)
        flush()
        write('\x08' * len(status))
        try:
            await asyncio.sleep(.1)
        except asyncio.CancelledError:
            break
    write(' ' * len(status) + '\x08' * len(status))



async def slow_function():
    await asyncio.sleep(3)
    return 42


async def supervisor():
    spinner = asyncio.create_task(spin('thinking!'))
    print('spinner object:', spinner)
    result = await slow_function()
    spinner.cancel()
    return result

def main():
    loop = asyncio.get_event_loop()
    result = loop.run_until_complete(supervisor())
    loop.close()
    print('Answer:', result)


if __name__ == '__main__':
    main()
```

结果

```bash
spinner object: <Task pending coro=<spin() running at spinner_asyncio.py:6>>
Answer: 42 
```
