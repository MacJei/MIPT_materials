import sys
import functools
import datetime


def singleton(cls):
    instance = None
    @functools.wraps(cls)
    def wrapper(*args, **kwargs):
        nonlocal instance
        if instance is None:
            instance = cls(*args, **kwargs)
        return instance
    return wrapper

@singleton
class _Logger(object):
    def __init__(self):
        self.index = 0
        
    def log_function_call(self, func, dest, *args, **kwargs):
        now = datetime.datetime.now()
        func_output = func(*args, **kwargs)
        if dest == sys.stderr or dest == sys.stdout:
            print(self.index, str(now), func.__name__, args, kwargs, func_output, file=dest)
        else:
            f = open(dest, 'a')
            f.write(self.index, str(now), func.__name__, args, kwargs, func_output)
            f.close()
        self.index += 1
        return
    
    
def Logger(dest):
    logger = _Logger()
    def decore(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            return logger.log_function_call(func, dest, *args, **kwargs)
        return wrapper
    return decore

