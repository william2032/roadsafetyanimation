import subprocess
import os

file_name = "app"
include_dir = "dependencies/include"
lib_dir = "dependencies/lib"
lib = "-lglew32s -lglfw3 -lopengl32 -lgdi32"
preprocessor = "-DGLEW_STATIC"


def main():
    # Remove old object files to prevent conflicts
    subprocess.call("del *.o", shell=True)  # For Windows

    # Compile
    compile_cmd = f"g++ -c main.cpp -I  {include_dir} {preprocessor}"
    print(f"Compiling: {compile_cmd}")
    subprocess.call(compile_cmd, shell=True)

    # Link
    link_cmd = f"g++ *.o -o {file_name} -L {lib_dir} {lib} -static"
    print(f"Linking: {link_cmd}")
    subprocess.call(link_cmd, shell=True)


if __name__ == "__main__":
    main()
