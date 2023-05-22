import ctypes
ll = ctypes.cdll.LoadLibrary
lib = ll('./build/libtest.so')
lib.test.argtypes = [ctypes.c_char_p]
lib.test.restype = ctypes.c_int
cpp_out = lib.test('abcd'.encode('utf-8'))
print(cpp_out)
