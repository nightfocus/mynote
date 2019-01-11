#coding:utf-8

# define const class for python

import sys


class Const:
    class ConstError(TypeError):
        def __init__(self, name):
            self.msg = "Can't rebind const instance attribute (%s)"%name

        def __str__(self):
            return 'error msg: {}'.format(self.msg)

        def __repr__(self):
            return self.__str__()

    def __setattr__(self, name, value):
        if name in self.__dict__:
            raise self.ConstError(name)

        self.__dict__[name] = value

    def __delattr__(self, name):
        if name in self.__dict__:
            raise self.ConstError(name)

        raise self.ConstError(name)


sys.modules[__name__] = Const()
