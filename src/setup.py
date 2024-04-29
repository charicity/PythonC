from distutils.core import setup, Extension

def main():
    setup(name="adds",
          version="1.0.0",
          description="Python interface for the set C library function",
          ext_modules=[Extension("mymodule", ["main.cpp"])])

if __name__ == "__main__":
    main()