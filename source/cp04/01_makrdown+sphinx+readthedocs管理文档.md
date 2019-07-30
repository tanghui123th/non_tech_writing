# markdown+sphinx+readthdocs管理文档

## 概述

传统博客的结构性很差。sphinx生成的文档结构是个好的选择。为什么不用gitbook？因为它又丑又慢。[受此启发，点我跳转](https://python-online.cn/zh_CN/latest/c04/c04_03.html)

总体上就是：Markdown+Pandoc+Sphinx+GitHub+ReadtheDocs

- Markdown：书写文档
- Pandoc：格式转化
- Sphinx：生成网页
- GitHub：托管项目
- ReadtheDocs：发布网页

## 操作步骤

### 初始化

这里是我自己搭建时的一个简单操作流程，没有太多解释。如果是新手请有效使用搜索引擎。也欢迎和我探讨。

我是在`virtualenv`下操作的，首先是安装依赖

```
$ pip install sphinx sphinx-autobuild sphinx_rtd_theme -i https://pypi.douban.com/simple/
$ mkdir docs
$ cd docs
$ sphinx-quickstart
```

然后是填一些项目初始信息，注意在选择是否把源文件和输出文件分开时，最好选择是。这时的目录结构

```
.
├── Makefile
├── build
├── make.bat
└── source
    ├── _templates
    ├── _static
    ├── conf.py
    └── index.rst
```

- `_templates`可以删掉
- `_static`可以删掉
- `conf.py`配置文件
- `index.rst`首页
- `build`生成的html和目录结构存放的地方
- `Makefile`不用管
- `make.bat`不用管

这时使用命令`make html`，就会生成博客的HTML页面，打开`build/html/index.html`，就可以看到网页内容。

现在的样式还有点丑，我们换一个。打开`conf.py`，设置`html_theme = 'default'`

### 转化MarkDown为RST

先安装pandoc

```
$ brew install pandoc
```

使用这个脚本，会把source下的所有md文件转换新生成一个rst文件

```python
# docs/md2rst.py
import os

source_path = '/Users/leo/Github/sphinx-test/docs/source'
all_md_file = []

def collect_md_file(rootDir):
    '''
    collect all md file inside rootDir
    '''
    lists = os.listdir(rootDir)
    for item in lists:
        path = os.path.join(rootDir, item) 
        if os.path.isdir(path): 
            collect_md_file(path)
        if path.endswith('md'): 
            all_md_file.append(path)

def execute_conver_cmd():
    for file in all_md_file:
        print('convert: ' + file)
        (filename, extension) = os.path.splitext(file)
        convert_cmd = 'pandoc -f markdown -t rst {md_file} -o {rst_file}'.format(
            md_file=filename+'.md', rst_file=filename+'.rst'
        )
        os.system(convert_cmd)

if __name__ == '__main__':
    collect_md_file(source_path)
    execute_conver_cmd()
    print('done!')
```

### Github部署、ReadTheDocs发布

git的使用就不在这里介绍了。注意一点`.gitignore`的内容，`build/`下的东西不用推上去。最后去ReadTheDocs上注册一个账号，再关联github仓库就可以了。

```
build/
*.pyc
Makefile
```