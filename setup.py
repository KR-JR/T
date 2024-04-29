from cx_Freeze import setup, Executable

# Dependencies are automatically detected, but some modules need manual configuration
build_exe_options = {
    "packages": ["os"],  # 필요한 패키지 목록
    "excludes": ["tkinter"],  # 제외할 패키지 목록
    "include_files": []  # 포함할 추가 파일 또는 폴더
}

base = None

setup(
    name="MyApp",
    version="0.1",
    description="My application!",
    options={"build_exe": build_exe_options},
    executables=[Executable("time2.py", base=base)]
)
