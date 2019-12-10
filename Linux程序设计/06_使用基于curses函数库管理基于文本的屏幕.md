# 6 使用curses函数库管理基于本文的屏幕

第五章讲的GTI、termios和tparm等还是太过底层。curses是一个层高层的接口，使屏幕程序更为简单。Linux中使用的版本的ncurses（new curses）

## 6.2 curses术语和概念

- `stdscr`和`curscr` 这是两个表示终端屏幕的数据结构
- `refresh`函数：调用时会curses库会比较curscr（当前屏幕）和stdscr（将要显示的屏幕），然后刷新屏幕。
- `initscr`和`endwin` 这两个函数用来在使用curses库时创建和删除一些临时的数据结构。

下面的程序会在5行15列处输出“Hello World”并停留两秒。

```c
#include<unistd.h>
#include<stdlib.h>
#include<curses.h>

int main()
{
    initscr();

    move(5, 15);
    printw("%s", "Hello World");
    refresh();

    sleep(2);

    endwin();
    exit(EXIT_SUCCESS);
}
```

## 6.3 屏幕

### 开始和结束

- `WINDOW *initscr(void);` 成功返回一个指向stdscr结构的指针。
- `int endwin(void);`

所有的curses程序必须以这两个函数开始和结束

### 输出到屏幕

- `addch(const chtype char_to_add);`
- `addstr(chtype *const string_to_add);`

chtype是curses自己的字符类型，比char占得位数要多。两个add函数在光标所在位置字符或字符串。

- `printw(char *format, ...);` 进行格式化输出到屏幕
- `refresh(void);`
- `box(WINDOW *win_ptr, chtype vertical_char, chtype horizontal_char);` 绘制方框
- `insch(chtype char_to_insert);` 插入一个字符，并把已有的字符右移
- `insertln(void);` 插入一个空白行，并将已有行下移
- `delch(void);`
- `deleteln(void);`

两个delete函数的作用与上面两个insert的作用正好相反，删除后也要左移和上移。

- `beep(void);` 蜂鸣
- `flash(void);` 闪屏

### 从屏幕读取

- `chtype inch(void);` 返回当前光标所在位置的字符
- `int instr(char *string);` 这个函数和下面的函数把返回内容写到指定的数组中
- `int innstr(char *string, int number_of_characters);`

### 清除屏幕


- `int erase(void);` 在屏幕的每个位置上写上空白字符
- `int clear(void);` 清屏
- `int clrttobot(void);` 从当前光标位置清除到屏幕结尾
- `int clrttoeol(void);` 从当前光标位置清除到行尾

### 移动光标

- `int move(int new_y, int new_x);` 将光标移动到对应位置
- `int leaveok(WINDOW *window_ptr, bool leave_flag);` flag为true，则刷新后光标会在屏幕随机放置。flag为false，则光标在刷新后位置不变。

### 字符属性

- `int attron(chtype, attribute);`
- `int attoff(chtype, attribute);`
- `int attrset(chtype, attribute);`
- `int standout(void);`
- `int standend(void);`

预定义属性有五个：

- A_BLINK
- A_BOLD
- A_DIM
- A_REVERSE
- A_STANDOUT

下面的程序是一个使用的例子

```c
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<curses.h>

int main()
{
    const char witch_one[] = " First Witch  ";
    const char witch_two[] = " Second Witch ";
    const char *scan_ptr;

    initscr();

    move(5, 15);
    attron(A_BOLD);
    printw("%s", "Macbeth");
    attroff(A_BOLD);
    refresh();
    sleep(1);

    move(8, 15);
    attron(A_STANDOUT);
    printw("%s", "Thunder and Lightning");
    attroff(A_STANDOUT);
    refresh();
    sleep(1);

    move(10, 10);
    printw("%s", "When shall we three meet again");
    move(11, 23);
    printw("%s", "In thunder, lightning, or in rain ?");
    move(13, 10);
    printw("%s", "When the hurlyburly's done,");
    move(14, 23);
    printw("%s", "When the battle's lost and won.");
    refresh();
    sleep(1);

    attron(A_DIM);
    scan_ptr = witch_one + strlen(witch_one) - 1;
    while (scan_ptr != witch_one) {
        move(10, 10);
        insch(*scan_ptr--);
    }
    scan_ptr = witch_two + strlen(witch_two) - 1;
    while (scan_ptr != witch_two) {
        move(13, 10);
        insch(*scan_ptr--);
    }
    attroff(A_DIM);
    refresh();
    sleep(1);

    move(LINES - 1, COLS - 1);
    refresh();
    sleep(10);

    endwin();
    exit(EXIT_SUCCESS);
}
```
