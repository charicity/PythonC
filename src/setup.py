from distutils.core import setup, Extension

def main():
    setup(name="adds",
          version="1.0.0",
          description="Python interface for the fputs C library function",
          ext_modules=[Extension("adds", ["main.cpp"])])

if __name__ == "__main__":
    main()