#!/usr/bin/env python
# coding=utf-8

# 求协方差，方差，标准差，协方差相关系数, 以及余弦相似度的demo
# 定义拷贝自：https://www.zhihu.com/question/20852004
#
# 协方差：  Cov(X,Y)
#          如果有X,Y两个变量，每个时刻的“X值与其均值之差”乘以“Y值与其均值之差”得到一个乘积，
#          再对这每时刻的乘积求和并求出均值（其实是求“期望”，但就不引申太多新概念了，简单认为就是求均值了）。
# 方差：    每一时刻变量值与变量均值之差再平方，求得一个数值，再将每一时刻这个数值相加后求平均。这个值代表了 偏离均值的幅度
# 标准差：  就是方差的值，进行开方。之所以有标准差，是为了将这个偏离均值的幅度还原回原来的量级。
# 相关系数：Cov(X,Y)/(σXσY)，用来计算多组数据集的相似性，或者说方向是否一致。
#          协方差除以 标准差的乘积
#          该系数v为 -1 <= v <= 1, v=1时，表示正向相似度最大，v=-1则反向最大，0表示两者无关
#
# 余弦相似度：通过测量两个向量点积空间夹角的余弦值来判断相似性，大小在[-1,1]区间。
#

import math
import numpy as np

# 获取X, Y的协方差相关系数
def get_cov_coe(X, Y, precision=2):
    # 求每个数列的均值
    x_avg = round(sum(X)/len(X), precision)
    y_avg = round(sum(Y)/len(Y), precision)
    print(x_avg, y_avg)

    cov = 0
    for i in range(len(X)):
        cov += (X[i] - x_avg) * (Y[i] - y_avg) # X,Y每个时刻的值与均值差的乘积和
    cov /= len(X) # 对乘积和求出均值
    cov = round(cov, precision)

    print("X,Y 协方差：", cov)

    # 求数列lst的标准差函数
    def get_variance(lst, avg):
        vv = 0
        for i, val in enumerate(lst):
            vv += (val - avg) ** 2
        vv /= len(X)
        return round(math.sqrt(vv), precision)

    x_variance = get_variance(X, x_avg)
    y_variance = get_variance(Y, y_avg)

    print("X,Y 各自标准差：", x_variance, y_variance)

    xy_cov_coe = cov / float(x_variance * y_variance)
    return xy_cov_coe

# 获取X, Y的协方差相关系数（直接使用numpy库）
# 它是输出一个矩阵
def get_cov_coe_withnumpy(X, Y):
    tx = np.array(X)
    ty = np.array(Y)

    print("X,Y的协方差距阵：\n", np.cov(X, Y, bias = True))
    # 输出2*2的矩阵，[0][0] 是X的标准差，[0][1]表示对两个输入量的协方差
    # [1][0]等同[0][1]，[1][1]是Y的标准差

    return np.corrcoef(X, Y, bias=True)
    #return np.corrcoef(np.vstack((X,Y)), bias=True)


# 获取X, Y的余弦相似度
def get_cos(X, Y):
    tn = 0
    cx = 0
    cy = 0
    for i in range(len(X)):
        tn += X[i] * Y[i]
        cx += X[i] ** 2
        cy += Y[i] ** 2
    return tn / float(math.sqrt(cx) * math.sqrt(cy))

# 获取X, Y的余弦相似度（直接使用numpy库）
def get_cos_withnumpy(X, Y):
    tx = np.array(X)
    ty = np.array(Y)
    cos1 = np.sum(tx * ty)
    cos21 = np.sqrt(sum(tx ** 2))
    cos22 = np.sqrt(sum(ty ** 2))
    return cos1 / float(cos21 * cos22)



def main():
    X = [19.1,  20.3, 25.8,  22.6,  32.9,  40.1,  29.2]
    Y = [103.0, 140,  314.4, 219.7, 490.3, 1012.1, 485.0]
    # Y = [20,   25,    60,    27,    338,501,    509]

    x_avg = round(sum(X) / len(X), 4)
    cov = 0
    for i in range(len(X)):
        cov += (X[i] - x_avg)
    cov /= len(X) # 对乘积和求出均值
    cov = round(cov, 4)

    print("X的标准差：", math.sqrt(np.cov((X), bias = True)))

    print("----------------")

    cov_coe = get_cov_coe(X, Y, precision=4)
    print("X,Y 协方差相关系数：", cov_coe)
    cov_coe2 = get_cov_coe_withnumpy(X, Y)
    print("X,Y 协方差相关系数(with numpy)：\n", cov_coe2)

    print("----------------------------------------")
    cos = get_cos(X, Y)
    print("X,Y 余弦相似度值：", cos)
    cos2 = get_cos_withnumpy(X, Y)
    print("X,Y 余弦相似度值(with numpy)：", cos2)

if __name__ == ("__main__"):
    main();
