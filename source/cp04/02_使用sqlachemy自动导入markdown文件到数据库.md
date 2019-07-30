# 使用sqlachemy自动导入markdown文件到数据库

## 原由

在线刷题的笔记做多了，一片一片发表很烦。索性直接用flask扩展的sqlachemy来自动导入数据库。

## 目前的局限

不同分类的文件目录结构不同，导致代码一些地方也要针对目录来写

## 代码

```python
import os
import datetime

from flask import Flask
from flask_sqlalchemy import SQLAlchemy

from flaskblog.models import Solution

all_md_file_list = []


def is_markdown_file(filename):
    return True if filename[-3:] == '.md' else False


def collect_md_file(rootDir):
    lists = sorted(os.listdir(rootDir))
    md_in_one_folder = []
    for item in lists:
        path = os.path.join(rootDir, item)
        if os.path.isdir(path):
            collect_md_file(path)
        if is_markdown_file(item):
            md_in_one_folder.append(path)
    all_md_file_list.append(md_in_one_folder)


# 从题号到[title,desc,slt]的映射
def convert_machine_guide_to_dict(md_file_list):
    all_solutions = {}
    for one_solution in md_file_list:
        for item in one_solution:
            file = open(item, 'r')
            file_content = file.read()

            path_array = item.split('/')
            num_str = path_array[-3][:2] + path_array[-2][:2]
            if num_str not in all_solutions.keys():
                all_solutions[num_str] = [path_array[-2][3:]]
                all_solutions[num_str].append(file_content)
            else:
                all_solutions[num_str].append(file_content)
            file.close()
    return all_solutions


def convert_leetcode_to_dict(md_file_list):
    all_solutions = {}
    for one_solution_list in md_file_list:
        if not one_solution_list:
            continue
        one_solution = one_solution_list[0]
        file = open(one_solution, 'r')
        file_content = file.read()

        path_array = one_solution.split('/')
        num_str = path_array[-2][:4]
        all_solutions[num_str] = [path_array[-2][4:]]
        all_solutions[num_str].append(file_content)
        file.close()
    return all_solutions


if __name__ == '__main__':
    app = Flask(__name__)
    app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql+pymysql://root:z13547842355@127.0.0.1/flaskblog_db'
    db = SQLAlchemy()
    db.init_app(app)
    # 先删除原有数据
    with app.app_context():
        db.session.query(Solution).delete()
        db.session.commit()

    MACHINE_TEST_GUIDE_DIR = '/Users/leo/Github/OJ_collections/machine_test_guide'
    LEETCODE_DIR = '/Users/leo/Github/OJ_collections/leetcode'

    collect_md_file(LEETCODE_DIR)
    leetcode_md_list = all_md_file_list
    all_leetcode_solutions = convert_leetcode_to_dict(leetcode_md_list)
    sub_id = 1
    # all_machine_guide_solutions = {'k': [title,slt]}
    for k in all_leetcode_solutions:
        dt = datetime.datetime.now()
        solution = Solution(sub_id=sub_id,
                            title=all_leetcode_solutions[k][0],
                            description='',
                            solution=all_leetcode_solutions[k][1],
                            category_id=1)
        with app.app_context():
            db.session.add(solution)
            db.session.commit()
        sub_id += 1
    all_md_file_list.clear()  # global variable, clear after use

    collect_md_file(MACHINE_TEST_GUIDE_DIR)
    machine_guide_md_list = all_md_file_list
    all_machine_guide_solutions = convert_machine_guide_to_dict(machine_guide_md_list)
    sub_id = 1
    # all_machine_guide_solutions = {'k': [title,desc,slt]}
    for k in all_machine_guide_solutions:
        dt = datetime.datetime.now()
        solution = Solution(sub_id=sub_id,
                            title=all_machine_guide_solutions[k][0],
                            description=all_machine_guide_solutions[k][1],
                            solution=all_machine_guide_solutions[k][2],
                            category_id=2)
        with app.app_context():
            db.session.add(solution)
            db.session.commit()
        sub_id += 1
    all_md_file_list.clear()
```
