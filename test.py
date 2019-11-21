import threading
import datetime
import time

class MyThread(threading.Thread):
    def run(self):
        time.sleep(2)
        

if __name__ == '__main__':
    print('=== time = {}'.format(datetime.datetime.now()))
    t_dict = {}
    for i in range(1000):
        t_dict[str(i)] = MyThread()
        t_dict[str(i)].start()
        
    for i in range(1000):
        t_dict[str(i)].join()
    print('=== time = {}'.format(datetime.datetime.now()))