# 10 输入输出

## 10.1 输入和输出

输入流对象：

- cin：与标准输入设备相连

输出流对象：

- cout：与标准输出设备相连
- cerr：与标准错误输出设备相连
- clog：与标准错误输出设备相连

在缺省情况下：cout、cerr、clog都会被打在屏幕上

```cpp
#include<iostream>
using namespace std;

int main(){
    int x, y;
    cin >> x >> y;
    freopen("test.txt", "w", stdout);    // 将标准输出重定向到test.txt文件
    if(y == 0){
        cerr << "error" << endl;    // 标准错误输出，继续输出到屏幕
    }else{
        cout << x / y;
    }
    return 0;
}
```

文件t.txt：
```
3.14 123
```

```cpp
#include<iostream>
using namespace std;

int main(){
    double f;
    int n;
    freopen("t.txt", "r", stdin);   // cin改为从t.txt中读取数据
    cin >> f >> n;
    cout << f << ", " << n << endl; // 输出 3.14 123
    return 0;
}
```

## 10.2 文件读写

### 创建文件

创建文件的一个列子

```cpp
#include<fstream>
using namespace std;

ofstream outFile("clients.dat", ios::out|ios::binary);
```

- clients.dat：要创建的文件的名字
- ios::out：文件打开方式（out会覆盖原有内容，app则是在尾部追加）
- ios::binary：以二进制文件格式打开文件

out和binary之间用了一个位或，实际上是两个位标志。

也可以先创建ofstream对象，再用open打开

```cpp
ofstream fout;
fout.open("test.out", ios::out|ios::binary);
```

判断打开文件是否成功：

```cpp
if(!fout){
    cout << "File open error!" << endl;
}
```

### 文件读写指针

这里的指针并不是指针类型

- 对于输入文件，有一个读指针
- 对于输出文件，有一个写指针
- 对于输入输出文件，有一个读写指针

指针标识文件操作的当前位置，该指针在哪里，读写操作就在哪里进行

### 字符文件（文本文件）读写

文件流也是流，所以可以作用于流的成员函数和流操作算子。我们写一个程序从一个文件中读入一串整数，排序后输出到另一个文件

in.txt的内容为

```
1 234 9 45 6 879
```

执行本程序后，生成的out.txt内容为

```
1 6 9 45 234 879 
```

```cpp
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
using namespace std;

int main(){
    vector<int> v;
    ifstream src_file("in.txt", ios::in);
    ofstream dest_file("out.txt", ios::out);
    int x;
    while(src_file >> x){
        v.push_back(x);
    }
    sort(v.begin(), v.end());
    for(int i = 0; i < v.size(); ++i){
        cout << v[i] << endl;
        dest_file << v[i] << " ";
    }
    dest_file.close();
    src_file.close();
    return 0;
}
```

### 二进制文件读写



- 二进制读文件
  ifstream和fstream的成员函数：将文件读指针指向的地方的n个字节的内容，读到内存地址s，然后将文件读指针向后移动n字节。（以ios::in方式打开文件时，文件读指针指向文件开头）

```cpp
istream &read(char *s. long n);
```

- 二进制写文件
  ofstream和fstream的成员函数：将内存地址s处的n个字节的内容，写入到文件中写指针指向的位置，然后将文件写指针向后移动n字节（以ios::out方式打开文件时，写指针在文件开头，以ios::app方式打开文件时，写指针指向文件尾部）

```cpp
istream& write(const char *s, long n);
```

在文件中写入和读取一个整数

```cpp
#include<iostream>
#include<fstream>
using namespace std;

int main(){
    ofstream fout("some.dat", ios::out | ios::binary);
    int x = 120;
    fout.write((const char *)(&x), sizeof(int));    // &x的类型是int*，这里强制转换
    fout.close();
    ifstream fin("some.dat", ios::in | ios::binary);
    int y;
    fin.read((char *) &y, sizeof(int));   // 同样类型转换
    fin.close();
    cout << y << endl;
    return 0;
}
```

从键盘输入几个学生的姓名和成绩，并以二进制文件形式保存。我们输入：

```
Tom 22
Jaco 23
Mike 34
```

```cpp
#include<iostream>
#include<fstream>
using namespace std;

struct Student{
    char name[20];
    int score;
};

int main(){
    Student s;
    ofstream OutFile("./students.dat", ios::out | ios::binary);
    while(cin >> s.name >> s.score){
        OutFile.write((char*) &s, sizeof(s));
    }
    OutFile.close();
    return 0;
}
```

将student.dat文件的内容读出并显示

```cpp
#include<iostream>
#include<fstream>
using namespace std;

struct Student{
    char name[20];
    int score;
};

int main(){
    Student s;
    ifstream inFile("students.dat", ios::in | ios::binary);
    if(!inFile){
        cout << "error" << endl;
        return 0;
    }
    while(inFile.read((char*) &s, sizeof(s))){
        int readed_bytes = inFile.gcount(); //  查看刚才一次读取了多少字节，这里是24
        cout << s.name << " " << s.score << endl;
    }
    inFile.close();
    return 0;
}
```

同时进行读写操作，把students.dat中Mike的名字改成Jane

```cpp
#include<iostream>
#include<fstream>
using namespace std;

struct Student{
    char name[20];
    int score;
};

int main(){
    Student s;
    fstream iofile("students.dat", ios::in | ios::out | ios::binary);
    if(!iofile){
        cout << "Error";
        return 0;
    }
    iofile.seekp(2 * sizeof(s), ios::beg);  // 定位写指针到第三个记录
    iofile.write("Jane", strlen("Jane"));
    iofile.seekg(0, ios::beg); // 定位读指针到开头
    while(iofile.read((char*) &s, sizeof(s))){
        cout << s.name << " " << s.score << endl;
    }
    iofile.close();
    return 0;
}
```

文件拷贝程序示例

用法：

```
mycopy src.dat dest.dat
```

将src.dat拷贝到dest.dat，如果dest.dat原来就有，则会覆盖原来的文件

```cpp
#include<iostream>
#include<fstream>
using namespace std;

int main(int argc, char *argv[]){
    if(argc != 3){
        cout << "File name missing!" << endl;
        return 0;
    }
    // 打开文件用于读
    ifstream inFile(argv[1], ios::binary | ios::in);
    if(!inFile){
        cout << "Source file open error." << endl;
        return 0;
    }
    // 打开文件用于写
    ofstream outFile(argv[2], ios::binary | ios::out);
    if(!outFile){
        cout << "New file open error." << endl;
        inFile.close();     // inFile打开了，但是outFile没有打开，要把inFile关掉
        return 0;
    }

    char c;
    while(inFile.get(c)){  // 每次读取一个字符并写入outFile中
        outFile.put(c);
    }
    outFile.close();
    inFile.close();
    return 0;
}
```

## 10.3 二进制文件和文本文件的区别

### 文本文件的换行

- linux、unix下的换行符号：`\n`（ascii码：0x0a）
- windows下的换行符：`\r\n`（ascii码：0x0d0a）  endl就是`\n`
- mac os下的换行符：`\r`（ascii码：0x0d）

导致linux、mac os文本文件在windows记事本中打开时不换行

linux/unix下打开文件，用不用binary没区别
windows下打开文件，如果不用binary，则：

- 读文件时，`\r\n`(0x0d0a)会被当成`\n`(0x0a)
- 写文件时，写入单独的`\n`(0x0a)时，系统会自动加`\r`，变成0x0d0a
