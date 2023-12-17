import os
import shutil
import datetime

def filterfile():
    '''
    该函数实现从文件夹中根据文件后缀名提取文件，并存储在新的文件夹中
    file_dir指读的文件目录;save_dir为保存文件的目录
    suffix用于存放打算提取文件的后缀名;
    '''

    file_dir = os.getcwd()
    print ("当前工作目录 : %s" % os.getcwd())
    save_dir = os.getcwd() + "\Include_files" #在当前根目录下新建一个文件夹用于存放提取出来的文件
    suffix = ['h']
    if os.path.exists(save_dir):
       shutil.rmtree(save_dir)  #如果已经存在该文件夹，移除
    if not os.path.exists(save_dir):
        os.makedirs(save_dir) #如果不存在该文件夹，则创建，用于储存后续提取出来的文件
    filelist = []  #存储要copy的文件全名
    Count_files = 0
    for dirpath,dirnames,filenames in os.walk(file_dir):#根据路径执行树状的遍历，分别遍历根目录，根目录下的文件夹，文件夹下的文件
        for file in filenames:#遍历文件夹中的文件
            file_type = file.split('.')[-1]#对文件名根据.进行分隔，实现文件名，后缀名的分离
            if(file_type in suffix):#下面根据后缀名是否在列表中，提取文件
                file_fullname = os.path.join(dirpath, file) #文件全名
                filelist.append(file_fullname)#将符合要求的文件存放在列表中
    for file in filelist:
        shutil.copy(file, save_dir)#将列表中的文件复制到新的文件夹
        Count_files += 1
    print("文件提取结束! 共%d个文件" % Count_files) 
    
def display_current_time():
    #用于显示当期时间
    current_datetime = datetime.datetime.now()
    formatted_datetime = current_datetime.strftime("%Y-%m-%d %H:%M:%S")
    print(formatted_datetime)

if __name__ == "__main__":
    display_current_time()
    filterfile()
    display_current_time()
