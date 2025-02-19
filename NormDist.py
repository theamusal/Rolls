from cffi import FFI

# Define C++ function that are being exported by the dll
ffi = FFI()
ffi.cdef("""
    int* getrolls(int arg);
    void setdice(int argdice, int argrolls, int argplus);
    const char* stats();
    bool writerolls(char* filename, int numberofrolls);
""")

# Open dll
lib = ffi.dlopen("normaldist.dll")

# Define wrappers for the C++ functions
# Some interesting formats for recieving and passing different data types here
def GetRolls(size):
    array_ptr = lib.getrolls(size)
    numbers = ffi.unpack(array_ptr, size)
    return numbers

def SetDice(dice, rolls, plus = 0):
    lib.setdice(dice, rolls, plus)
    
def Stats():
    return ffi.string(lib.stats()).decode('utf-8')

def WriteRolls(filename, size):
    lib.writerolls(ffi.new("char[]", filename.encode()), size)