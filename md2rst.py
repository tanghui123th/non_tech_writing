import os

source_path = '/Users/leo/Github/sphinx-test/notes/source'
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
