import sys
import os
import shutil
import zipfile
from pathlib import Path
import subprocess

EXE_NAME = "Ardu_block_libs.exe"

def build():
    base_dir = Path(__file__).parent
    arduino_dir = base_dir / "Arduino"
    archive = base_dir / "Arduino.zip"

    # 1. Створюємо архів з папки Arduino
    if archive.exists():
        archive.unlink()
    shutil.make_archive("Arduino", "zip", arduino_dir)
    print(f"✅ Архів створено: {archive}")

    # 2. Запускаємо PyInstaller з вшитим архівом
    cmd_pyinstaller = [
        "--onefile",
        f"--add-data={archive};.",
        f"--name={EXE_NAME}",
        str(base_dir / __file__)
    ]
    print("⚙️ Виконую: PyInstaller ", " ".join(cmd_pyinstaller))
    try:
        subprocess.run([sys.executable, "-m", "PyInstaller"] + cmd_pyinstaller, check=True)
    except subprocess.CalledProcessError as e:
        print("❌ Помилка під час створення EXE:", e)
        print('Переконайтеся, що PyInstaller встановлено \n pip install pyinstaller')
        sys.exit(1)

    # 3. Прибираємо сміття
    shutil.rmtree("build", ignore_errors=True)
    shutil.rmtree("__pycache__", ignore_errors=True)
    spec_file = Path(f"{EXE_NAME}.spec")
    if spec_file.exists():
        spec_file.unlink()


    dist_dir = base_dir / "dist" 
    shutil.move(dist_dir / EXE_NAME, base_dir / EXE_NAME)

    if dist_dir.exists():
        shutil.rmtree(dist_dir, ignore_errors=True)
    
    if archive.exists():
        archive.unlink()

    print(f"✅ EXE зібрано {EXE_NAME}")

def install():
    import tempfile

    # знаходимо архів усередині exe
    if hasattr(sys, "_MEIPASS"):
        archive = Path(sys._MEIPASS) / "Arduino.zip"
    else:
        archive = Path(__file__).parent / "Arduino.zip"

    if not archive.exists():
        print(f"❌ Архів не знайдено: {archive}")
        sys.exit(1)

    # шлях до Documents/Arduino
    target = Path.home() / "Documents" / "Arduino"
    target.mkdir(parents=True, exist_ok=True)

    # тимчасова папка
    tmp = Path(tempfile.mkdtemp())

    try:
        # розпаковка архіву
        with zipfile.ZipFile(archive, "r") as zip_ref:
            zip_ref.extractall(tmp)

        # визначаємо, де знаходиться папка Arduino
        # якщо верхньої папки немає, використовуємо tmp як джерело
        extracted_dirs = [p for p in tmp.iterdir() if p.is_dir()]
        if len(extracted_dirs) == 1 and extracted_dirs[0].name.lower() == "arduino":
            source = extracted_dirs[0]
        else:
            source = tmp  # беремо всі файли з архіву

        # переносимо вміст у Documents/Arduino
        for item in source.iterdir():
            dest = target / item.name
            if dest.exists():
                shutil.rmtree(dest) if item.is_dir() else dest.unlink()
            shutil.move(str(item), dest)

        print("✅ Arduino з бібліотеками та tools встановлено в Documents/Arduino")

    finally:
        # прибираємо тимчасові файли
        shutil.rmtree(tmp, ignore_errors=True)


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] in ("-b", "--build"):
        build()
    else:
        install()
