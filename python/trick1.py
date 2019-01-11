#!/usr/bin/env python
# coding=utf-8

import constobject

def main():
    seasons = ['Spring', 'Summer', 'Fall', 'Winter']
    # z = list(enumerate(seasons))
    for i, element in enumerate(seasons):
        print(i, element)
    print("1 ----------------------")

    my_dict = {i: i * i for i in range(10)} # 推导生成一个字典
    for k,v in my_dict.items():
        print(v)
    print("2 ----------------------")

    my_set = [i * 15 for i in range(10)] # 推导生成一个list。 如果将中括号改为大括号，就是推导成一个字典。
    for i, element in enumerate(my_set):
        print(i, element)
    print("3 ----------------------")

    foo = "yasoob"
    print(foo[::-1]) # 反序

    l = ['a', 1, 2, 3, 'e']
    print(''.join(str(x) for x in l)) # 合并字符串
    print("4 ----------------------")

    # define a const variable.
    constobject.aa = 1000
    print(constobject.aa)


if __name__ == ("__main__"):
    main();