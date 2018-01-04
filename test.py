from random import randint,random,sample
Enchar = 'abcdefghijklmnopqrstuvwsyz'
Sex = '男女'
number = [];

while(1):
    n = int(input("请输入需要的数据组数"))
    if(n == 0):
        break;
    
    numberup = int(input("请输入学号上限"))
    numberdown = int(input("请输入学号下限"))
    

    for i in range(numberdown,numberup+1):
        number.append(i)

    number = sample(number,n)
    namen = int(input("请输入名字长度"))
    name = ''

    for i in range(n-1):
        name = ''
        for j in range(namen):
             name += Enchar[randint(0,23)]
        print(number[i],' ',name,' ',Sex[randint(0,1)],' ',randint(10000000000,99999999999),' %.2f %.2f %.2f %.2f %.2f %.2f'%(random()*100,random()*100,random()*100,random()*100,random()*100,random()*100))

