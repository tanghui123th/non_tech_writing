import json


filename = 'person_info.json'
with open(filename) as f_ojb:
    info = json.load(f_ojb)

print(info)   # 会输出  {'name': 'leo sirius', 'age': 24, 'gender': None}